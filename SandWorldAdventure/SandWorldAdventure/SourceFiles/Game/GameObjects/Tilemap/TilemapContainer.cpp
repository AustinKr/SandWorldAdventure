#include "HeaderFiles/Game/GameObjects/Tilemap/TilemapContainer.h"
#include <string>

namespace SandboxEngine::Game::GameObject::Tilemap
{
	const Vector2Int TilemapContainer::CHUNK_SIZE = Vector2Int(25, 25);

	TilemapContainer::TilemapContainer()
	{
		m_Chunks = std::vector<Chunk>();
		/*m_ChunksHeight = 0;
		m_ChunksWidth = 0;
		m_Width = 0;
		m_Height = 0;*/
		m_ChunkBounds = {};
		m_TileBounds = {};
	}

	Vector2Int TilemapContainer::GetChunkBounds() { return m_ChunkBounds; }
	Vector2Int TilemapContainer::GetTileBounds() { return m_TileBounds; }
	Chunk* TilemapContainer::GetChunk(int i) { return &m_Chunks[i]; };

	Vector2Int TilemapContainer::TileToChunkCoordinates(Vector2Int tilePosition)
	{
		Vector2 chunkPos = Vector2(floor(tilePosition.X / (int)CHUNK_SIZE.X), floor(tilePosition.Y / (int)CHUNK_SIZE.Y));
		return chunkPos;
	}
	
	Tile* TilemapContainer::GetTileInChunk(Chunk* pChunk, Vector2Int tilePosition)
	{
		Vector2 newXY = Vector2(tilePosition.X % (int)CHUNK_SIZE.X, tilePosition.Y % (int)CHUNK_SIZE.Y);
		return &pChunk->Tiles.value()[newXY.X + newXY.Y * CHUNK_SIZE.X];
	}
	TilemapContainer::TILE_INFO TilemapContainer::GetTileInChunk(Vector2Int tilePosition) //TODO: Check all that use this and try to replace with the other function to save memory
	{
		if (!IsTileInBounds(tilePosition))
			return std::make_pair(nullptr, nullptr);
		// There should definitely be a chunk because the tile is in bounds

		Vector2 chunkCoords = TileToChunkCoordinates(tilePosition);
		Chunk* chunk = GetChunk(chunkCoords.X + chunkCoords.Y * m_ChunkBounds.X);
		
		if (!IsChunkInitialized(chunk))
			return std::make_pair(chunk, nullptr);
		return std::make_pair(chunk, GetTileInChunk(chunk, tilePosition));
	}
	bool TilemapContainer::ContainsTile(Vector2Int tilePosition)
	{
		TILE_INFO tileInfo = GetTileInChunk(tilePosition);
		return tileInfo.second != nullptr && tileInfo.second->HasValue;
	}

	void TilemapContainer::AssignChunks(Vector2Int chunkBounds)
	{
		m_ChunkBounds = chunkBounds;
		m_TileBounds = m_ChunkBounds * CHUNK_SIZE;
		
		m_Chunks.assign(m_ChunkBounds.X * m_ChunkBounds.Y, Chunk());
	}
	TilemapContainer::TILE_INFO TilemapContainer::AddTile(Vector2Int tilePosition, Tile newTile, double currentTime, bool tryCreate)
	{
		// Generate more space if needed
		if (!IsTileInBounds(tilePosition))
		{
			if (tryCreate)
				TryCreateChunks(tilePosition);
			else
				return std::make_pair(nullptr, nullptr);
		}
		Vector2 chunkCoordinates = TileToChunkCoordinates(tilePosition);
		// Update the chunk
		Chunk* chunk = TryInitializeChunk(chunkCoordinates);
		Tile* tile = GetTileInChunk(chunk, tilePosition);

		// It is addition of a tile if the current tile is empty
		if (!tile->HasValue)
			chunk->NonEmptyTilesCount++;

		// Change tile
		*tile = newTile;
		tile->HasValue = true;
		tile->LastMoveTime = currentTime; // For physics
		return std::make_pair(chunk, tile);
	}
	TilemapContainer::TILE_INFO TilemapContainer::RemoveTile(Vector2Int tilePosition)
	{
		TILE_INFO tileInfo = GetTileInChunk(tilePosition);
		if (tileInfo.second == nullptr)
			return std::make_pair(nullptr, nullptr);

		if (tileInfo.second->HasValue)
			tileInfo.first->NonEmptyTilesCount--;

		tileInfo.second->HasValue = false;
		return tileInfo;
	}


