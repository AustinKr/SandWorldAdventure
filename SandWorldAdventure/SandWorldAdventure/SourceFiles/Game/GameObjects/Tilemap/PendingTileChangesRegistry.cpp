#include "HeaderFiles/Game/GameObjects/Tilemap/PendingTileChangesRegistry.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"

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
		if (!m_PendingTileChanges.insert(std::make_pair(tilePosition, PendingTileChange(newTile))).second) // try insert
			m_PendingTileChanges[tilePosition] = { newTile }; // override
		return true;
	}
	void PendingTileChangesRegistry::SwapTiles(Vector2Int tileAPosition, Vector2Int tileBPosition)
	{
		// TODO: Get fast way to retrieve tiles

		if (tileAPosition == tileBPosition)
			return;

		Tile tileACopy = {}, tileBCopy = {};
		// Tile a
		if (WillTileChange(tileAPosition))
			tileACopy = m_PendingTileChanges[tileAPosition].GetNewTile();
		else
		{
			Tile* pTileA = ((Tilemap*)mp_Tilemap)->Container.GetTileInChunk(tileAPosition).second;
			if (pTileA != nullptr)
				tileACopy = *pTileA;
		}
		// Tile B
		if (WillTileChange(tileBPosition))
			tileBCopy = m_PendingTileChanges[tileBPosition].GetNewTile();
		else
		{
			Tile* pTileB = ((Tilemap*)mp_Tilemap)->Container.GetTileInChunk(tileBPosition).second;
			if (pTileB != nullptr)
				tileBCopy = *pTileB;
		}

		// Swap
		SetTile(tileAPosition, tileBCopy);
		SetTile(tileBPosition, tileACopy);
	}
	bool PendingTileChangesRegistry::WillAddTile(Vector2Int tilePosition)
	{
		if (!WillTileChange(tilePosition))
			return false;

		return m_PendingTileChanges[tilePosition].GetNewTile().HasValue;
	}
	bool PendingTileChangesRegistry::WillRemoveTile(Vector2Int tilePosition)
	{
		if (!WillTileChange(tilePosition))
			return false;

		return !m_PendingTileChanges[tilePosition].GetNewTile().HasValue;
	}
	bool PendingTileChangesRegistry::WillTileChange(Vector2Int tilePosition)
	{
		return m_PendingTileChanges.contains(tilePosition) && !m_PendingTileChanges[tilePosition].GetIsRefreshOnly();
	}

	void PendingTileChangesRegistry::UpdateRegistry(Time time)
	{
		/*
		. Apply Pending Tile changes // (and update tiles as we go)
		. Iterate through all the tiles and those adjacent that are affected by the changes

		TODO: Make fast way to retrieve tiles (use in Tilemap::Swap, SaneTileBehavior, etc..)
		TODO: Debug applyTileChanges to make adjacent tiles that are affected move immediately or at least the frame after carry on
		*/

		fprintf(stdout, std::to_string(m_PendingTileChanges.size()).append("\n").c_str());
		Tilemap *pTilemap = ((Tilemap*)mp_Tilemap);

		std::set<Vector2Int, Vector2Compare> tilesToUpdateSet = {};
		std::vector<Vector2Int> tilesToUpdateList = {};
		
		ApplyTileChanges(time, &tilesToUpdateSet, &tilesToUpdateList);
		UpdateAdjacentTiles(time, &tilesToUpdateSet, &tilesToUpdateList);

		fprintf(stdout, std::to_string(tilesToUpdateSet.size()).append("\n\n").c_str());
	}

	bool PendingTileChangesRegistry::UpdateTile(Time time, TilemapContainer::TILE_INFO info, Vector2 position)
	{
		return TileBehavior::GetTileBehavior(info.second->BehaviorIndex)->TryUpdate((Tilemap*)mp_Tilemap, position, info, time);
	}

	// Processes all the pending tile changes by adding, removing, or simply refreshing each given tile. (Updates most of the necessary changed tiles as it goes and outputs the adjacent ones)
	void PendingTileChangesRegistry::ApplyTileChanges(Time time, std::set<Vector2Int, Vector2Compare> *pOutTilesSet, std::vector<Vector2Int> *pOutTilesList)
	{
		Tilemap* pTilemap = ((Tilemap*)mp_Tilemap);

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
				
					// TODO: Not really necessary in the future when it is faster to check set tiles than pending changes
					if (pendingTiles.contains(currentAdjPosition) && !pendingTiles[currentAdjPosition].GetIsRefreshOnly() && !pendingTiles[currentAdjPosition].GetNewTile().HasValue) // if will remove
						continue;

					if (pOutTilesSet->insert(currentAdjPosition).second)
						pOutTilesList->push_back(currentAdjPosition);
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

			// Try update
			if(pOutTilesSet->insert(pair.first).second)
				UpdateTile(time, info, pair.first);
		}
	}
	void PendingTileChangesRegistry::UpdateAdjacentTiles(Time time, std::set<Vector2Int, Vector2Compare>* pTilesSet, std::vector<Vector2Int>* pTilesList)
	{
		Tilemap* pTilemap = ((Tilemap*)mp_Tilemap);

		// Update tiles that result from the previous changes
		Vector2Int currentPosition; TilemapContainer::TILE_INFO currentTile;
		for (int i = 0; i < pTilesList->size(); i++)
		{
			if (pTilesList->size() > Tilemap::MAX_TILE_UPDATES)
			{
				fprintf(stdout, "Hit max tile updates limit!\n");
				return;
			}

			currentPosition = pTilesList->at(i);
			currentTile = pTilemap->Container.GetTileInChunk(currentPosition);
			// Update tile
			if (currentTile.second == nullptr || !currentTile.second->HasValue || !UpdateTile(time, currentTile, currentPosition))
				continue;

			// Append neighbors
			for (int ii = 0; ii < 8; ii++)
			{
				Vector2Int currentAdjPosition = currentPosition + Tilemap::ADJACENT_TILE_POSITIONS[ii];
				if (pTilesSet->insert(currentAdjPosition).second)
					pTilesList->push_back(currentAdjPosition);
			}
		}
	}
}