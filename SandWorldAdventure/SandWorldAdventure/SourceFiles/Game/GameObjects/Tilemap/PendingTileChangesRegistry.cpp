#include "HeaderFiles/Game/GameObjects/Tilemap/PendingTileChangesRegistry.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"
#include <set>

namespace SandboxEngine::Game::GameObject::Tilemap
{
	PendingTileChangesRegistry::PendingTileChangesRegistry(IGameObject* pTilemap)
	{
		mp_Tilemap = pTilemap;
	}

	void PendingTileChangesRegistry::TryRefreshTile(Vector2Int tilePosition)
	{
		m_PendingTileChanges.insert(std::make_pair(tilePosition, PendingTileChange{}));
	}
	bool PendingTileChangesRegistry::SetTile(Vector2Int tilePosition, Tile newTile)
	{
		if (tilePosition.X < 0 || tilePosition.Y < 0 ||
			(!newTile.HasValue && !((Tilemap*)mp_Tilemap)->Container.IsTileInBounds(tilePosition)))
			return false;
		if (!m_PendingTileChanges.insert(std::make_pair(tilePosition, PendingTileChange(newTile))).second)
			m_PendingTileChanges[tilePosition] = { newTile };
		return true;
	}
	//TODO: References are made to this with Container.ContainsTile(tilePosition) || included in the old function
	// Checks whether the pending new tiles will add/override a tile
	bool PendingTileChangesRegistry::WillContainTile(Vector2Int tilePosition)
	{
		return  (m_PendingTileChanges.contains(tilePosition) && m_PendingTileChanges[tilePosition].GetNewTile().HasValue);// || m_PendingTileAdditions.contains(tilePosition);
	}
	
	void PendingTileChangesRegistry::UpdateRegistry(Time time)
	{
		/*
		. Apply Pending Tile changes // (and update tiles as we go)
		. Iterate through all the tiles and those adjacent that are affected by the changes

		*/
		// TODO: Split into multiple functions

		Tilemap *pTilemap = ((Tilemap*)mp_Tilemap);

		std::set<Vector2Int> tilesToUpdateSet = {};
		std::vector<Vector2Int> tilesToUpdateList = {};
		
		COLLECTION pendingTiles = std::move(m_PendingTileChanges);
		m_PendingTileChanges = {};

		// Set tiles
		for (auto& pair : pendingTiles)
		{
			TilemapContainer::TILE_INFO info;
			if (!pair.second.GetIsRefreshOnly())
			{
				// Append neighbors
				for (int ii = 0; ii < 8; ii++)
				{
					Vector2Int currentAdjPosition = pair.first + Tilemap::ADJACENT_TILE_POSITIONS[ii];
					if (tilesToUpdateSet.insert(currentAdjPosition).second)
						tilesToUpdateList.push_back(currentAdjPosition);
				}

				// Try remove
				if (!pair.second.GetNewTile().HasValue)
				{
					pTilemap->Container.RemoveTile(pair.first);
					continue;
				}
				// Else add
				info = pTilemap->Container.AddTile(pair.first, pair.second.GetNewTile(), time.CurrentTime);
			}
			else
				info = pTilemap->Container.GetTileInChunk(pair.first);

			// Update
			TileBehavior::GetTileBehavior(pair.second.GetNewTile().BehaviorIndex)->TryUpdate(pTilemap, pair.first, info, time);
			tilesToUpdateSet.insert(pair.first);
		}

		// Update tiles that result from the previous changes
		Vector2Int currentPosition; TilemapContainer::TILE_INFO currentTile;
		int maxAdditionalUpdates = Tilemap::MAX_TILE_UPDATES - tilesToUpdateList.size();
		for (int i = 0; i < tilesToUpdateList.size(); i++)
		{
			currentPosition = tilesToUpdateList.at(i);
			currentTile = pTilemap->Container.GetTileInChunk(currentPosition);
			// Update tile
			if (currentTile.second == nullptr || !TileBehavior::GetTileBehavior(currentTile.second->BehaviorIndex)->TryUpdate(pTilemap, currentPosition, currentTile, time))
				continue;

			// Append neighbors
			for (int ii = 0; ii < min(8, maxAdditionalUpdates); ii++)
			{
				Vector2Int currentAdjPosition = currentPosition + Tilemap::ADJACENT_TILE_POSITIONS[ii];
				if (tilesToUpdateSet.insert(currentAdjPosition).second)
					tilesToUpdateList.push_back(currentAdjPosition);
			}

		}
	}
}