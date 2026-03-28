#pragma once
#include "GP2D/GraphicsTypes.h"
#include "SWAEngine/Math/vector2.h"
#include <utility>

namespace SWAEngine::Tilemap
{
	struct Tile
	{
		unsigned int BehaviorUID;
		GP2D::GP2D_HEX_COLOR Color;
		bool HasValue; // Whether this tile is active and non-null
		void* p_Properties; // The allocated memory that stores this tile's properties

		inline Tile() 
			: BehaviorUID(0), Color(0x0), HasValue(false), p_Properties(nullptr)
		{

		}
		inline Tile(unsigned int uid, GP2D::GP2D_HEX_COLOR color, bool hasValue)
			: BehaviorUID(uid), Color(color), HasValue(hasValue), p_Properties(nullptr)
		{

		}
	};
}