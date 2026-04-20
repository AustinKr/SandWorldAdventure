#pragma once
#include <SWAEngine/GameObject/Component/IComponent.h>
#include "SWAEngine/Tilemap/ITilemapContainer.h"
#include "SWAEngine/Tilemap/PropertyManager.h"
#include "SWAEngine/Math/Time.h"
#include <unordered_map>

namespace SWAEngine::Tilemap
{
	// TODO: Move tilemap into GameObject/component
	struct SWA_ENGINE_API Tilemap : GameObject::Component::IComponent
	{
	public:
		static const int ACTIVE_TILES_ID; // Used in PropertyManager
		static const int PENDING_TILES_ID; // Used in PropertyManager
		static const unsigned int MAX_MOVE_STEPS;

		PropertyManager PropertyManager;

		// In world coordinates
		Math::Vector2 Origin;
		// In world coordinates
		Math::Vector2 TileScale;

		Tilemap(const char* name, Math::Vector2 origin = {}, Math::Vector2 scale = {1,1});

		virtual std::string const GetName() override;
		virtual void SetActive(bool state) override;
		virtual bool GetActive() override; // Returns true by default
		virtual void Update(std::string, Math::Time time) override;
		virtual void Release() override;
		bool IsEmpty();

		// Returns a copy of the active tile
		Tile GetActiveTile(Math::Vector2Int position);
		// Returns a copy of the pending tile or the currently active tile (pending prioritized)
		Tile GetTile(Math::Vector2Int position, int* containerID = nullptr);

		// Sets the tile (in the pending tiles container) and returns a copy
		// Note: does not automatically release the tile if failed for whatever reason
		Tile SetTile(Math::Vector2Int position, Tile tile);
		// Swaps the tiles.
		void SwapTiles(Math::Vector2Int a, Math::Vector2Int b);
		// Moves the tile in the given direction until it hits another active tile or the end
		// Use this function twice to move in two directions smoothly
		// Assumes the tile at origin exists and is active
		// Returns the new position, and the hit tile
		std::pair<Math::Vector2Int, Tile> TryStepMoveTile(Math::Vector2Int origin, Math::Vector2 movement, int maxSteps = MAX_MOVE_STEPS);

		// Detects collision of given rect and the active tiles
		bool DetectCollisionRect(Math::Vector2Int bottomLeft, Math::Vector2Int topRight);

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

		TILES ApplyPendingTiles(Math::Time time);
		// TODO: This is slow to update all tiles
		void UpdateTiles(Math::Time time, TILES tiles);

	};
}