	void TilemapContainer::SwapTiles(TILE_INFO tileInfoA, Vector2Int positionA, TILE_INFO tileInfoB, Vector2Int positionB)
	{
		if (tileInfoA.second == nullptr || tileInfoB.second == nullptr)
			return;

		// Remove and add tiles from their chunks
		if (tileInfoA.second->HasValue)
		{
			tileInfoA.first->NonEmptyTilesCount--;
			tileInfoB.first->NonEmptyTilesCount++;
		}
		if (tileInfoB.second->HasValue)
		{
			tileInfoB.first->NonEmptyTilesCount--;
			tileInfoA.first->NonEmptyTilesCount++;
		}
		Tile buffer = *tileInfoA.second;
		*tileInfoA.second = *tileInfoB.second;
		*tileInfoB.second = buffer;
	}
	void TilemapContainer::SwapTiles(Vector2Int positionA, Vector2Int positionB, OUT TILE_INFO* pNewTileInfoA, OUT TILE_INFO* pNewTileInfoB)
	{
		if (positionA == positionB)
			return;

		TILE_INFO tileInfoA = GetTileInChunk(positionA);
		TILE_INFO tileInfoB = GetTileInChunk(positionB);
		if (tileInfoA.second == nullptr || tileInfoB.second == nullptr)
			return;
		SwapTiles(tileInfoA, positionA, tileInfoB, positionB);

		if (pNewTileInfoA != nullptr)
			*pNewTileInfoA = tileInfoA;
		if (pNewTileInfoB != nullptr)
			*pNewTileInfoB = tileInfoB;
	}


	Chunk* TilemapContainer::TryInitializeChunk(Vector2Int chunkPosition)
	{
		// Update the chunk
		Chunk* chunk = &m_Chunks[chunkPosition.Y * m_ChunkBounds.X + chunkPosition.X];
		chunk->X = chunkPosition.X;
		chunk->Y = chunkPosition.Y;

		// make sure the chunk tiles are initialized
		if (!IsChunkInitialized(chunk))
			chunk->Tiles = std::vector<Tile>(CHUNK_SIZE.X * CHUNK_SIZE.Y, Tile());
		return chunk;
	}

	void TilemapContainer::TryCreateChunks(Vector2Int tilePosition)
	{
		int addedWidthTiles = tilePosition.X - m_TileBounds.X + 1;
		int addedHeightTiles = tilePosition.Y - m_TileBounds.Y + 1;
		Vector2 addedChunks = TileToChunkCoordinates(Vector2Int(addedWidthTiles, addedHeightTiles));
		// We have to add 1 because we want the COUNT not the INDEX
		addedChunks.X += 1;
		addedChunks.Y += 1;

		// Go through the needed chunks and add them
		if (addedChunks.X > 0)
		{
			Chunk newChunk;
			std::vector<Chunk>::const_iterator iterator;
			for (int h = m_ChunkBounds.Y - 1; h >= 0; h--)
			{
				for (int w = m_ChunkBounds.X; w < addedChunks.X + m_ChunkBounds.X; w++)
				{
					newChunk = Chunk();
					newChunk.X = w;
					newChunk.Y = h;

					iterator = m_Chunks.cbegin() + h * m_ChunkBounds.X; // Iterator becomes invalidated after each insertion because keys must be rehashed
					m_Chunks.insert(iterator + w, newChunk);
				}
			}
			m_ChunkBounds.X += addedChunks.X;
			m_TileBounds.X = m_ChunkBounds.X * CHUNK_SIZE.X;
		}
		if (addedChunks.Y > 0)
		{
			Chunk newChunk;
			for (int h = m_ChunkBounds.Y; h < addedChunks.Y + m_ChunkBounds.Y; h++)
			{
				for (int w = 0; w < m_ChunkBounds.X; w++)
				{
					newChunk = Chunk();
					newChunk.X = w;
					newChunk.Y = h;
					m_Chunks.push_back(newChunk);
				}
			}
			m_ChunkBounds.Y += addedChunks.Y;
			m_TileBounds.Y = m_ChunkBounds.Y * CHUNK_SIZE.Y;
		}
	}

	bool TilemapContainer::IsChunkInitialized(Chunk* chunk) { return chunk->Tiles.has_value() && chunk->Tiles.value().size() == CHUNK_SIZE.X * CHUNK_SIZE.Y; }
	bool TilemapContainer::IsTileInBounds(Vector2Int tilePosition) const { return tilePosition.X >= 0 && tilePosition.Y >= 0 && tilePosition.X < m_TileBounds.X && tilePosition.Y < m_TileBounds.Y; }
}