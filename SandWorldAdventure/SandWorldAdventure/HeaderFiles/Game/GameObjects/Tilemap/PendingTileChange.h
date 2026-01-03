#pragma once
#include "HeaderFiles/Game/GameObjects/Tilemap/Tile.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	struct PendingTileChange
	{
	private:
		Tile _newTile;
		bool _isRefreshOnly;
	public:
		// Returns a copy of the tile
		Tile GetNewTile() const { return _newTile; }
		// Returns a copy of the boolean
		bool GetIsRefreshOnly() const { return _isRefreshOnly; }

		// Create a tile change that may add/set or remove a tile
		PendingTileChange(Tile newTile)
		{
			_newTile = newTile;
			_isRefreshOnly = false;
		}
		// Creates refresh only
		PendingTileChange()
		{
			_newTile = {};
			_isRefreshOnly = true;
		}
	};
}