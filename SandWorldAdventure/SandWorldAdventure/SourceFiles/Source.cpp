// This file contains the entry point and initializes everything. It also contains game logic

// TODO: Move GUI code into its own dll

#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/RenderLayerNames.h"

#include <iostream>
#include <string>

#include "HeaderFiles/Game/Game.h"

//#include "KeyboardInterfaceAPI/Headers/KeyboardInterface/KeyboardState.h"

using namespace SandboxEngine;
using namespace SandboxEngine::Game;
using namespace SandboxEngine::Game::GameObject;

// - Forward declarations -
void InitializeGame();
void InitializeGraphics();
void InitializeGUI();
void RunGameLoopCycle();
void Release();
// Game stuff
void GenerateTilemap();

// - Variables -
Tilemap::Tilemap* gp_TestTilemap;
double TileXPosition = 0;

int main(void)
{
	// Creates a window and nearly everything else necessary relating to openGL
	MasterWindow::InitializeWindow();
	InitializeGraphics();
	InitializeGUI();
	InitializeGame();

	GameInstance::TimeInfo = {};
	double oldTime = 0;

	// Game loop
	while (!glfwWindowShouldClose(MasterWindow::p_glfwWindow))
	{
		// Keep running

		// - Draw to screen -
		glClear(GL_COLOR_BUFFER_BIT); // Background

		// Get the current and delta times
		oldTime = GameInstance::TimeInfo.CurrentTime;
		GameInstance::TimeInfo.CurrentTime = glfwGetTime();
		GameInstance::TimeInfo.FrameDeltaTime = GameInstance::TimeInfo.CurrentTime - oldTime;
		
		RunGameLoopCycle(); // Game loop
		GameInstance::UpdateObjects(); // Apply physics and logic to objects
		MasterWindow::Pipeline.RenderScene(); // Render the scene

		glfwSwapBuffers(MasterWindow::p_glfwWindow); // Try to swap buffers
		glfwPollEvents(); // Poll the window system for events both to provide input to the application and to tell the windows system that the application hasn't locked up
	}

	// Release
	Release();
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

void InitializeGame()
{
	// Initialize game instance
	GameInstance::GameObjectsRegistry = {};
	GameInstance::TimeInfo = {};

	// - Create objects -

	// Create tilemap
	gp_TestTilemap = new Tilemap::Tilemap(Vector2Int(1, 1));
	// Initialize tilemap
	gp_TestTilemap->Position = Vector2(-100, -100);
	gp_TestTilemap->TileSize = Vector2(1, 1);
	GameInstance::RegisterGameObject("MainTilemap", gp_TestTilemap);
	GenerateTilemap();

	GameInstance::p_DebugServiceObject = new DebugObject(); // Create Debug object
	GameInstance::RegisterGameObject("DebugService", GameInstance::p_DebugServiceObject);
	new Player(gp_TestTilemap); // Creates the only instance of the player for this device
	GameInstance::RegisterGameObject("MainPlayer", GameInstance::p_Player); // Create player object (player will create its mesh)

}
void InitializeGraphics()
{
	// Initialize camera
	int width, height;
	glfwGetWindowSize(MasterWindow::p_glfwWindow, &width, &height);
	MasterWindow::Pipeline.ActiveCamera.ScreenSize = SandboxEngine::Vector2Int(width, height);
	MasterWindow::Pipeline.ActiveCamera.Scale = SandboxEngine::Vector2(150, 150);
	MasterWindow::Pipeline.ActiveCamera.Origin = SandboxEngine::Vector2(0, 0);

	// Create layers
	MasterWindow::Pipeline.InsertLayer(RENDERLAYERS_Tilemap0, { "Tilemap0" });
	MasterWindow::Pipeline.InsertLayer(RENDERLAYERS_Objects, { "Objects" });
	MasterWindow::Pipeline.InsertLayer(RENDERLAYERS_Characters, { "Characters" });
	MasterWindow::Pipeline.InsertLayer(RENDERLAYERS_GUI, { "GUI" });
	MasterWindow::Pipeline.InsertLayer(RENDERLAYERS_Debug, { "Debug" });

	// TODO: Game specific shaders(those that are not basic and required for every graphics pipeline) should really be registered here
}
void InitializeGUI()
{
	//// Create button component
	//auto pButton = new GUISystem::Components::GUIButtonComponent();
	//// Subscribe event
	//pButton->ButtonEventHandler.SubscribeEvent(new TestButtonEventDelegate());

	//// Create element
	//GUISystem::GUIElement* pTestElement = new GUISystem::GUIElement(&MasterWindow::UserInterfaceSystem, GUISystem::GUIHierarchy::NULL_UID);
	//// Register componenets
	//pTestElement->RegisterComponent(new GUISystem::Components::GUISpriteComponent(0, std::string(GraphicsPipeline::GraphicsPipeline2D::PROJECT_DIRECTORY).append("Resources/GUI/Background.bmp").c_str()));
	//pTestElement->RegisterComponent(pButton);
	//// Set transform
	//pTestElement->SetTransform({ 0.1,0 }, {.5, .8}, true);
	//// Register element
	//MasterWindow::UserInterfaceSystem.p_Hierarchy->RegisterElement(pTestElement);

	//// Other operations
	//GUISystem::GUITransform globalCoord = pTestElement->GetTransform();
	//pTestElement->SetTransform(globalCoord.GlobalPosition, globalCoord.GlobalScale, false);
}

void RunGameLoopCycle()
{
	/*
	Vector2 tr = gp_TestTilemap->FromTileToWorld(gp_TestTilemap->Container.GetTileBounds()),
		bl = gp_TestTilemap->Position;
	GameInstance::p_DebugServiceObject->CreateRectangle(bl - Vector2(5, 5), bl + Vector2(5, 5), 1, GraphicsPipeline::GraphicsPipeline2D::GP2D_BASE_SHADER);
	GameInstance::p_DebugServiceObject->CreateRectangle(tr - Vector2(5, 5), tr + Vector2(5, 5), 1, GraphicsPipeline::GraphicsPipeline2D::GP2D_BASE_SHADER);
	GameInstance::p_DebugServiceObject->CreateRectangle(Vector2(60, -80), Vector2(110, -60), 3, GraphicsPipeline::GraphicsPipeline2D::GP2D_BASE_SHADER);*/


	/*if ((int)floor(GameInstance::TimeInfo.CurrentTime) % 2 == 0)
	{
		int s = 100;
		for (int i = 0; i < s; i++)
		{
			for (int j = 0; j < s; j++)
			{
				Vector2 diff = Vector2(s, s) / 2.0 - Vector2(i, j);
				if (diff.GetMagnitudeSqrd() > pow((double)s / 2.0, 2.0))
					continue;

				unsigned int col = i * j % 2 == 0 ? 0x0f0fffff : 0xffff00ff;
				gp_TestTilemap->AddTile(Vector2Int(i, j), Tilemap::Tile(col, Tilemap::TILE_BEHAVIOR_NAMES::Solid));
			}
		}

		int width = 120;
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				gp_TestTilemap->RemoveTile(Vector2Int(i + 20, j + 50));
			}
		}
	}*/
	//gp_TestTilemap->Container.RemoveTiles(gp_TestVecOfTileRemovals.begin(), gp_TestVecOfTileRemovals.end(), Vector2Int(20, 50));
}

void Release()
{
	GameInstance::Release();
	MasterWindow::Release();
}

void GenerateTilemap()
{
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			//(unsigned int)(i*j / 10000.0 * 272) << 8 & 0x0000ff00 | 0x000000ff
			unsigned int col = i * j % 2 == 0 ? 0x00ff00ff : 0x0000ffff;
			if (i + j * 50 * 50 == 50 * 50 - 1)
				col = 0xffff00ff;
			gp_TestTilemap->PendingTileChangesRegistry.SetTile(Vector2Int(i, j), Tilemap::Tile(col, Tilemap::TILE_BEHAVIOR_NAMES::Solid));
		}
	}

	//g_pTestTilemap->AddTile(Vector2Int(50 + cos(GameInstance::TimeInfo.CurrentTime * 2.0 + 1) * 50,
	//									50 + sin(GameInstance::TimeInfo.CurrentTime) * 50),
	//						Tilemap::TileActionQueue::AddTileActionArgument( true, Tilemap::Tile(0xf00000, Tilemap::TILE_BEHAVIOR_NAMES::Sand),  true, true));
	//g_pTestTilemap->AddTile(Vector2Int(50 + sin(GameInstance::TimeInfo.CurrentTime * 2.0 + 1) * 50,
	//									50 + cos(GameInstance::TimeInfo.CurrentTime*5.0) * 50),
	//						Tilemap::TileActionQueue::AddTileActionArgument(true, Tilemap::Tile(0xd0d000, Tilemap::TILE_BEHAVIOR_NAMES::Sand), true, true));


	/*g_pTestTilemap->AddTile(Vector2Int(100 - int(GameInstance::TimeInfo.CurrentTime * 10) % 100,
										50 + sin(GameInstance::TimeInfo.CurrentTime) * 50),
		Tilemap::TileActionQueue::AddTileActionArgument(true, Tilemap::Tile(0x00fddf, Tilemap::TILE_BEHAVIOR_NAMES::Sand), true, true));

	if (int(GameInstance::TimeInfo.CurrentTime) % 20 < 10)
	{
		for (unsigned int i = (sin(GameInstance::TimeInfo.CurrentTime) + 1) * 10; i < 50 + 10 * (sin(GameInstance::TimeInfo.CurrentTime) + 1); i++)
		{
			UINT color = (unsigned int)(i % 6 >= 3 ? 0xff0000 : 0x00ff00);
			if ((int)floor(TileXPosition) % 6 >= 3)
				color = 0x00ff00;
			int behavior = color == 0xff0000 ? Tilemap::TILE_BEHAVIOR_NAMES::Solid : Tilemap::TILE_BEHAVIOR_NAMES::Sand;

			g_pTestTilemap->AddTile(Vector2Int((int)floor(TileXPosition) % 60, i),
				Tilemap::TileActionQueue::AddTileActionArgument(true, Tilemap::Tile(color, behavior), true, true));
		}
		TileXPosition += 5 * GameInstance::TimeInfo.FrameDeltaTime;
	}

	g_pTestTilemap->RemoveTile(Vector2Int(100 - int(GameInstance::TimeInfo.CurrentTime * 50) % 100, int(GameInstance::TimeInfo.CurrentTime * .2) % 3));
	g_pTestTilemap->RemoveTile(Vector2Int(int(GameInstance::TimeInfo.CurrentTime * 50) % 100, int(GameInstance::TimeInfo.CurrentTime * .2) % 3));*/
}