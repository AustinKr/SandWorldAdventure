#pragma once
#include "GP2D/GraphicsTypes.h"
#include "SWA/Player/Inventory/BaseItem.h"

namespace SWA::Player::Inventory
{
	// Can be a tile or some other item
	struct TileItem : BaseItem
	{
		ItemTypes Type;
		const char* TextureName;
		GP2D::GP2D_HEX_COLOR Color; // Base color
		GP2D::GP2D_HEX_COLOR ColorDeviation; // Mixed with base color randomly

		unsigned int BehaviorUID;

		// Empty item (placeholder)
		TileItem();
		// Creates an item
		TileItem(const char* textureName, GP2D::GP2D_HEX_COLOR color, GP2D::GP2D_HEX_COLOR colorDeviation, unsigned int behavior);

		virtual void TryUse(Inputs& rInputs, SWAEngine::Time time) override;
	};
}