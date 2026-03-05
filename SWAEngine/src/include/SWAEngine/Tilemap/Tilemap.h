#pragma once
#include "SWAEngine/Tilemap/ITilemapContainer.h"

namespace SWAEngine::Tilemap
{
	struct SWA_ENGINE_API Tilemap
	{
	public:
		// In world coordinates
		Math::Vector2 Origin;
		// In world coordinates
		Math::Vector2 TileScale;

		Tilemap(Math::Vector2 origin = {}, Math::Vector2 scale = {1,1});
		void Release();
		bool IsEmpty();

		// Returns a copy of the tile
		Tile GetTile(Math::Vector2Int position);
		// Sets the tile (in the pending tiles container)
		Tile& SetTile(Math::Vector2Int position, Tile tile);

		// Should be called every frame cycle
		void Update();

		Math::Vector2 TileToWorld(Math::Vector2Int tile, bool applyOffsets = true);
		Math::Vector2Int WorldToTile(Math::Vector2 world, bool applyOffsets = true);

		// Returns the bounds of the map in tiles
		// (the top-right-most tile position)
		Math::Vector2Int GetBounds();
	private:
		// Contains currently active tiles
		ITilemapContainer* mp_ActiveTilesContainer;
		// Contains new override tiles or null tiles to replace any currently active in their place
		ITilemapContainer* mp_PendingTilesContainer;

		void ApplyPendingTiles();
	};
}