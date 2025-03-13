#pragma once
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Time.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"
#include <unordered_map>

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class TileBehavior
	{
	public:
		static const TileBehavior* TILE_BEHAVIORS[2];
		
		static double TryStepMoveTile(
			Tilemap* pTilemap,
			Tile* pTile, 
			Vector2 tilePosition, 
			Vector2 direction, 
			double end,
			Time time,
			TileActionQueue::TileActionQueue::ACTION_QUEUE* pActionQueue);

		static TileBehavior* GetTileBehavior(int behaviorIndex);
		static void ReleaseTileBehaviors();

		// - Abstract -
		

		/// <summary>
		/// Apply custom physics and responsible for redrawing self. Called even when  tile.hasValue == false.
		/// By default: only returns immediate adjacent tiles if the action is remove or add
		/// </summary>
		/// <param name="pTilemap"></param>
		/// <param name="tilePosition"></param>
		/// <param name="pTile"></param>
		/// <param name="chunkPosition"></param>
		/// <param name="pChunk"></param>
		/// <param name="pTileBehaviorActionQueue">The action queue, reserved for tile behaviors to register actions, that will replace the current action queue</param>
		/// <param name="pTileAction">The action being applied on this tile</param>
		/// <param name="time">Used to create physics and such</param>
		/// <returns>Whether to also update the tiles adjacent to this one because of this tile being updated(used in UpdateTilesFromActionQueue)</returns>
		virtual bool TryUpdate(
			Tilemap* pTilemap,
			Vector2Int tilePosition,
			Tile* pTile,
			Chunk* pChunk,
			TileActionQueue::TileActionQueue::ACTION_QUEUE* pTileBehaviorActionQueue,
			TileActionQueue::BaseTileActionArgument tileActionArguments,
			Time time) = 0;
	};
}

// GOOD FOR NEW IMPLEMENTATION