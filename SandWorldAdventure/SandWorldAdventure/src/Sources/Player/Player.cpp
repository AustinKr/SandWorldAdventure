#include "SWA/Player/Player.h"
#include "SWA/Player/PlayerInventoryGUI.h"

#include "SWA/RenderLayerNames.h"
#include "SWA/ShaderNames.h"

#include "SWA/Game.h"

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
			AccY = 1;
		else if (Window::Window::GetKeyState(GLFW_KEY_DOWN) || Window::Window::GetKeyState(GLFW_KEY_S))
			AccY = -1;
		else// if (!MasterWindow::GetKeyState(GLFW_KEY_UP) && !MasterWindow::GetKeyState(GLFW_KEY_DOWN))
			AccY = 0;

		if (Window::Window::GetKeyState(GLFW_KEY_RIGHT) || Window::Window::GetKeyState(GLFW_KEY_D))
			AccX = 1;
		else if (Window::Window::GetKeyState(GLFW_KEY_LEFT) || Window::Window::GetKeyState(GLFW_KEY_A))
			AccX = -1;
		else
			AccX = 0;

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
					Game::p_Tilemap->SetTile(position, { item.BehaviorUID, item.Color, true});
				else
					Game::p_Tilemap->SetTile(position, {});
			}
		}
	}

	Player::Player() :
		BaseGameObject("Player"),
		CurrentInventory{}, AccX(0), AccY(0), m_Vel({}), m_Dampening(.98), Speed{ 1.0 }, CameraFollowSpeed{ 1 }, m_ShouldAddTile(0), m_ShouldBreakTile(0)
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
		CurrentInventory.SetItemAt({ 1,1 }, Item(PLAYER_ITEM_TYPE_TILE, "wet_sand_tile_slot", 0xeccc70ff, 0)); // TODO: Get behaviors
		CurrentInventory.SetItemAt({ 3,1 }, Item(PLAYER_ITEM_TYPE_TILE, "sand_tile_slot", 0xffe082ff, 0));
		PlayerInventoryGUI::Initialize(CurrentInventory);
	}

	Vector2 Player::GetPosition()
	{
		return Vector2(mp_Mesh->Origin.X, mp_Mesh->Origin.Y);
	}
	void Player::SetPosition(Vector2 newPosition)
	{
		mp_Mesh->Origin = { (float)newPosition.X, (float)newPosition.Y };
	}

	void Player::Update(SWAEngine::Time time)
	{
		SetInputs();

		// Use selected item
		TryUseItem(CurrentInventory.GetItemAt(CurrentInventory.SelectedItemPosition));

		//// Testing raycast
		//Vector2 originTiles = ((Tilemap::Tilemap* const)mp_Tilemap)->FromWorldToTile(GetPosition());
		//Vector2 difference = (mouseTilePosition - originTiles), direction = difference.Normalize();
		//bool succeeded = true;
		//auto pairHit = ((Tilemap::Tilemap*)mp_Tilemap)->Raycast(originTiles, difference, &succeeded);
		//if (succeeded)
		//{
		//	((Tilemap::Tilemap*)mp_Tilemap)->AddTile(originTiles, Tilemap::Tile(0xff0000ff, 0));
		//	((Tilemap::Tilemap*)mp_Tilemap)->AddTile(pairHit.first, Tilemap::Tile(0xff0000ff, 0));
		//}


		// Physics
		m_Vel += Vector2::Normalize(Vector2(AccX, AccY)) * time.FrameDeltaTime * Speed;
		m_Vel.X *= m_Dampening;
		m_Vel.Y *= m_Dampening;

		// Move camera
		SetPosition(GetPosition() + m_Vel * time.FrameDeltaTime);
		GenericPipeline::s_ActiveCamera.Origin += (mp_Mesh->Origin - GenericPipeline::s_ActiveCamera.Origin) * float(time.FrameDeltaTime * CameraFollowSpeed);
	}
	void Player::Release()
	{
		CurrentInventory.Release(); // Clears/releases everything associated with the player inventory
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Characters).UnregisterMesh(mp_Mesh); // Unregister the mesh

		BaseGameObject::Release();
	}
}