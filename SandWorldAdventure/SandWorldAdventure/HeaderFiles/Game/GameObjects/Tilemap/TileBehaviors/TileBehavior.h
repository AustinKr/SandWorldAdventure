#pragma once
#include "HeaderFiles/Time.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include <unordered_map>

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class TileBehavior
	{
	public:
		static const TileBehavior* TILE_BEHAVIORS[2];
		
		static TileBehavior* GetTileBehavior(int behaviorIndex);
		static void ReleaseTileBehaviors();

		// - Abstract -
		

		/// <summary>
		/// Apply custom physics. Called even after a tile is removed. Make certain you check whether the tile exists before applying any behavior!
		/// By default, returns true if the action is remove or add
		/// </summary>
		/// <param name="pTilemap"></param>
		/// <param name="tilePosition"></param>
		/// <param name="tileInfo"></param>
		/// <param name="time">Used to create physics and such</param>
		virtual bool TryUpdate(
			Tilemap* pTilemap,
			Vector2Int tilePosition,
			TilemapContainer::TILE_INFO tileInfo,
			Time time) = 0;
	};
}