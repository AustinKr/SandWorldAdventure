// This file contains the entry point and initializes everything. It also contains game logic

#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/GraphicsPipeline/ShaderTypes/ShaderType.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/ShaderInformation.h"

#include <iostream>
#include <string>

#include "HeaderFiles/Game/Game.h"

//#include "KeyboardInterfaceAPI/Headers/KeyboardInterface/KeyboardState.h"

using namespace SandboxEngine;
using namespace SandboxEngine::Game;
using namespace SandboxEngine::Game::GameObject;

// - Forward declarations -
void InitializeGame();
void RunGameLoopCycle();
void Release();
// Game stuff
void GenerateTilemap();

// - Variables -
Player* pPlayer = nullptr;
Tilemap::Tilemap* gp_TestTilemap;
double TileXPosition = 0;

Tilemap::Tile* gp_TestArrayOfTiles;
std::vector<std::pair<Vector2Int, Tilemap::Tile>> g_TestVecOfTiles;

int main(void)
{
	// Creates a window and nearly everything else necessary relating to openGL
	MasterWindow::InitializeWindow();

	// Initialize Game
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
	pPlayer = new Player(gp_TestTilemap);
	GameInstance::RegisterGameObject("MainPlayer", pPlayer); // Create player object (player will create its mesh)

}

void RunGameLoopCycle()
{
	// Set color of default shader
	GraphicsPipeline::ShaderTypes::ShaderType* pShader = MasterWindow::Pipeline.TryGetShader<GraphicsPipeline::ShaderTypes::ShaderType>(GraphicsPipeline::GraphicsPipeline2D::GP2D_BASE_SHADER);
	GraphicsPipeline::Shaders::ShaderInformation* shaderInfo = ((GraphicsPipeline::Shaders::ShaderInformation*)pShader->p_ShaderInformation);
	shaderInfo->SetShadeColor(&MasterWindow::Pipeline, float3(1, 0, 1));
	/*
	Vector2 tr = gp_TestTilemap->FromTileToWorld(gp_TestTilemap->Container.GetTileBounds()),
		bl = gp_TestTilemap->Position;
	GameInstance::p_DebugServiceObject->CreateRectangle(bl - Vector2(5, 5), bl + Vector2(5, 5), 1, GraphicsPipeline::GraphicsPipeline2D::GP2D_BASE_SHADER);
	GameInstance::p_DebugServiceObject->CreateRectangle(tr - Vector2(5, 5), tr + Vector2(5, 5), 1, GraphicsPipeline::GraphicsPipeline2D::GP2D_BASE_SHADER);
	GameInstance::p_DebugServiceObject->CreateRectangle(Vector2(60, -80), Vector2(110, -60), 3, GraphicsPipeline::GraphicsPipeline2D::GP2D_BASE_SHADER);*/
}

void Release()
{
	GameInstance::Release();
	MasterWindow::Release();
}

void GenerateTilemap()
{
	// temp-
	int width = 300;
	gp_TestArrayOfTiles = (Tilemap::Tile*)malloc(width * 500 * sizeof(Tilemap::Tile));
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 500; j++)
		{
			unsigned int col = i * j % 2 == 0 ? 0x00ff00ff : 0x0000ffff;
			gp_TestArrayOfTiles[i + j * width] = {col, Tilemap::TILE_BEHAVIOR_NAMES::Solid};
		}
	}
	g_TestVecOfTiles = {};
	int s = 200;
	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			Vector2 diff = Vector2(s, s) / 2.0 - Vector2(i, j);
			if (diff.GetMagnitudeSqrd() > pow((double)s / 2.0, 2.0))
				continue;

			unsigned int col = i * j % 2 == 0 ? 0x0f0fffff : 0xffff00ff;
			g_TestVecOfTiles.push_back(std::make_pair(Vector2Int(i, j), Tilemap::Tile(col, Tilemap::TILE_BEHAVIOR_NAMES::Solid)));
		}
	}

	gp_TestTilemap->Container.AddBlockOfTiles(gp_TestArrayOfTiles, Vector2Int(50, 50), Vector2Int(300, 500), GameInstance::TimeInfo.CurrentTime);
	gp_TestTilemap->Container.AddTiles(g_TestVecOfTiles.begin(), g_TestVecOfTiles.end(), Vector2Int(200, 200), GameInstance::TimeInfo.CurrentTime);

	// -

	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			//(unsigned int)(i*j / 10000.0 * 272) << 8 & 0x0000ff00 | 0x000000ff
			unsigned int col = i * j % 2 == 0 ? 0x00ff00ff : 0x0000ffff;
			if (i + j * 50 * 50 == 50 * 50 - 1)
				col = 0xffff00ff;
			gp_TestTilemap->SetTile(Vector2Int(i, j), Tilemap::Tile(col, Tilemap::TILE_BEHAVIOR_NAMES::Solid));
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








#ifdef HELLO_IDGIT

