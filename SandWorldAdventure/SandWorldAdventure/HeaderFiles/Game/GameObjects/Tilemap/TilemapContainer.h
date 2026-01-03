#pragma once
#include <vector>
#include "Chunk.h"
#include "HeaderFiles/Math.h"
#include "HeaderFiles/Time.h"

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
		/// <summary>
		/// Expects Iter to be of std::pair<Vector2Int, Tile>
		/// Expects LambdaFunction to have the arguments (TILE_INFO tileInfo, Vector2Int tilePosition, Time timeInfo, unsigned int action)
		///  and assumes that no tile has a key value < (0,0) on either axis
		/// </summary>
		/// <typeparam name="Iter"></typeparam>
		/// <param name="greatestTileKey">The top right bounding box value of the collection</param>
		/// <returns></returns>
		template<typename Iter, typename LambdaFunction>
		inline bool AddTiles(Iter tilesBegin, Iter tilesEnd, Vector2Int greatestTileKey, Time timeInfo, LambdaFunction OnAdd)
		{
			// try create more space
			if (!IsTileInBounds(greatestTileKey))
				TryCreateChunks(greatestTileKey);

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
				pExistingTile->LastMoveTime = timeInfo.CurrentTime - timeInfo.FrameDeltaTime; // For physics

				// Allow extra functionality
				OnAdd(std::make_pair(pChunk, pExistingTile), currentIterator->first, timeInfo, 1);
			}

			return true;
		}
		
		// Expects Iter to be of std::pair<Vector2Int, char>
		// Expects LambdaFunction to have the arguments (TILE_INFO tileInfo, Vector2Int tilePosition, Time timeInfo, unsigned int action)
		template<typename Iter, typename LambdaFunction>
		inline bool RemoveTiles(Iter tilesBegin, Iter tilesEnd, Vector2Int leastTileKey, Time timeInfo, LambdaFunction OnRemove)
		{
			if (leastTileKey.X >= GetTileBounds().X ||
				leastTileKey.Y >= GetTileBounds().Y)
				return false;

			Vector2Int chunkPosition;
			Chunk* pChunk;
			Tile* pExistingTile;
			for (Iter currentIterator = tilesBegin; currentIterator != tilesEnd; currentIterator++)
			{
				if (currentIterator->first.X >= GetTileBounds().X ||
					currentIterator->first.Y >= GetTileBounds().Y)
					continue;
				chunkPosition = TileToChunkCoordinates(currentIterator->first);
				pChunk = GetChunk(chunkPosition.Y * GetChunkBounds().X + chunkPosition.X);
				if (!IsChunkInitialized(pChunk))
					continue;
				pExistingTile = GetTileInChunk(pChunk, currentIterator->first);

				if (currentIterator->second != '\1') // when char == '\1' it is a tile refresh
				{
					// Change tile
					if (pExistingTile->HasValue)
						pChunk->NonEmptyTilesCount--;
					pExistingTile->HasValue = false;
				}
				// Allow extra functionality
				OnRemove(std::make_pair(pChunk, pExistingTile), currentIterator->first, timeInfo, currentIterator->second == '\1' ? 4 : 2); // 4 is refresh, 2 is remove
			}
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