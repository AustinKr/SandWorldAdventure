#pragma once
#include "SWAEngine/Math.h"

namespace SWAEngine::Tilemap
{
	struct Tile
	{
		unsigned int BehaviorID; // TODO: Create behavior registry system
		HEX_COLOR Color;

		// Whether this tile is active and non-null
		bool HasValue;
	};
}