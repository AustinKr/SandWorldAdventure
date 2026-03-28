#pragma once
#include "SWAEngine/dllclause.h"
#include "SWAEngine/Tilemap/Tile.h"
#include "SWAEngine/Math/Int3.h"
#include <utility>
#include <unordered_map>
#include <set>

namespace SWAEngine::Tilemap::TilePropertyManager
{
	// Stores the property data of tiles.
	// Also contains functions to manage different kinds of properties stored in a Tile.
	// TODO: Move out of namespace just to Tilemap
	struct SWA_ENGINE_API PropertyManager
	{
		// First: The allocated memory for a property struct.
		// Second: The tiles that share this property
		// (first two ints are the X and Y coordinates, the last is the container ID)
		std::unordered_map<void*, std::set<Math::Int3>> DataContainer;

		void TryEraseData(void* data, Math::Int3 tileLocation);

		// TODO: These helper functions should probably be deleteded
		template<typename TYPE>
		static inline Tile&& CreateNew(unsigned int behaviorID, GP2D::GP2D_HEX_COLOR color, TYPE properties)  // Allocates data in new tile. You are responsible for releasing it
		{
			Tile newTile = { behaviorID, color, true };
			newTile.p_Properties = new TYPE(properties);
			return std::move(newTile);
		}
		template<typename TYPE>
		static inline void Set(Tile& rTile, TYPE properties) // Sets properties of tile
		{
			*static_cast<TYPE*>(rTile.p_Properties) = properties;
		}	
		template<typename TYPE>
		static inline TYPE Get(Tile& rTile) // Parses properties of tile and returns a copy
		{
			return *static_cast<TYPE*>(rTile.p_Properties);
		}
	};
}

