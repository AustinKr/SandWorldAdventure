#include "Inventory/BasicItem.h"

namespace SWA::Inventory
{
	// Empty item (placeholder)
	BasicItem::BasicItem() :
		TextureName("NULL"),
		p_Data(nullptr),
		ExtraFlags(0)
	{

	}
	// Creates an item
	BasicItem::BasicItem(const char* imagePath, void* pData, int extraFlags) 
		: TextureName(imagePath), p_Data(pData), ExtraFlags(extraFlags)
	{

	}
	void BasicItem::Release()
	{
		if (p_Data != nullptr)
			delete(p_Data);
		p_Data = 0;
	}
}