#pragma once
#include "GP2D/GraphicsTypes.h"
#include "SWA/Player/Inventory/Items/Item.h"

namespace SWA::Player::Inventory::Items
{
	// Can be a tile
	struct TileItem : Item
	{
		GP2D::GP2D_HEX_COLOR Color; // Base color
		GP2D::GP2D_HEX_COLOR ColorDeviation; // Mixed with base color randomly

		unsigned int BehaviorUID;

		TileItem();
		TileItem(const char* textureName, GP2D::GP2D_HEX_COLOR color, GP2D::GP2D_HEX_COLOR colorDeviation, unsigned int behavior);

		virtual void TryUse(Inputs& rInputs, SWAEngine::Time time) override;
	};
}