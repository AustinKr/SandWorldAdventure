#include "SWAEngine/Inventory/BaseItem.h"

namespace SWAEngine::Inventory
{
	// Empty item (placeholder)
	BaseItem::BaseItem() :
		Type(0), TextureName(nullptr)
	{

	}
	// Creates an item
	BaseItem::BaseItem(unsigned int type, const char* textureName) :
		Type(type), TextureName(textureName)
	{

	}
	void BaseItem::Release()
	{
		delete(this);
	}
}