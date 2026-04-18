#pragma once

#include "SWAEngine/Tilemap/Tilemap.h"
#include "SWAEngine/Tilemap/TilemapMesh.h"
#include "SWAEngine/Time.h"

#include "SWA/Player/Player.h"

namespace SWA
{
	// Static struct that defines game logic and related info
	struct Game
	{
	private:
		static void CreateRenderLayers();
		static void InitializeGame();

	public:
		static const unsigned int FPS; // TODO: Can be configured by user in future

		static Player::Player* p_MainPlayer;
		static SWAEngine::Tilemap::Tilemap* p_Tilemap;
		static SWAEngine::Tilemap::TilemapMesh* p_TilemapMesh;

		static void CreateResources();
		static void Initialize();
		static void Update(SWAEngine::Time time);
		static void Release();
	};
}