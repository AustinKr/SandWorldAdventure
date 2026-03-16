#include "SWA/Player/Player.h"
#include "SWA/Player/PlayerInventoryGUI.h"

#include "SWA/RenderLayerNames.h"
#include "SWA/ShaderNames.h"

#include "SWA/Game.h"

#include "SWAEngine/Tilemap/TileBehavior/Types.h"

#include "GP2D/Pipeline/GenericPipeline.h"
#include "GP2D/Pipeline/Window/Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace GP2D::Pipeline;
using namespace SWAEngine::Math;
using namespace GP2D::Math;

namespace SWA::Player
{
	void Player::SetInputs()
	{
		// Keyboard
		if (Window::Window::GetKeyState(GLFW_KEY_UP) || Window::Window::GetKeyState(GLFW_KEY_W))
			m_Movement.Y = 1;
		else if (Window::Window::GetKeyState(GLFW_KEY_DOWN) || Window::Window::GetKeyState(GLFW_KEY_S))
			m_Movement.Y = -1;

		if (Window::Window::GetKeyState(GLFW_KEY_RIGHT) || Window::Window::GetKeyState(GLFW_KEY_D))
			m_Movement.X = 1;
		else if (Window::Window::GetKeyState(GLFW_KEY_LEFT) || Window::Window::GetKeyState(GLFW_KEY_A))
			m_Movement.X = -1;

		m_ShouldBreakTile = Window::Window::GetKeyState(GLFW_MOUSE_BUTTON_1);
		m_ShouldAddTile = Window::Window::GetKeyState(GLFW_MOUSE_BUTTON_2);
	}

	void Player::TryUseItem(Item item)
	{
		switch (item.Type)
		{
		case PLAYER_ITEM_TYPE_TILE:
			if (m_ShouldAddTile || m_ShouldBreakTile)
			{
				// Get cursor position
				double cursorX, cursorY;
				glfwGetCursorPos(Window::Window::sp_Window, &cursorX, &cursorY);
				cursorY = GenericPipeline::s_ActiveCamera.GetScreenSize().Y - cursorY; // Invert so that now it's from the bottom left of the screen

				// Do so conversions
				Float2 mouseWorldPosition =
					GenericPipeline::s_ActiveCamera.ViewportToWorld(
						GenericPipeline::s_ActiveCamera.ScreenToViewport({ int(cursorX), int(cursorY) }));
				Vector2Int mouseTilePosition = Game::p_Tilemap->WorldToTile({ mouseWorldPosition.X, mouseWorldPosition.Y });

				// remove/add tiles at cursor
				UseCurrentTileItem(item, mouseTilePosition);
			}
			break;

			// TODO: Do other stuff if different item type
		}
	}
	void Player::UseCurrentTileItem(Item item, Vector2Int mouseTilePosition)
	{
		double radius = 5;
		for (int i = -radius; i < radius; i++)
		{
			for (int j = -radius; j < radius; j++)
			{
				if (i * i + j * j >= radius * radius)
					continue;

				Vector2Int position = mouseTilePosition + Vector2Int(i, j);
				if (position.X < 0 || position.Y < 0)
					continue;
				if (m_ShouldAddTile)
					Game::p_Tilemap->SetTile(position, { item.BehaviorUID, MixColor(item.Color, item.ColorDeviation), true});
				else
					Game::p_Tilemap->SetTile(position, {});
			}
		}
	}


	void Player::TryApplyVelocity()
	{
		// TODO: Implement step move

		auto last = GetPosition();

		SetPosition(GetPosition() + m_Velocity * m_Time.FrameDeltaTime);

		if (IsColliding())
		{
			// Move back
			SetPosition(last);
			// Assumed to hit immovable object
			m_Velocity *= .01;
			m_Acceleration = { };
		}
	}

	GP2D::Pipeline::GP2D_HEX_COLOR Player::MixColor(GP2D::Pipeline::GP2D_HEX_COLOR colA, GP2D::Pipeline::GP2D_HEX_COLOR colB)
	{
		int r1 = (colA >> 24) & 0xFF;
		int g1 = (colA >> 16) & 0xFF;
		int b1 = (colA >> 8) & 0xFF;
		int a1 = colA & 0xFF;

		int r2 = (colB >> 24) & 0xFF;
		int g2 = (colB >> 16) & 0xFF;
		int b2 = (colB >> 8) & 0xFF;
		int a2 = colB & 0xFF;

		float fac = (float)rand() / RAND_MAX;

		int red = (float)r1 * fac + r2 * (1.0f - fac);
		int green = (float)g1 * fac + g2 * (1.0f - fac);
		int blue = (float)b1 * fac + b2 * (1.0f - fac);
		int alpha = (float)a1 * fac + a2 * (1.0f - fac);

		return alpha | (blue << 8) | (green << 16) | (red << 24);
	}

