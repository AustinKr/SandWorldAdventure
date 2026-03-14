#pragma once
#include "GP2D/Pipeline/GraphicsTypes.h"
#include "SWA/Player/ItemTypes.h"

namespace SWA::Player
{
	// Can be a tile or some other item
	struct Item
	{
		ItemTypes Type;
		const char* TextureName;
		GP2D::Pipeline::GP2D_HEX_COLOR Color; // Base color
		GP2D::Pipeline::GP2D_HEX_COLOR ColorDeviation; // Mixed with base color randomly

		unsigned int BehaviorUID;

		// Empty item (placeholder)
		inline Item() 
			: Type(PLAYER_ITEM_TYPE_EMPTY), TextureName(nullptr), Color(0x0), ColorDeviation(0x0), BehaviorUID(0)
		{

		}
		// Creates an item
		inline Item(ItemTypes type, const char* textureName, GP2D::Pipeline::GP2D_HEX_COLOR color, GP2D::Pipeline::GP2D_HEX_COLOR colorDeviation, unsigned int behavior)
			: Type(type), TextureName(textureName), Color(color), ColorDeviation(colorDeviation), BehaviorUID(behavior)
		{

		}

		inline void Release()
		{

		}
	};
}