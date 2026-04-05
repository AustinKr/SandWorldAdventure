#pragma once
#include "SWA/Player/Inventory/ItemTypes.h"
#include "SWAEngine/Time.h"
#include "SWA/Player/Inputs.h"

namespace SWA::Player::Inventory
{
	// Can be a tile or some other item
	struct BaseItem
	{
		ItemTypes Type;
		const char* TextureName;

		// Empty item (placeholder)
		inline BaseItem()
			: Type(PLAYER_ITEM_TYPE_EMPTY), TextureName(nullptr)
		{

		}
		// Creates an item
		inline BaseItem(ItemTypes type, const char* textureName)
			: Type(type), TextureName(textureName)
		{

		}

		inline virtual void Release() {}
		inline virtual void TryUse(Inputs& rInputs, SWAEngine::Time time) {}
	};
}