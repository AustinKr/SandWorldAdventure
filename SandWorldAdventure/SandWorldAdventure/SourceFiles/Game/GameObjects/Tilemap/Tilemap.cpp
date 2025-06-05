#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/RenderLayerNames.h"
#include "HeaderFiles/Game/GameInstance.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{

	const int Tilemap::MAX_RAYCAST_STEPS = 600;
	const int Tilemap::MAX_ADJACENT_TILES = 8000;
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

	Tilemap::Tilemap() : m_PendingTileRemovals{}, m_PendingTileAdditions{}, Container{}
	{
		p_Mesh = new GraphicsPipeline::TilemapMesh(this);
		MasterWindow::Pipeline.GetLayer(RenderLayerNames::RENDERLAYERS_Tilemap0).RegisterMesh(p_Mesh);

		Container.AssignChunks(Vector2Int());
		Position = Vector2(0, 0);
		TileSize = Vector2(0, 0);
	}
	Tilemap::Tilemap(Vector2Int chunkBounds) : m_PendingTileRemovals{}, m_PendingTileAdditions{}, Container{}
	{
		p_Mesh = new GraphicsPipeline::TilemapMesh(this);
		MasterWindow::Pipeline.GetLayer(RenderLayerNames::RENDERLAYERS_Tilemap0).RegisterMesh(p_Mesh);

		Container.AssignChunks(chunkBounds);
		Position = Vector2(0, 0);
		TileSize = Vector2(0, 0);
	}

	// Inherited via IGameObject
	void Tilemap::Update(Time time)
	{

		//// - Apply pending tile changes -
		//for (auto& iter : m_PendingNewTiles)
		//{
		//	if (tilesToUpdateList.size() + 1 > MAX_ADJACENT_TILES) // This check really shouldn't be needed mostly
		//		break;

		//	if (iter.second.HasValue)
		//	{
		//		tileInfo = Container.AddTile(iter.first, iter.second, time.CurrentTime); // Make change in container
		//		if (tilesToUpdateDict.insert(std::make_pair(iter.first, TileBehavior::TILE_ACTION_ADD)).second)
		//			tilesToUpdateList.push_back(iter.first);
		//		continue;
		//	}

		//	Container.RemoveTile(iter.first);
		//	if (tilesToUpdateDict.insert(std::make_pair(iter.first, TileBehavior::TILE_ACTION_REMOVE)).second)
		//		tilesToUpdateList.push_back(iter.first);
		//}
		//m_PendingNewTiles.clear();


		/*
		if (m_TilesToUpdateDict.size() + 8 > MAX_ADJACENT_TILES)
			{
				fprintf(stdout, "Reached max adjacent tiles limit!!\n");
				return false;
			}
			*/

		std::unordered_map<
			Vector2Int, // Position
			unsigned int, // Action
			Vector2Hasher> tilesToUpdateDict = {}; // Used to check if a position is already here
		std::vector<Vector2Int> tilesToUpdateList = {}; // Used to iterate in chronological order

		auto OnTileChange = [&](TilemapContainer::TILE_INFO tileInfo, Vector2Int tilePosition, Time timeInfo, unsigned int action)
		{
			if (!TileBehavior::GetTileBehavior(tileInfo.second->BehaviorIndex)->TryUpdate(this, tilePosition, tileInfo, timeInfo, (TileBehavior::TileActionTypes)action))
				return;
			// Add to collections so we can update adjacent
			tilesToUpdateDict.insert(std::make_pair(tilePosition, action));
			tilesToUpdateList.push_back(tilePosition);

			//// So it won't try to be updated again. action == Add(1) or Remove(2)
			//if (tilesToUpdateDict.contains(tilePosition))
			//	tilesToUpdateDict[tilePosition] = action;
			//else
			//	tilesToUpdateDict.insert(std::make_pair(tilePosition, action));

			//// Update the tile
			//if (!TileBehavior::GetTileBehavior(tileInfo.second->BehaviorIndex)->TryUpdate(this, tilePosition, tileInfo, timeInfo, (TileBehavior::TileActionTypes)action))
			//	return;

			//// Insert adjacent
			//Vector2Int currentAdjPosition;
			//for (int i = 0; i < 8; i++)
			//{
			//	currentAdjPosition = tilePosition + ADJACENT_TILE_POSITIONS[i];

			//	/*if (m_TilesToUpdateDict.insert(std::make_pair(currentAdjPosition, TileBehavior::TILE_ACTION_UPDATE)).second)
			//		m_TilesToUpdateList.push_back(currentAdjPosition);*/
			//	tilesToUpdateDict.insert(std::make_pair(currentAdjPosition, TileBehavior::TILE_ACTION_UPDATE));
			//}
		};

		fprintf(stdout, std::to_string(m_PendingTileRemovals.size()).append(" r\n").c_str());
		fprintf(stdout, std::to_string(m_PendingTileAdditions.size()).append(" a\n").c_str());

		Container.RemoveTiles(m_PendingTileRemovals.begin(), m_PendingTileRemovals.end(), m_LeastTileRemovalKey, time, OnTileChange);
		m_PendingTileRemovals.clear();

		Container.AddTiles(m_PendingTileAdditions.begin(), m_PendingTileAdditions.end(), m_GreatestTileAdditionKey, time, OnTileChange);
		m_PendingTileAdditions.clear();



		//fprintf(stdout, std::to_string(tilesToUpdateDict.size()).append("\n").c_str());
		//fprintf(stdout, std::to_string(tilesToUpdateList.size()).append("\n").c_str());

		// - Update blocks of adjacent tiles -
		Vector2Int currentAdjPosition, currentPosition;
		TilemapContainer::TILE_INFO tileInfo;
		for (int i = 0; i < tilesToUpdateList.size(); i++)
		{
			currentPosition = tilesToUpdateList.at(i);
			// Try update
			if (tilesToUpdateDict[currentPosition] == TileBehavior::TILE_ACTION_UPDATE)
			{
				// Update tile

				tileInfo = Container.GetTileInChunk(currentPosition);
				if (tileInfo.second == nullptr || !TileBehavior::GetTileBehavior(tileInfo.second->BehaviorIndex)->TryUpdate(this, currentPosition, tileInfo, time, (TileBehavior::TileActionTypes)tilesToUpdateDict[currentPosition]))
					continue; // Continue and don't append adjacent

				// Debug feature -
				/*
				Vector2 currentWorldPosition = FromTileToWorld(currentPosition);
				GameInstance::p_DebugServiceObject->CreateRectangle(currentWorldPosition + TileSize * 0.375, currentWorldPosition + TileSize * 0.75, .01, GraphicsPipeline::GraphicsPipeline2D::GP2D_BASE_SHADER);
				*/
				// -
			}
			
			// Append adjacent
			if (tilesToUpdateList.size() + 8 > MAX_ADJACENT_TILES)
			{
				fprintf(stdout, std::string("Reached max adjacent tiles limit!! ").append(std::to_string(time.CurrentTime)).append("\n").c_str());
				break;
			}
			for (int ii = 0; ii < 8; ii++)
			{
				currentAdjPosition = currentPosition + ADJACENT_TILE_POSITIONS[ii];
				
				if (tilesToUpdateDict.insert(std::make_pair(currentAdjPosition, TileBehavior::TILE_ACTION_UPDATE)).second)
					tilesToUpdateList.push_back(currentAdjPosition);
			}
		}

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

	bool Tilemap::AddTile(Vector2Int tilePosition, Tile newTile, bool shouldOverride)
	{
		if (tilePosition.X < 0 || tilePosition.Y < 0) // So it doesn't try to add into void
			return false;
		if (!shouldOverride && Container.ContainsTile(tilePosition)) // Don't override existing container tiles if !shouldOverride
			return false;

		newTile.HasValue = true; // Make sure it is known as addition

		if (m_PendingTileAdditions.contains(tilePosition))
		{
			if (!shouldOverride && m_PendingTileAdditions.at(tilePosition).HasValue) // Don't override existing additions if !shouldOverride
				return false;

			m_PendingTileAdditions.at(tilePosition) = newTile; // Override
			return true;
		}

		// Erase any existing removal
		if (m_PendingTileRemovals.contains(tilePosition))
			m_PendingTileRemovals.erase(tilePosition);
		// Insert addition
		m_PendingTileAdditions.insert(std::make_pair(tilePosition, newTile));
		// For optimisation
		if (m_PendingTileAdditions.size() == 0)
			m_GreatestTileAdditionKey = tilePosition;
		else
		{
			m_GreatestTileAdditionKey = { 
				max(m_GreatestTileAdditionKey.X, tilePosition.X),
				max(m_GreatestTileAdditionKey.Y, tilePosition.Y) };
		}
		return true;
	}
	bool Tilemap::RemoveTile(Vector2Int tilePosition)
	{
		if (m_PendingTileRemovals.contains(tilePosition))
			return false; // Nothing to override, removals don't have any data. Leave it the same
		
		// Find whether we should remove
		bool shouldRemoveTile = false;
		if (Container.ContainsTile(tilePosition))
			shouldRemoveTile = true;
		if (m_PendingTileAdditions.contains(tilePosition))
		{
			m_PendingTileAdditions.erase(tilePosition); // Erase any existing addition

			if (!shouldRemoveTile) // Don't insert a removal if there is nothing in the container to remove, and we stopped the existing addition before it was applied
				return true;
		}
		if (!shouldRemoveTile)
			return false;

		// Insert removal
		m_PendingTileRemovals.insert(std::make_pair(tilePosition, '\0')); // Remove tile! (Second param unused.)
		// For optimisation
		if (m_PendingTileRemovals.size() == 0)
			m_LeastTileRemovalKey = tilePosition;
		else
		{
			m_LeastTileRemovalKey = {
				min(m_LeastTileRemovalKey.X, tilePosition.X),
				min(m_LeastTileRemovalKey.Y, tilePosition.Y) };
		}

		return true;
	}
	bool Tilemap::SwapTiles(Vector2Int tilePositionA, Vector2Int tilePositionB)
	{
		if (tilePositionA.X < 0 || tilePositionA.Y < 0 ||
			tilePositionB.X < 0 || tilePositionB.Y < 0 ||
			(tilePositionA == tilePositionB))
			return false;

		TilemapContainer::TILE_INFO existingA = Container.GetTileInChunk(tilePositionA);
		TilemapContainer::TILE_INFO existingB = Container.GetTileInChunk(tilePositionB);

		if (existingA.second == nullptr && existingB.second == nullptr)
			return false;
		// Swap

		if (existingB.second == nullptr || !existingB.second->HasValue)
			RemoveTile(tilePositionA); // Remove because it is equivalent to tileB
		else
			AddTile(tilePositionA, *existingB.second, true); // Set to tileB (override is true because tileA still exists)
		
		if (existingA.second == nullptr || !existingA.second->HasValue)
			RemoveTile(tilePositionB); // Remove
		else
			AddTile(tilePositionB, *existingA.second, true); // Set

		return true; 
	}
	bool Tilemap::WillContainTile(Vector2Int tilePosition)
	{
		return Container.ContainsTile(tilePosition) || m_PendingTileAdditions.contains(tilePosition);
	}

	std::pair<double, TilemapContainer::TILE_INFO> Tilemap::Raycast(Vector2 origin, Vector2 direction, double end)
	{
		// TODO: Implement effecient grid raycast method

		Vector2 currentPosition;
		double distance = 0;
		TilemapContainer::TILE_INFO hitTileInfo = std::make_pair(nullptr, nullptr);
		for (distance = 1; distance < floor(end) && distance < MAX_RAYCAST_STEPS; distance++)
		{
			currentPosition = origin + Vector2(.5, .5) + direction * distance;
			hitTileInfo = Container.GetTileInChunk(currentPosition);
			if (hitTileInfo.second == nullptr)
				continue;
			if (hitTileInfo.second->HasValue)
				return std::make_pair(distance - 1, hitTileInfo);
		}
		return std::make_pair(distance, hitTileInfo);
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