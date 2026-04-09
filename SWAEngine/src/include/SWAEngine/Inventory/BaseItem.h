#pragma once
#include "SWAEngine/dllClause.h"

namespace SWAEngine::Inventory
{
	// Should be created by Inventory::SetItemAt(pos, new MyItem())
	struct SWA_ENGINE_API BaseItem
	{
		unsigned int Type;
		const char* TextureName;
		// TODO: Add a stack count to items e.g. unsigned int StackCount;

		// Empty item (placeholder)
		BaseItem();
		// Creates an item
		BaseItem(unsigned int type, const char* textureName);

		virtual void Release();
	};
}