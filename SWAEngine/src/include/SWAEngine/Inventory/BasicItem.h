#pragma once
#include "SWAEngine/dllClause.h"

namespace SWAEngine::Inventory
{
	struct SWA_ENGINE_API BasicItem
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