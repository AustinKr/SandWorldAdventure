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