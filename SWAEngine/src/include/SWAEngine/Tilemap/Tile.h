#pragma once
#include "GP2D/Pipeline/GraphicsTypes.h"

namespace SWAEngine::Tilemap
{
	struct Tile
	{
		unsigned int BehaviorUID;
		GP2D::Pipeline::GP2D_HEX_COLOR Color;

		// Whether this tile is active and non-null
		bool HasValue;

		float LastUpdateTime; // Used for physics

		inline Tile() 
			: BehaviorUID(0), Color(0x0), HasValue(false), LastUpdateTime(0)
		{

		}
		inline Tile(unsigned int uid, GP2D::Pipeline::GP2D_HEX_COLOR color, bool hasValue, float time = 0)
			: BehaviorUID(uid), Color(color), HasValue(hasValue), LastUpdateTime(time)
		{

		}
	};
}