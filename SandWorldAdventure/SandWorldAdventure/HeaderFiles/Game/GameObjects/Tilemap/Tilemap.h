#pragma once

#include "HeaderFiles/Math.h"
#include "HeaderFiles/Game/GameObjects/IGameObject.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/TilemapMesh.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/TilemapContainer.h"
#include <unordered_map>

#define TILEMAP_H
namespace SandboxEngine::Game::GameObject::Tilemap
{
	class Tilemap : public IGameObject
	{
	private:
		static const int MAX_RAYCAST_STEPS;
		static const int MAX_ADJACENT_TILES;
		static const Vector2Int ADJACENT_TILE_POSITIONS[8];

		//// A collection of new tiles that will replace the tilemap at the end of each game cycle

		// TODO: Could make structs to store this information

		Vector2Int m_GreatestTileAdditionKey;
		Vector2Int m_LeastTileRemovalKey;
		// the second parameter is not used(char)
		std::unordered_map<Vector2Int, char, Vector2Hasher> m_PendingTileRemovals;
		std::unordered_map<Vector2Int, Tile, Vector2Hasher> m_PendingTileAdditions;

	public:
		
		GraphicsPipeline::TilemapMesh* p_Mesh;
		TilemapContainer Container;
		Vector2 Position;
		Vector2 TileSize;

		Tilemap(Vector2Int chunkBounds);
		Tilemap();

		/// <summary>
		///  Inherited via IGameObject
		/// Trys to update all chunks and tiles
		/// </summary>
		virtual void Update(Time time) override;
		// Inherited via IGameObject
		virtual void Release() override;

		bool AddTile(Vector2Int tilePosition, Tile newTile, bool shouldOverride = true);
		bool RemoveTile(Vector2Int tilePosition);

		// TODO: Create AddTiles and AddTileBlock functions in tilemap

		bool SwapTiles(Vector2Int tilePositionA, Vector2Int tilePositionB);
		bool WillContainTile(Vector2Int tilePosition);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="origin"></param>
		/// <param name="direction"></param>
		/// <param name="end"></param>
		/// <returns>the distance traveled before it hit the tile(or null) in front which is returned through tile info</returns>
		std::pair<double, TilemapContainer::TILE_INFO> Raycast(Vector2 origin, Vector2 direction, double end);
		
		Vector2 FromWorldToTile(Vector2 world);
		Vector2 FromTileToWorld(Vector2 tile);

	};
}