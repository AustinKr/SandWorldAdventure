#pragma once
#include "TileBehavior.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class SandTileBehavior : private TileBehavior
	{
	public:
		virtual bool TryUpdate(
			Tilemap* pTilemap,
			Vector2Int tilePosition,
			Tile* pTile,
			Chunk* pChunk,
			TileActionQueue::TileActionQueue::ACTION_QUEUE* pTileBehaviorActionQueue,
			TileActionQueue::BaseTileActionArgument tileActionArguments,
			Time time) override;
	};
}

// GOOD FOR NEW IMPLEMENTATION