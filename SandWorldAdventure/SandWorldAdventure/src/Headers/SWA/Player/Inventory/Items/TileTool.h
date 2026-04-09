#pragma once
#include "SWA/Player/Inventory/Items/UseableItem.h"
#include "SWA/Player/Inventory/Items/TileToolFlags.h"

namespace SWA::Player::Inventory::Items
{
	// Allows you to place/remove the specified tiles
	struct TileTool : UseableItem
	{
		TileToolFlags Flags;

		TileTool();
		TileTool(const char* textureName, TileToolFlags flags);

		virtual void TryUse(SWAEngine::Inventory::Inventory& rItemInventory, Inputs& rInputs, SWAEngine::Time time) override;
	};
}