#pragma once
#include "SWAEngine/Inventory/BaseItem.h"
#include "SWA/Player/Inventory/ItemTypes.h"
#include "SWAEngine/Time.h"
#include "SWA/Player/Inputs.h"

namespace SWA::Player::Inventory
{
	// Can be a tile or some other item
	struct PlayerItem : SWAEngine::Inventory::BaseItem
	{
		inline PlayerItem() : BaseItem() {}
		inline PlayerItem(ItemTypes type, const char* textureName) : BaseItem(type, textureName) {}

		inline virtual void TryUse(Inputs& rInputs, SWAEngine::Time time) {}
	};
}