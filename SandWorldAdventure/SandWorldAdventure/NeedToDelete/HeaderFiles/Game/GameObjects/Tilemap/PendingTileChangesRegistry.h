#pragma once
#include "HeaderFiles/Game/GameObjects/IGameObject.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TilemapContainer.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/PendingTileChange.h"
#include "HeaderFiles/Time.h"
#include <unordered_map>
#include <set>

namespace SandboxEngine::Game::GameObject::Tilemap
{
	struct PendingTileChangesRegistry
	{
	public:
		typedef std::unordered_map<Vector2Int, PendingTileChange, Vector2Hasher> COLLECTION;

		PendingTileChangesRegistry(IGameObject* pTilemap);

		void TryRefreshTile(Vector2Int tilePosition);
		bool SetTile(Vector2Int tilePosition, Tile newTile);
		void SwapTiles(Vector2Int tileA, Vector2Int tileB);
		// Returns true if a tile will be added, or false if that change doesn't exist
		bool WillAddTile(Vector2Int tilePosition);
		// Returns true if a tile will be removed, or false if that change doesn't exist
		bool WillRemoveTile(Vector2Int tilePosition);
		// Returns true if there is a tile change(add or remove)
		bool WillTileChange(Vector2Int tilePosition);

		void UpdateRegistry(Time time);
	private:
		IGameObject* mp_Tilemap;
		// The tiles that will override any of the existing tilemap in their places
		COLLECTION m_PendingTileChanges;

		bool UpdateTile(Time time, TilemapContainer::TILE_INFO info, Vector2 position);

		// Iterate through pending changes and either add, remove, or refresh the given tile
		void ApplyTileChanges(Time time, std::set<Vector2Int, Vector2Compare>* pOutTilesSet, std::vector<Vector2Int>* pOutTilesList);
		// Updates adjacent tiles originating from the given set
		void UpdateAdjacentTiles(Time time, std::set<Vector2Int, Vector2Compare>* pTilesSet, std::vector<Vector2Int>* pTilesList);
	};
}