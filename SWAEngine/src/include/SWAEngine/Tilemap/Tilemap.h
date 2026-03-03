#pragma once
#include "SWAEngine/Tilemap/ITilemapContainer.h"

namespace SWAEngine::Tilemap
{
	struct SWA_ENGINE_API Tilemap
	{
	public:
		// In world coordinates
		Vector2 Origin;

		Tilemap();
		void Release();

		// Returns a copy of the tile
		Tile GetTile(Vector2Int position);
		// Sets the tile (in the pending tiles container)
		Tile& SetTile(Vector2Int position, Tile tile);

		// Should be called every frame cycle
		void Update();

		Vector2 TileToWorld(Vector2Int);
		Vector2Int WorldToTile(Vector2);

	private:
		// Contains currently active tiles
		ITilemapContainer* mp_ActiveTilesContainer;
		// Contains new override tiles or null tiles to replace any currently active in their place
		ITilemapContainer* mp_PendingTilesContainer;

		void ApplyPendingTiles();
	};
}