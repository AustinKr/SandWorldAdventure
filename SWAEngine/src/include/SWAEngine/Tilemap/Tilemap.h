#pragma once
#include "SWAEngine/Tilemap/ITilemapContainer.h"
#include "SWAEngine/Time.h"
#include <unordered_map>
#include <optional>

namespace SWAEngine::Tilemap
{
	struct SWA_ENGINE_API Tilemap // TODO: Could make tilemap a game object
	{
	public:
		static const unsigned int MAX_MOVE_STEPS;

		// In world coordinates
		Math::Vector2 Origin;
		// In world coordinates
		Math::Vector2 TileScale;

		Tilemap(Math::Vector2 origin = {}, Math::Vector2 scale = {1,1});
		void Release();
		bool IsEmpty();

		// Returns a copy of the active tile
		Tile GetActiveTile(Math::Vector2Int position);
		// Returns a copy of the currently active tile or the pending tile
		Tile GetTile(Math::Vector2Int position);

		// Sets the tile (in the pending tiles container)
		Tile& SetTile(Math::Vector2Int position, Tile tile);
		// Swaps the active tiles. (optionally uses the given tiles in place of a and b)
		void SwapTiles(Math::Vector2Int a, Math::Vector2Int b, std::optional<Tile> tileA = std::nullopt, std::optional<Tile> tileB = std::nullopt);
		// Moves the tile in the given direction until it hits another active tile or the end
		// Use this function twice to move in two directions smoothly
		// Assumes the tile at origin exists and is active
		bool TryStepMoveTile(Math::Vector2Int origin, Math::Vector2 movement, int maxSteps = MAX_MOVE_STEPS);

		// Detects collision of given rect and the active tiles
		bool DetectCollisionRect(Math::Vector2Int bottomLeft, Math::Vector2Int topRight);

		// Should be called every frame cycle
		void Update(Time time);

		Math::Vector2 TileToWorld(Math::Vector2Int tile, bool applyOffsets = true);
		Math::Vector2Int WorldToTile(Math::Vector2 world, bool applyOffsets = true);

		// Returns the bounds of the map in tiles
		// (the top-right-most tile position)
		Math::Vector2Int GetBounds();
		bool IsInBounds(Math::Vector2Int tile);
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

	};
}