#pragma once

namespace SWA::Inventory
{
	struct BasicItem
	{
		const char* TextureName;
		void* p_Data;
		int ExtraFlags;

		// Empty item (placeholder)
		BasicItem();
		// Creates an item
		BasicItem(const char* TextureName, void* pData, int extraFlags);

		void Release();
	};
}