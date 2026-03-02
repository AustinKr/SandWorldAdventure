#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/RenderLayerNames.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{

	const int Tilemap::MAX_RAYCAST_STEPS = 600;
	const int Tilemap::MAX_TILE_UPDATES = 2000;
	const Vector2Int Tilemap::ADJACENT_TILE_POSITIONS[8] =
	{
		Vector2Int(-1,-1),
		Vector2Int(0,-1),
		Vector2Int(1,-1),

		Vector2Int(-1,1),
		Vector2Int(0,1),
		Vector2Int(1,1),

		Vector2Int(-1,0),
		Vector2Int(1,0),
	};

	Tilemap::Tilemap() : PendingTileChangesRegistry{this}, Container{}
	{
		p_Mesh = new GraphicsPipeline::TilemapMesh(this);
		MasterWindow::Pipeline.GetLayer(RenderLayerNames::RENDERLAYERS_Tilemap0).RegisterMesh(p_Mesh, 0);

		Container.AssignChunks(Vector2Int());
		Position = Vector2(0, 0);
		TileSize = Vector2(0, 0);
	}
	Tilemap::Tilemap(Vector2Int chunkBounds) : PendingTileChangesRegistry{this}, Container{}
	{
		p_Mesh = new GraphicsPipeline::TilemapMesh(this);
		MasterWindow::Pipeline.GetLayer(RenderLayerNames::RENDERLAYERS_Tilemap0).RegisterMesh(p_Mesh, 0);

		Container.AssignChunks(chunkBounds);
		Position = Vector2(0, 0);
		TileSize = Vector2(0, 0);
	}

	// Inherited via IGameObject
	void Tilemap::Update(Time time)
	{
		/*
		. Apply Pending Tile changes // (and update tiles as we go)
		. Iterate through all the tiles and those adjacent that are affected by the changes

		*/

		PendingTileChangesRegistry.UpdateRegistry(time);


		/*
		std::unordered_map<
			Vector2Int, // Position
			unsigned int, // Action
			Vector2Hasher> tilesToUpdateDict = {};


		auto OnTileChange = [&](TilemapContainer::TILE_INFO tileInfo, Vector2Int tilePosition, Time timeInfo, unsigned int action)
		{
			tilesToUpdateDict.insert(std::make_pair(tilePosition, action));
		};

		// Move the pending tiles into private collections to iterate
		std::unordered_map<Vector2Int, char, Vector2Hasher> oldPendingTileRemovals = std::move(m_PendingTileRemovals);
		std::unordered_map<Vector2Int, Tile, Vector2Hasher> oldPendingTileAdditions = std::move(m_PendingTileAdditions);
		m_PendingTileRemovals = {}, m_PendingTileAdditions = {}; // Clear up pending tiles to be iterated next game cycle
		// Apply
		Container.RemoveTiles(oldPendingTileRemovals.begin(), oldPendingTileRemovals.end(), m_LeastTileRemovalKey, time, OnTileChange);
		Container.AddTiles(oldPendingTileAdditions.begin(), oldPendingTileAdditions.end(), m_GreatestTileAdditionKey, time, OnTileChange);

		// Iterate
		std::vector<std::pair<Vector2Int, unsigned int>> tilesToUpdateList = std::vector<std::pair<Vector2Int, unsigned int>>(tilesToUpdateDict.begin(), tilesToUpdateDict.end());
		std::pair<Vector2Int, unsigned int> currentTileToUpdate; TilemapContainer::TILE_INFO tileInfo;
		Vector2Int currentAdjPosition;
		for (int i = 0; i < tilesToUpdateList.size(); i++)
		{
			currentTileToUpdate = tilesToUpdateList.at(i);
			tileInfo = Container.GetTileInChunk(currentTileToUpdate.first);

			// Update tile
			if (tileInfo.second == nullptr || !TileBehavior::GetTileBehavior(tileInfo.second->BehaviorIndex)->TryUpdate(this, currentTileToUpdate.first, tileInfo, time, (TileBehavior::TileActionTypes)currentTileToUpdate.second))
				continue; // Continue and don't append adjacent
			
			// Append adjacent
			if (tilesToUpdateList.size() + 8 > MAX_ADJACENT_TILES) // TODO: Better way to check for the limit if you calculate it and designate space first(this reduces checks)
			{
				fprintf(stdout, std::string("Reached max adjacent tiles limit!! ").append(std::to_string(time.CurrentTime)).append("\n").c_str());
				break;
			}
			for (int ii = 0; ii < 8; ii++)
			{
				currentAdjPosition = currentTileToUpdate.first + ADJACENT_TILE_POSITIONS[ii];
				
				auto pair = std::make_pair(currentAdjPosition, TileBehavior::TILE_ACTION_REFRESH);
				if (tilesToUpdateDict.insert(pair).second)
					tilesToUpdateList.push_back(pair);
			}
		}*/

		// TODO: Make this more efficient cause we are reaching our limit way way too fast
		// TODO: #1 Use gpu to get adjacent tiles and then update after
		// TODO: #2 Could try to use addblock of tiles wherever we can using an algorithm to sort and group pending tile changes. acceleration structure possibly
		// TODO: #3 Store adjacent tiles overflow and update them next cycle?
	}
	// Inherited via IGameObject
	void Tilemap::Release()
	{
		TileBehavior::ReleaseTileBehaviors();

		if (p_Mesh != nullptr)
		{
			MasterWindow::Pipeline.GetLayer(RenderLayerNames::RENDERLAYERS_Tilemap0).UnregisterMesh(p_Mesh);
			p_Mesh = nullptr;
		}
	}

	std::pair<Vector2, TilemapContainer::TILE_INFO> Tilemap::Raycast(Vector2Int origin, Vector2 vector, bool *pSucceeded)
	{
		// TODO: Implement effecient grid raycast method

		int endSqrd = vector.GetMagnitudeSqrd();
		Vector2 currentPosition = {}; Vector2 direction = vector.Normalize();
		TilemapContainer::TILE_INFO hitTile = {};
		int d = 0;
		for (d = 1; d * d < endSqrd + 1; d++)
		{
			currentPosition = Vector2(origin.X + .5, origin.Y + .5) + direction * d;
			if (!Container.IsTileInBounds(currentPosition) || PendingTileChangesRegistry.WillAddTile(currentPosition))
				break;
			if (!PendingTileChangesRegistry.WillRemoveTile(currentPosition) && Container.ContainsTile(currentPosition))
			{
				// Hit something
				hitTile = Container.GetTileInChunk(currentPosition);
				break;
			}
			// Did not hit something
		}
		*pSucceeded = d > 1;

		// fail or hit
		return std::make_pair((Vector2Int)(currentPosition - direction), hitTile);
	}

	Vector2Int Tilemap::TryStepMoveTile(
		Vector2Int tilePosition,
		Vector2 vector,
		bool* pSucceeded)
	{
		std::pair<Vector2, TilemapContainer::TILE_INFO> hit = Raycast(tilePosition, vector, pSucceeded);
		if (!*pSucceeded)
			return tilePosition;
		// Raycast succeeded !

		Vector2 newTilePosition = hit.first;
		PendingTileChangesRegistry.SwapTiles(tilePosition, newTilePosition);
		return newTilePosition;
	}

	// TODO: This can be Vector2Int
	Vector2 Tilemap::FromWorldToTile(Vector2 world)
	{
		world -= Position;
		world /= TileSize;
		return Vector2(floor(world.X), floor(world.Y));
	}
	Vector2 Tilemap::FromTileToWorld(Vector2 tile)
	{
		tile *= TileSize;
		tile += Position;
		return tile;
	}
}