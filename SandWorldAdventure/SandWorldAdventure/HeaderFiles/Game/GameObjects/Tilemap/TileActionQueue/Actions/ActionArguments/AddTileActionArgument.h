#pragma once
#include "HeaderFiles/Game/GameObjects/Tilemap/Tile.h"
#include "BaseTileActionArgument.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	/// <summary>
	/// A structure that contains the member variables of AddTileAction to be passed as arguments
	/// </summary>
	struct AddTileActionArgument : public BaseTileActionArgument
	{
	public:
		// The new tile to use
		Tile NewTile;
		// Should this tile be add regardless of any existing tile? If true and there is a tile, this will immediately be removed from the queue
		bool ShouldOverrideExisting;
		// If the tile is out of bounds, should it try to create more space?
		bool ShouldExpandBounds;

		inline AddTileActionArgument() : BaseTileActionArgument()
		{
			NewTile = {};
			ShouldOverrideExisting = false;
			ShouldExpandBounds = false;
		}
		inline AddTileActionArgument(bool shouldUpdateAdjacent, Tile newTile, bool shouldOverrideExisting, bool shouldExpandBounds) : BaseTileActionArgument(shouldUpdateAdjacent)
		{
			NewTile = newTile;
			ShouldOverrideExisting = shouldOverrideExisting;
			ShouldExpandBounds = shouldExpandBounds;
		}
	};
}