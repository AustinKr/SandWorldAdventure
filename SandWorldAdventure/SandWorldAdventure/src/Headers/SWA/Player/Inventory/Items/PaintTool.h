#pragma once
#include "SWA/Player/Inventory/Items/UseableItem.h"

namespace SWA::Player::Inventory::Items
{
	// 
	struct PaintTool : UseableItem
	{

		virtual void TryUse(SWAEngine::Inventory::Inventory& rItemInventory, Inputs& rInputs, SWAEngine::Time time) override;
	};
}