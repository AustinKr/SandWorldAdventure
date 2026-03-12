#pragma once
#include "GP2D/Pipeline/GraphicsTypes.h"

namespace SWAEngine::Tilemap
{
	struct Tile
	{
		unsigned int BehaviorUID; // TODO: Create behavior registry system
		GP2D::Pipeline::GP2D_HEX_COLOR Color;

		// Whether this tile is active and non-null
		bool HasValue;
	};
}