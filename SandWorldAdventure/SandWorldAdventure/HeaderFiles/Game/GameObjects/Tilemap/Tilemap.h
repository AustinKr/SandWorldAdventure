#pragma once

#include "HeaderFiles/Math.h"
#include "HeaderFiles/Game/GameObjects/IGameObject.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/TilemapMesh.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/TilemapContainer.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/PendingTileChangesRegistry.h"
#include <unordered_map>

#define TILEMAP_H
namespace SandboxEngine::Game::GameObject::Tilemap
{
	class Tilemap : public IGameObject
	{
		//// A collection of new tiles that will replace the tilemap at the end of each game cycle

		// TODO: Could make structs to store this information

		//Vector2Int m_GreatestTileAdditionKey;
		//Vector2Int m_LeastTileRemovalKey;
		//// This is also to be used for refreshes if char == '\1'
		//std::unordered_map<Vector2Int, char, Vector2Hasher> m_PendingTileRemovals;
		//std::unordered_map<Vector2Int, Tile, Vector2Hasher> m_PendingTileAdditions;

	public:
		static const int MAX_RAYCAST_STEPS;
		static const int MAX_TILE_UPDATES;
		static const Vector2Int ADJACENT_TILE_POSITIONS[8];
		
		GraphicsPipeline::TilemapMesh* p_Mesh;
		TilemapContainer Container;
		PendingTileChangesRegistry PendingTileChangesRegistry;
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

		//// TODO: Could simplify add and remove tiles so it will just make the change and override whatever
		//bool AddTile(Vector2Int tilePosition, Tile newTile, bool shouldOverride = true);
		//bool RemoveTile(Vector2Int tilePosition);

		//// TODO: Create AddTiles and AddTileBlock functions in tilemap

		//bool SwapTiles(Vector2Int tilePositionA, Vector2Int tilePositionB);
		//void RefreshTile(Vector2Int tilePosition);


		/// <summary>
		/// 
		/// </summary>
		/// <param name="origin"></param>
		/// <param name="vector"></param>
		/// <returns>The position of the space before the hit tile, which is the second parameter</returns>
		std::pair<Vector2, TilemapContainer::TILE_INFO> Raycast(Vector2Int origin, Vector2 vector, bool *pSucceeded);
		
		Vector2Int TryStepMoveTile(
			Vector2Int tilePosition,
			Vector2 vector,
			bool* pSucceeded);

		Vector2 FromWorldToTile(Vector2 world);
		Vector2 FromTileToWorld(Vector2 tile);

	};
}