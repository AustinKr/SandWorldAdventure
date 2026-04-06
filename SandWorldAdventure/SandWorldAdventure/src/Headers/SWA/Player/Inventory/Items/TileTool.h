#pragma once
#include "GP2D/GraphicsTypes.h"
#include "SWA/Player/Inventory/Items/Item.h"

namespace SWA::Player::Inventory::Items
{
	// Allows you to place/remove the specified tiles
	struct TileTool : Item
	{
		GP2D::GP2D_HEX_COLOR Color; // Base color
		GP2D::GP2D_HEX_COLOR ColorDeviation; // Mixed with base color randomly

		unsigned int BehaviorUID;

		TileTool();
		TileTool(const char* textureName, GP2D::GP2D_HEX_COLOR color, GP2D::GP2D_HEX_COLOR colorDeviation, unsigned int behavior);

		virtual void TryUse(Inputs& rInputs, SWAEngine::Time time) override;
	};
}