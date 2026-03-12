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
		GP2D::Pipeline::GP2D_HEX_COLOR Color;
		unsigned int BehaviorUID;

		// Empty item (placeholder)
		Item();
		// Creates an item
		Item(ItemTypes type, const char* textureName, GP2D::Pipeline::GP2D_HEX_COLOR color, unsigned int behavior);

		void Release();
	};
}