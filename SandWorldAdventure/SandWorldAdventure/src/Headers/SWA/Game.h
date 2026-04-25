#pragma once

#include <SWAEngine/Component/Physics/TilemapCollider.h>
#include <SWAEngine/Component/Tilemap/Tilemap.h>
#include <SWAEngine/Component/Tilemap/TilemapMesh.h>
#include <SWAEngine/Math/Time.h>

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
		// TODO: Rename to TARGET_FPS
		static const unsigned int FPS; // TODO: Can be configured by user in future

		static Player::Player* p_MainPlayer;
		static SWAEngine::Component::Tilemap::Tilemap* p_Tilemap;
		static SWAEngine::Component::Physics::TilemapCollider* p_TilemapCollider;
		static SWAEngine::Component::Tilemap::TilemapMesh* p_TilemapMesh;

		static void CreateResources();
		static void Initialize();
		static void Update(SWAEngine::Math::Time time);
		static void Release();
	};
}