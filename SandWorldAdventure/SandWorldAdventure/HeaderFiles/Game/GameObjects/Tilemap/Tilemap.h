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
		// A collection of new tiles that will replace the tilemap at the end of each game cycle
		std::unordered_map<
			Vector2Int,
			Tile,
			Vector2Hasher> m_PendingNewTiles;

		const int MAX_RAYCAST_STEPS = 600;
		const int MAX_ADJACENT_TILES = 3000;
		const Vector2Int ADJACENT_TILE_POSITIONS[8] =
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

		//Set and remove are basically equivalent- the only major difference is the functionality or convenience they provide
		bool SetTile(Vector2Int tilePosition, Tile newTile, bool shouldOverride = true);
		//Set and remove are basically equivalent- the only major difference is the functionality or convenience they provide
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