#pragma once
#include "GP2D/GraphicsTypes.h"
#include "SWAEngine/Math/vector2.h"

namespace SWAEngine::Tilemap
{
	struct Tile
	{
		unsigned int BehaviorUID;
		GP2D::GP2D_HEX_COLOR Color;

		// Whether this tile is active and non-null
		bool HasValue;


		// TODO: propreties should be in behavior and behaivor should be individual
		float LastUpdateTime; // Used for physics

		Math::Vector2 Velocity;

		inline Tile() 
			: BehaviorUID(0), Color(0x0), HasValue(false), LastUpdateTime(0), Velocity{}
		{

		}
		inline Tile(unsigned int uid, GP2D::GP2D_HEX_COLOR color, bool hasValue, float time = 0)
			: BehaviorUID(uid), Color(color), HasValue(hasValue), LastUpdateTime(time), Velocity{}
		{

		}
	};
}