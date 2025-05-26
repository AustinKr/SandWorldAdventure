#include "HeaderFiles/Game/GameObjects/Player.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/RenderLayerNames.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehaviorNames.h"

namespace SandboxEngine::Game::GameObject
{
	Player::Player(IGameObject* pTilemap) : mp_Tilemap(pTilemap), AccX(0), AccY(0), m_Vel({}), m_Dampening(.98), Speed{80.0}, CameraFollowSpeed{2}, m_ShouldAddTile(0), m_ShouldBreakTile(0)
	{
		if (mp_Tilemap == nullptr)
			throw std::exception("Tilemap cannot be nullptr!");

		// GraphicsPipeline2D::Release() releases registered mesh data
		mp_Mesh = new GraphicsPipeline::Mesh(); // Create its mesh
		MasterWindow::Pipeline.GetLayer(RENDERLAYERS_Characters).RegisterMesh(mp_Mesh); // Register the mesh

		mp_Mesh->Scale = float2(15.0f, 30.0f);
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
			0, 2, GraphicsPipeline::GraphicsPipeline2D::GP2D_PLAYER_SHADER
		};
	}

	Vector2 Player::GetPosition()
	{
		return Vector2(mp_Mesh->Origin.X, mp_Mesh->Origin.Y);
	}
	void Player::SetPosition(Vector2 newPosition)
	{
		mp_Mesh->Origin = float2(newPosition.X, newPosition.Y);
	}

	void Player::Update(Time time)
	{
		// Keyboard

		if (MasterWindow::GetKeyState(GLFW_KEY_UP))
			AccY = 1;
		else if (MasterWindow::GetKeyState(GLFW_KEY_DOWN))
			AccY = -1;
		else// if (!MasterWindow::GetKeyState(GLFW_KEY_UP) && !MasterWindow::GetKeyState(GLFW_KEY_DOWN))
			AccY = 0;

		if (MasterWindow::GetKeyState(GLFW_KEY_RIGHT))
			AccX = 1;
		else if (MasterWindow::GetKeyState(GLFW_KEY_LEFT))
			AccX = -1;
		else
			AccX = 0;

		m_ShouldBreakTile = MasterWindow::GetKeyState(GLFW_MOUSE_BUTTON_1);
		m_ShouldAddTile = MasterWindow::GetKeyState(GLFW_MOUSE_BUTTON_2);


		// Get cursor position
		Vector2 cursorPosition;
		glfwGetCursorPos(MasterWindow::p_glfwWindow, &cursorPosition.X, &cursorPosition.Y);
		Vector2 mouseWorldPosition =
			MasterWindow::Pipeline.ActiveCamera.ViewportToWorld(
				MasterWindow::Pipeline.ActiveCamera.ScreenToViewport(
					Vector2(cursorPosition.X, MasterWindow::Pipeline.ActiveCamera.ScreenSize.Y - cursorPosition.Y)));

		// Cursor remove/add tiles
		if (m_ShouldAddTile || m_ShouldBreakTile)
		{
			Vector2 mousePosition = ((Tilemap::Tilemap*const)mp_Tilemap)->FromWorldToTile(mouseWorldPosition);
			double radius = 5;
			for (int i = -radius; i < radius; i++)
			{
				for (int j = -radius; j < radius; j++)
				{
					if (i * i + j * j >= radius * radius)
						continue;

					if (m_ShouldAddTile)
					{
						float fac = float(abs(i * j)) / float(radius * radius);
						UINT color = GraphicsPipeline::GraphicsPipeline2D::RGBA_To_UINT(0xff - 40 * fac, 0xe0 - 40 * fac, 0x82 + 60 * fac, 255);// (i * j) % 2 == 0 ? 0xff0000ff : 0x00ff00ff; //0xFFE082FF
						((Tilemap::Tilemap*)mp_Tilemap)->SetTile(Vector2Int(i + mousePosition.X, j + mousePosition.Y), Tilemap::Tile(color, Tilemap::TILE_BEHAVIOR_NAMES::Sand));
					}
					else
						((Tilemap::Tilemap*)mp_Tilemap)->RemoveTile(Vector2Int(i + mousePosition.X, j + mousePosition.Y));
				}
			}
		}


		// Physics

		m_Vel += Vector2::Normalize(Vector2(AccX, AccY)) * time.FrameDeltaTime * Speed;

		SetPosition(GetPosition() + m_Vel * time.FrameDeltaTime);
		MasterWindow::Pipeline.ActiveCamera.Origin += (GetPosition() - MasterWindow::Pipeline.ActiveCamera.Origin) * time.FrameDeltaTime * CameraFollowSpeed;

		m_Vel.X *= m_Dampening;
		m_Vel.Y *= m_Dampening;
	}
	void Player::Release()
	{
		// Not realy necessary unless player is for some reason deleted prior to the application termination

		MasterWindow::Pipeline.GetLayer(RENDERLAYERS_Characters).UnregisterMesh(mp_Mesh); // Unregister the mesh
	}
}