/*
// Forward declarations for functions

void GenerateTilemap(Time time);
// Called even before the window is created to create objects
void Initialize();
void Release();
void OnWindowResize(OnResizeGameEventArguments args);
void GetTimeInfo(LARGE_INTEGER* pFrameStartTime, LARGE_INTEGER* pFrameEndTime, LARGE_INTEGER* pCounterFrequency, LARGE_INTEGER* pInitialTime, Time* pInfo, double* pDeltaTime);

//std::vector<GameDelegate<OnResizeGameEventArguments>>::const_iterator OnWindowResizeEventIterator;

MasterWindow g_WndInst;

// Variables
Tilemap::Tilemap* g_pTestTilemap;
double TileXPosition = 0;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	AllocConsole();

	Initialize();

	// Register wnd class
	g_WndInst.Register(&hInstance);
	// Create window inst
	g_WndInst.HWnd = CreateWindow(g_WndInst.ClassName, L"Sand World Adventure", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, NULL);
	if (g_WndInst.HWnd == NULL)
		return 0;

	UpdateWindow(g_WndInst.HWnd);
	ShowWindow(g_WndInst.HWnd, nCmdShow);
	// Window is created and shown

	// Start timer
	Time timeInfo = {};
	double deltaTime = 0;
	LARGE_INTEGER frameEndTime{};
	LARGE_INTEGER counterFrequency{};
	LARGE_INTEGER initialTime{};
	QueryPerformanceCounter(&initialTime);
	LARGE_INTEGER frameStartTime = initialTime;

	HDC hdc = GetDC(g_WndInst.HWnd);
	ScreenState* pMainScreen = &GameInstance::MainCamera.MainScreen;
	while (g_WndInst.RunMessageLoop())
	{
		// Get the current and delta times
		GetTimeInfo(&frameStartTime, &frameEndTime, &counterFrequency, &initialTime, &timeInfo, &deltaTime);
		GameInstance::TimeInfo = timeInfo;

		if (pMainScreen->BitmapInfo.bmiHeader.biWidth <= 0 || pMainScreen->BitmapInfo.bmiHeader.biHeight <= 0)
			continue;

		// Debug background
		IGameObject* p_debugService = GameInstance::Layers[0].Objects["DebugService"];
		
		// Screen background
		Renderer::Background(pMainScreen, 0x0505ff);
		if((int)std::floor(timeInfo.CurrentTime) % 3 == 0)
			GameInstance::MainCamera.DrawRect(pMainScreen, g_pTestTilemap->Position.X, g_pTestTilemap->Position.Y, g_pTestTilemap->TileSize.X * g_pTestTilemap->Container.GetTileBounds().X, g_pTestTilemap->TileSize.Y * g_pTestTilemap->Container.GetTileBounds().Y, 0xffff00);


		//GenerateTilemap(timeInfo);

		// Get cursor position
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);
		ScreenToClient(g_WndInst.HWnd, &cursorPosition);
		Vector2 mouseWorldPosition = GameInstance::MainCamera.FromViewportToWorld(
											GameInstance::MainCamera.FromScreenToViewport(
													Vector2(cursorPosition.x, pMainScreen->BitmapInfo.bmiHeader.biHeight - cursorPosition.y)));
		
		// Cursor remove tiles
		int isMouseDown = KeyboardInterface::KeyboardState::IsButtonDown(VK_LBUTTON) ? 1 : (KeyboardInterface::KeyboardState::IsButtonDown(VK_RBUTTON) ? 2 : 0);
		if (isMouseDown != 0)
		{
			Vector2 mousePosition = g_pTestTilemap->FromWorldToTile(mouseWorldPosition);
			double radius = 5;
			for (int i = -radius; i < radius; i++)
			{
				for (int j = -radius; j < radius; j++)
				{
					if (i * i + j * j >= radius * radius)
						continue;
					if (isMouseDown == 1)
						g_pTestTilemap->AddTile(Vector2Int(i, j) + mousePosition, Tilemap::TileActionQueue::AddTileActionArgument(true, Tilemap::Tile((i * j) * 50, Tilemap::TILE_BEHAVIOR_NAMES::Sand), false, true));
					else
						g_pTestTilemap->RemoveTile(Vector2Int(i + mousePosition.X, j + mousePosition.Y));
				}
			}
		}
		
		//GameInstance::MainCamera.Position = Vector2(50,50) / mouseWorldPosition;

		GameInstance::UpdateObjects();
		GameInstance::CompositeLayers();

		// Copy buffer to screen
		StretchDIBits(hdc,
			0, 0, pMainScreen->BitmapInfo.bmiHeader.biWidth, pMainScreen->BitmapInfo.bmiHeader.biHeight,
			0, 0, pMainScreen->BitmapInfo.bmiHeader.biWidth, pMainScreen->BitmapInfo.bmiHeader.biHeight,
			pMainScreen->Data, &pMainScreen->BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	// Release objects
	DeleteDC(hdc);
	FreeConsole();
	Release();

	return 0;
}

void Initialize()
{
	// Initialize game instance
	GameInstance::Layers = {};
	GameInstance::MainCamera = Camera(Vector2(0,0), Vector2(100,100), Vector2(1,1));
	GameInstance::TimeInfo = {};

	// - Subscribe to events -
	g_WndInst.OnResizeEvent.Subscribe(GameDelegate(OnWindowResize));


	// - Create objects -

	// Create Debug object
	GameInstance::RegisterGameObject(0, "Debug", new DebugObject(), "DebugService");

	// Create tilemap
	g_pTestTilemap = new Tilemap::Tilemap(Vector2Int(1, 1));
	// Initialize tilemap
	g_pTestTilemap->Position = Vector2(-50, -40);
	g_pTestTilemap->TileSize = Vector2(1, 1);
	GameInstance::RegisterGameObject(1, "Tilemap0", g_pTestTilemap, "MainTilemap");

	// Create player object
	GameInstance::RegisterGameObject(2, "Characters", new Player(), "MainPlayer");



	for (int i = 20; i < 80; i++)
	{
		for (int j = 20; j < 40; j++)
		{
			g_pTestTilemap->AddTile(Vector2Int(i, j), Tilemap::TileActionQueue::AddTileActionArgument(true, Tilemap::Tile((i * j) * 50, Tilemap::TILE_BEHAVIOR_NAMES::Sand), false, true));
		}
	}

	//TilemapRenderer::Resize(&TestTilemap);
}

void Release()
{
	GameInstance::Release();

	//g_pTestTilemap->Release();

	//WndInst.OnResizeEvent.Unsubscribe(OnWindowResizeEventIterator); // shouldnt be needed with the current design
}

void GenerateTilemap(Time time)
{
	//g_pTestTilemap->AddTile(50 + cos(time.CurrentTime * 2.0 + 1) * 50,
	//						50 + sin(time.CurrentTime) * 50,
	//						time,
	//						Tilemap::TileActionQueue::AddTileActionArgument( true, Tilemap::Tile(0xf00000, TILE_BEHAVIOR_NAMES::Sand),  true, true));
	//g_pTestTilemap->AddTile(50 + sin(time.CurrentTime * 2.0 + 1) * 50,
	//						50 + cos(time.CurrentTime*5.0) * 50,
	//						time,
	//						Tilemap::TileActionQueue::AddTileActionArgument(true, Tilemap::Tile(0xd0d000, TILE_BEHAVIOR_NAMES::Sand), true, true));
	

	g_pTestTilemap->AddTile(Vector2Int(100 - int(time.CurrentTime * 10) % 100,
		50 + sin(time.CurrentTime) * 50),
		Tilemap::TileActionQueue::AddTileActionArgument(true, Tilemap::Tile(0x00fddf, Tilemap::TILE_BEHAVIOR_NAMES::Sand), true, true));

	if (int(time.CurrentTime) % 20 < 10)
	{
		for (unsigned int i = (sin(time.CurrentTime) + 1) * 10; i < 50 + 10 * (sin(time.CurrentTime) + 1); i++)
		{
			UINT color = (unsigned int)(i % 6 >= 3 ? 0xff0000 : 0x00ff00);
			if ((int)floor(TileXPosition) % 6 >= 3)
				color = 0x00ff00;
			int behavior = color == 0xff0000 ? Tilemap::TILE_BEHAVIOR_NAMES::Solid : Tilemap::TILE_BEHAVIOR_NAMES::Sand;

			g_pTestTilemap->AddTile(Vector2Int((int)floor(TileXPosition) % 60, i),
				Tilemap::TileActionQueue::AddTileActionArgument(true, Tilemap::Tile(color, behavior), true, true));
		}
		TileXPosition += 5 * time.FrameDeltaTime;
	}

	g_pTestTilemap->RemoveTile(Vector2Int(100 - int(time.CurrentTime * 50) % 100, int(time.CurrentTime * .2) % 3));
	g_pTestTilemap->RemoveTile(Vector2Int(int(time.CurrentTime * 50) % 100, int(time.CurrentTime * .2) % 3));
}

void OnWindowResize(OnResizeGameEventArguments args)
{
	//TODO: Call tilemap function that resizes
	// Resize tilemap renderer
	g_pTestTilemap->RenderInformation.ShouldResize = true;

	IGameObject* p_debugService = GameInstance::Layers[0].Objects["DebugService"];
	if (p_debugService != nullptr)
		((GameObject::DebugObject*)p_debugService)->DebugScreen.Resize(args.Width, args.Height);
}

void GetTimeInfo(LARGE_INTEGER* pFrameStartTime, LARGE_INTEGER* pFrameEndTime, LARGE_INTEGER* pCounterFrequency, LARGE_INTEGER* pInitialTime, Time* pInfo, double* pDeltaTime)
{
	// Get the time
	QueryPerformanceFrequency(pCounterFrequency); // Ticks per second
	QueryPerformanceCounter(pFrameEndTime);
	// Delta time
	*pDeltaTime = (double)(pFrameEndTime->QuadPart - pFrameStartTime->QuadPart) / (double)pCounterFrequency->QuadPart;
	*pFrameStartTime = *pFrameEndTime;

	// Set time struct
	pInfo->CurrentTime = (pFrameEndTime->QuadPart - pInitialTime->QuadPart) / (double)pCounterFrequency->QuadPart;
	pInfo->FrameDeltaTime = *pDeltaTime;
}*/
#endif