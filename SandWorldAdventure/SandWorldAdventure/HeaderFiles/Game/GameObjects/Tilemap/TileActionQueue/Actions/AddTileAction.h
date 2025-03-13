#pragma once
#include "BaseTileAction.h"
#include "ActionArguments/AddTileActionArgument.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	class AddTileAction : public BaseTileAction
	{
	public:
		// The new tile to use
		Tile NewTile;
		// Should this tile be registered regardless of any existing tile or add action? If false and there is a tile, this will not be added to the queue
		bool ShouldOverrideExisting;
		// If the tile is out of bounds, should it try to create more space?
		bool ShouldExpandBounds;

		AddTileAction(AddTileActionArgument arguments = {});

		virtual bool Register(Tilemap* pTilemap, Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue) override;
		virtual void Apply(Tilemap* pTilemap, Vector2Int tilePosition, Time time, TileActionQueue::ACTION_QUEUE* pCurrentActionQueue) override;
		virtual void Release() override;
	};
}

// GOOD FOR NEW IMPLEMENTATION