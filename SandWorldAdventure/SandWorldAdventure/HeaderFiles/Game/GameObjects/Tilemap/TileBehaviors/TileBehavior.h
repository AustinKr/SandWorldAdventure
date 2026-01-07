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
		/// Apply custom physics. Not called after a tile is removed- only when .HasValue.
		/// By default, returns false
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