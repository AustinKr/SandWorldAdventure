#pragma once
#include <vector>
#include "Chunk.h"
#include "HeaderFiles/Math.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class TilemapContainer
	{
		// TODO: Would be useful to keep track of the real count of tiles

	private:
		//TODO: Should use a dictionary for chunks if not a quadtree
		// Chunks of tiles
		std::vector<Chunk> m_Chunks;

		// The columns and rows of chunks
		Vector2Int m_ChunkBounds;
		// The width and height of all the tiles in the map
		Vector2Int m_TileBounds;

	public:
		typedef std::pair<Chunk*, Tile*> TILE_INFO;
		static const Vector2Int CHUNK_SIZE;

		TilemapContainer();

		// Retreives the columns and rows of chunks
		Vector2Int GetChunkBounds();
		// Retreives the columns and rows of tiles
		Vector2Int GetTileBounds();
		// Gets the chunk
		Chunk* GetChunk(int i);

		// Calculates the position of the chunk which contains the tile position
		Vector2Int TileToChunkCoordinates(Vector2Int tilePosition);

		// Assumes that the chunk is exists and is initialized
		Tile* GetTileInChunk(Chunk* pChunk, Vector2Int tilePosition);
		/// <summary>
		/// A safe way of retrieving a tile
		/// </summary>
		/// <param name="tilePosition"></param>
		/// <returns>Returns the chunk and tile (if any). The chunk may be returned but tile nullptr if the chunk is uninitialized</returns>
		TILE_INFO GetTileInChunk(Vector2Int tilePosition);
		// Uses GetTileInChunk makes comparision
		bool ContainsTile(Vector2Int tilePosition);

		// Creates and does NOT initialize chunks
		void AssignChunks(Vector2Int chunkBounds);
		TILE_INFO AddTile(Vector2Int tilePosition, Tile newTile, double currentTime, bool tryCreate = true);
		TILE_INFO RemoveTile(Vector2Int tilePosition);
		template<typename Iter>
		// Expects Iter to be of Tile
		inline bool AddBlockOfTiles(Iter tileIteratorBegin, Vector2Int origin, Vector2Int size, double currentTime)
		{
			if (origin.X < 0 || origin.Y < 0)
				return false;

			// Try create more space
			if (!IsTileInBounds(origin + size))
				TryCreateChunks(origin + size);

			Vector2Int currentChunkPos = TileToChunkCoordinates(origin);
			Iter tileIterator = tileIteratorBegin;
			for (int i = origin.X; i < origin.X + size.X; i++)
			{
				for (int j = origin.Y; j < origin.Y + size.Y; j++)
				{
					Chunk* pChunk = TryInitializeChunk(currentChunkPos);
					Tile* tile = GetTileInChunk(pChunk, Vector2Int(i, j));

					// It is addition of a tile if the current tile is empty
					if (!tile->HasValue)
						pChunk->NonEmptyTilesCount++;

					// Change tile
					*tile = *tileIterator;
					tile->HasValue = true;
					tile->LastMoveTime = currentTime; // For physics

					// Iterate
					tileIterator++;

					// Try update current chunk
					if (j % CHUNK_SIZE.Y == 0)
						currentChunkPos = TileToChunkCoordinates(Vector2Int(i, j));
				}
				// Try update current chunk
				if (i % CHUNK_SIZE.X == 0)
					currentChunkPos = TileToChunkCoordinates(Vector2Int(i, origin.Y + size.Y - 1));
			}

			return true;
		}
		template<typename Iter>
		// Expects Iter to be of std::pair<Vector2Int, Tile>
		//  and assumes that no tile has a key value < (0,0) on either axis
		inline bool AddTiles(Iter tilesBegin, Iter tilesEnd, Vector2Int boundingBoxValue, double currentTime)
		{
			// try create more space
			if (!IsTileInBounds(boundingBoxValue))
				TryCreateChunks(boundingBoxValue);

			Chunk* pChunk;
			Tile* pExistingTile;
			for (Iter currentIterator = tilesBegin; currentIterator != tilesEnd; currentIterator++)
			{
				pChunk = TryInitializeChunk(TileToChunkCoordinates(currentIterator->first));
				pExistingTile = GetTileInChunk(pChunk, currentIterator->first);

				// It is addition of a tile if the current tile is empty
				if (!pExistingTile->HasValue)
					pChunk->NonEmptyTilesCount++;

				// Change tile
				*pExistingTile = currentIterator->second;
				pExistingTile->HasValue = true;
				pExistingTile->LastMoveTime = currentTime; // For physics
			}

			return true;
		}

		// Swaps the tiles stored at each position (does not update). Also note that the swap will still occur even if both tiles are empty
		void SwapTiles(TILE_INFO tileInfoA, Vector2Int positionA, TILE_INFO tileInfoB, Vector2Int positionB);
		// Swaps the tiles stored at each position (does not update). Also note that the swap will still occur even if both tiles are empty
		void SwapTiles(Vector2Int positionA, Vector2Int positionB, OUT TILE_INFO* pNewTileInfoA = nullptr, OUT TILE_INFO* pNewTileInfoB = nullptr);

		// Makes sure the chunk is initialized and knows where it is
		Chunk* TryInitializeChunk(Vector2Int chunkPosition);

		// Creates more chunks if the tile(x,y) is outside of the tilemap
		void TryCreateChunks(Vector2Int tilePosition);

		// Checks whether the chunk has tiles initialized
		static bool IsChunkInitialized(Chunk* chunk);
		bool IsTileInBounds(Vector2Int tilePosition) const;
	};
}