	Player::Player() :
		BaseGameObject("Player"),
		CurrentInventory{}, 
		Gravity(1), m_LastVelocity{}, m_Velocity{}, m_Dampening(.98), Speed{1.0},
		m_Time{}, CameraFollowSpeed{ 1 }, m_ShouldAddTile(0), m_ShouldBreakTile(0), m_Movement{}
	{
		// Create the mesh
		mp_Mesh = new Mesh::Mesh();
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Characters).RegisterMesh(mp_Mesh);
		mp_Mesh->Scale = { 0.125f, 0.25f};
		mp_Mesh->Vertices =
		{
			{{ 0, 0 }, {0, 0, 0}},
			{{ 1, 0 }, {1, 0, 0}},
			{{ 1, 1 }, {1, 1, 0}},
			{{ 0, 1 }, {0, 1, 0}},
		};
		mp_Mesh->Triangles =
		{
			0, 1, 2,
			0, 2, 3
		};
		mp_Mesh->Shaders =
		{
			0, 6, SWA_TEXTURE_SHADER
		};

		// Create player inventory and gui
		CurrentInventory.Assign({ 4,8 }, {});
		CurrentInventory.SetItemAt({ 1,1 }, Item(PLAYER_ITEM_TYPE_TILE, "wet_sand_tile_slot", 0xeccc70ff, 0xff0000ff, SWAEngine::Tilemap::TileBehavior::SOLID));
		CurrentInventory.SetItemAt({ 3,1 }, Item(PLAYER_ITEM_TYPE_TILE, "sand_tile_slot", 0xffe082ff, 0xff0000ff, SWAEngine::Tilemap::TileBehavior::SAND));
		PlayerInventoryGUI::Initialize(CurrentInventory);
	}

	void Player::Update(SWAEngine::Time time)
	{
		m_Time = time;

		// Use selected item
		TryUseItem(CurrentInventory.GetItemAt(CurrentInventory.SelectedItemPosition));

		// Move
		SetInputs();
		Accelerate(Vector2::Normalize(m_Movement) * Speed);

		Accelerate({ 0, -Gravity });

		// Apply physics
		m_Velocity += m_Acceleration * m_Time.FrameDeltaTime;
		m_Velocity *= m_Dampening;

		m_LastVelocity = m_Velocity;

		// Move
		TryApplyVelocity();

		// Move camera
		GenericPipeline::s_ActiveCamera.Origin += (mp_Mesh->Origin - GenericPipeline::s_ActiveCamera.Origin) * float(m_Time.FrameDeltaTime * CameraFollowSpeed);
	}
	void Player::Release()
	{
		CurrentInventory.Release(); // Clears/releases everything associated with the player inventory
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Characters).UnregisterMesh(mp_Mesh); // Unregister the mesh

		BaseGameObject::Release();
	}

	Vector2 Player::GetPosition()
	{
		return Vector2(mp_Mesh->Origin.X, mp_Mesh->Origin.Y);
	}
	void Player::SetPosition(Vector2 newPosition)
	{
		mp_Mesh->Origin = { (float)newPosition.X, (float)newPosition.Y };
	}
	Vector2 Player::GetScale()
	{
		return Vector2(mp_Mesh->Scale.X, mp_Mesh->Scale.Y);
	}
	void Player::SetScale(Vector2 newScale)
	{
		mp_Mesh->Scale = { (float)newScale.X, (float)newScale.Y };
	}


	SWAEngine::Math::Vector2 Player::GetVelocity()
	{
		return m_Velocity;
	}
	void Player::AddVelocity(SWAEngine::Math::Vector2 vel)
	{
		m_Velocity += vel;
	}
	void Player::Accelerate(SWAEngine::Math::Vector2 acc)
	{
		m_Acceleration += acc;
	}
	SWAEngine::Math::Vector2 Player::GetAcceleration()
	{
		return (m_Velocity - m_LastVelocity) / m_Time.FrameDeltaTime;
	}

	bool Player::IsColliding()
	{
		return 
			GetPosition().X < 0 || GetPosition().Y < 0 ||
			Game::p_Tilemap->DetectCollisionRect(GetPosition(), GetPosition() + GetScale());
	}
}