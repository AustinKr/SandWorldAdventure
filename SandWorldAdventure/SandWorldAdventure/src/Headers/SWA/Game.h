#pragma once

// SWA engine
#include "SWAEngine/Tilemap/Tilemap.h"
#include "SWAEngine/Tilemap/TilemapMesh.h"

#include "SWA/Time.h"

namespace SWA
{
	// Static struct that defines game logic and related info
	struct Game
	{
	private:
		static void AddTextures();
		static void AddShaders();
		static void CreateRenderLayers();
		static void InitializeGame();

	public:
		static SWAEngine::Tilemap::Tilemap* gp_Tilemap; // TODO: Rename
		static SWAEngine::Tilemap::TilemapMesh* gp_TilemapMesh;

		static void CreateResources();
		static void Initialize();
		static void Update(Time time);
		static void Release();
	};
}