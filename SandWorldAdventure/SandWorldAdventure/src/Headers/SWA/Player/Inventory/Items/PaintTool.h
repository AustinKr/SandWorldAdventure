#pragma once
#include "SWA/Player/Inventory/Items/UseableItem.h"
#include "GP2D/GraphicsTypes.h"

namespace SWA::Player::Inventory::Items
{
	// 
	struct PaintTool : UseableItem
	{
		GP2D::GP2D_HEX_COLOR Color;
		float Strength;
		int Radius;

		PaintTool();
		PaintTool(GP2D::GP2D_HEX_COLOR color, float strength, int radius);

		virtual void TryUse(SWAEngine::Inventory::Inventory& rItemInventory, Inputs& rInputs, SWAEngine::Time time) override;
	};
}