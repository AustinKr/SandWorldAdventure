#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/RenderLayerNames.h"
#include "HeaderFiles/Game/GameInstance.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	Tilemap::Tilemap()
	{
		p_Mesh = new GraphicsPipeline::TilemapMesh(this);
		MasterWindow::Pipeline.GetLayer(RenderLayerNames::RENDERLAYERS_Tilemap0).RegisterMesh(p_Mesh);

		m_PendingNewTiles = {};
		Container = {};
		Container.AssignChunks(Vector2Int());
		Position = Vector2(0, 0);
		TileSize = Vector2(0, 0);
	}
	Tilemap::Tilemap(Vector2Int chunkBounds)
	{
		p_Mesh = new GraphicsPipeline::TilemapMesh(this);
		MasterWindow::Pipeline.GetLayer(RenderLayerNames::RENDERLAYERS_Tilemap0).RegisterMesh(p_Mesh);

		m_PendingNewTiles = {};
		Container = {};
		Container.AssignChunks(chunkBounds);
		Position = Vector2(0, 0);
		TileSize = Vector2(0, 0);
	}

	// Inherited via IGameObject
	void Tilemap::Update(Time time)
	{
		std::unordered_map<
			Vector2Int,
			TileBehavior::TileActionTypes,
			Vector2Hasher> tilesToUpdateDict = {}; // Used to check if a position is already here
		std::vector<Vector2Int> tilesToUpdateList = {}; // Used to iterate in chronological order
		TilemapContainer::TILE_INFO tileInfo; // This placeholder is reused

		// - Apply pending tile changes -
		for (auto& iter : m_PendingNewTiles)
		{
			if (tilesToUpdateList.size() + 1 > MAX_ADJACENT_TILES) // This check really shouldn't be needed mostly
				break;

			if (iter.second.HasValue)
			{
				tileInfo = Container.AddTile(iter.first, iter.second, time.CurrentTime); // Make change in container
				if (tilesToUpdateDict.insert(std::make_pair(iter.first, TileBehavior::TILE_ACTION_ADD)).second)
					tilesToUpdateList.push_back(iter.first);
				continue;
			}

			Container.RemoveTile(iter.first);
			if (tilesToUpdateDict.insert(std::make_pair(iter.first, TileBehavior::TILE_ACTION_REMOVE)).second)
				tilesToUpdateList.push_back(iter.first);
		}
		m_PendingNewTiles.clear();

		fprintf(stdout, std::to_string(tilesToUpdateDict.size()).append("\n").c_str());

		// - Update blocks of adjacent tiles -
		Vector2Int currentAdjPosition, currentPosition;
		for (int i = 0; i < tilesToUpdateList.size(); i++)
		{
			currentPosition = tilesToUpdateList.at(i);
			tileInfo = Container.GetTileInChunk(currentPosition);
			if (tileInfo.second == nullptr || !TileBehavior::GetTileBehavior(tileInfo.second->BehaviorIndex)->TryUpdate(this, currentPosition, tileInfo, time, tilesToUpdateDict.at(currentPosition)))
				continue;
			
			// Debug feature -
			/*
			Vector2 currentWorldPosition = FromTileToWorld(currentPosition);
			GameInstance::p_DebugServiceObject->CreateRectangle(currentWorldPosition + TileSize * 0.375, currentWorldPosition + TileSize * 0.75, .01, GraphicsPipeline::GraphicsPipeline2D::GP2D_BASE_SHADER);
			*/
			// -

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

		fprintf(stdout, std::to_string(tilesToUpdateList.size()).append("\n").c_str());
		
		// TODO: Make this more efficient cause we are reaching our limit way way too fast

		// TODO: Store adjacent tiles overflow and update them next cycle?
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

	bool Tilemap::SetTile(Vector2Int tilePosition, Tile newTile, bool shouldOverride)
	{
		if (tilePosition.X < 0 || tilePosition.Y < 0)
			return false;
		if (!shouldOverride && Container.ContainsTile(tilePosition))
			return false;

		if (m_PendingNewTiles.contains(tilePosition))
		{
			if (!shouldOverride && m_PendingNewTiles.at(tilePosition).HasValue)
				return false;

			m_PendingNewTiles.at(tilePosition) = newTile;
			return true;
		}
		m_PendingNewTiles.insert(std::make_pair(tilePosition, newTile));
		return true;
	}
	bool Tilemap::RemoveTile(Vector2Int tilePosition)
	{
		Tile tile = {};
		tile.HasValue = false;

		if (m_PendingNewTiles.contains(tilePosition))
		{
			if (!Container.ContainsTile(tilePosition))
			{	// Erase whatever is here because this is a removal and there is already nothing in the container
				bool value = m_PendingNewTiles.at(tilePosition).HasValue;
				m_PendingNewTiles.erase(tilePosition);
				return value;
			}
			m_PendingNewTiles.at(tilePosition) = tile; // Else override the value
		}
		else if (!Container.ContainsTile(tilePosition))
			return false;
		else
			m_PendingNewTiles.insert(std::make_pair(tilePosition, tile));
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
			SetTile(tilePositionA, *existingB.second, true); // Set to tileB (override is true because tileA still exists)
		
		if (existingA.second == nullptr || !existingA.second->HasValue)
			RemoveTile(tilePositionB); // Remove
		else
			SetTile(tilePositionB, *existingA.second, true); // Set

		return true; 
	}
	bool Tilemap::WillContainTile(Vector2Int tilePosition)
	{
		return Container.ContainsTile(tilePosition) && (!m_PendingNewTiles.contains(tilePosition) || m_PendingNewTiles.at(tilePosition).HasValue);
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