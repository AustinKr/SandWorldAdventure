#pragma once
#include "SWA/Player/Inventory/Items/Types.h"
#include "SWA/Player/Inputs.h"

#include "SWAEngine/Time.h"
#include "SWAEngine/Inventory/BaseItem.h"
#include "SWAEngine/Inventory/Inventory.h"

namespace SWA::Player::Inventory::Items
{
	// Can be a consumable, tool, or some other item that can be used by itself
	struct UseableItem : SWAEngine::Inventory::BaseItem
	{
		inline UseableItem() : BaseItem() {}
		inline UseableItem(Types type, const char* textureName) : BaseItem(type, textureName) {}

		inline virtual void TryUse(SWAEngine::Inventory::Inventory& rItemInventory, Inputs& rInputs, SWAEngine::Time time) {}
	};
}