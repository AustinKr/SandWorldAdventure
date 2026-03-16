#pragma once
#include "SWAEngine/Tilemap/ITilemapContainer.h"
#include "SWAEngine/Time.h"
#include <unordered_map>

namespace SWAEngine::Tilemap
{
	struct SWA_ENGINE_API Tilemap // TODO: Could make tilemap a game object
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

		// Detects collision of given rect and the active tiles
		bool DetectCollisionRect(Math::Vector2Int bottomLeft, Math::Vector2Int topRight);

		// Should be called every frame cycle
		void Update(Time time);

		Math::Vector2 TileToWorld(Math::Vector2Int tile, bool applyOffsets = true);
		Math::Vector2Int WorldToTile(Math::Vector2 world, bool applyOffsets = true);

		// Returns the bounds of the map in tiles
		// (the top-right-most tile position)
		Math::Vector2Int GetBounds();
	private:
		static const Math::Vector2Int SURROUNDING_TILES[8];

		typedef std::unordered_map<Math::Vector2Int, Tile, Math::Vector2Hasher> TILES;

		// Contains currently active tiles
		ITilemapContainer* mp_ActiveTilesContainer;
		// Contains new override tiles or null tiles to replace any currently active in their place
		ITilemapContainer* mp_PendingTilesContainer;

		TILES ApplyPendingTiles(Time time);
		// TODO: This is slow to update all tiles
		void UpdateTiles(Time time, TILES tiles);

		void TryUpdateTile(Time time, Math::Vector2Int pos, Tile tile);
	};
}