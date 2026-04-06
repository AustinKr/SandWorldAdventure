#pragma once
#include "SWAEngine/Inventory/BaseItem.h"
#include "SWA/Player/Inventory/Types.h"
#include "SWAEngine/Time.h"
#include "SWA/Player/Inputs.h"

namespace SWA::Player::Inventory::Items
{
	// Can be a tile or some other item
	struct Item : SWAEngine::Inventory::BaseItem
	{
		inline Item() : BaseItem() {}
		inline Item(Types type, const char* textureName) : BaseItem(type, textureName) {}

		inline virtual void TryUse(Inputs& rInputs, SWAEngine::Time time) {}
	};
}