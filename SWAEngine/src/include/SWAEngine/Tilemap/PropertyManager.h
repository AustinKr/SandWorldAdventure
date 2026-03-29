#pragma once
#include "SWAEngine/dllclause.h"
#include "SWAEngine/Math/Int3.h"
#include <unordered_map>
#include <set>

namespace SWAEngine::Tilemap
{
	// Stores the property data of tiles.
	// Also contains functions to manage different kinds of properties stored in a Tile.
	// TODO: Move out of namespace just to Tilemap
	struct SWA_ENGINE_API PropertyManager
	{
		// TODO: I think this could be an unoreded set
		// First: The allocated memory for a property struct.
		// Second: The tiles that share this property
		// (first two ints are the X and Y coordinates, the last is the container ID)
		std::unordered_map<void*, std::set<Math::Int3, Math::Int3Compare>> DataContainer;

		void TryEraseData(void* data, Math::Int3 tileLocation, bool shouldRelease = true);
	};
}

