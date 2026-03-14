#include "SWA/Player/Item.h"

namespace SWA::Player
{
	Item::Item()
		: Type(PLAYER_ITEM_TYPE_EMPTY), TextureName(nullptr), Color(0x0), BehaviorUID(0)
	{

	}
	Item::Item(ItemTypes type, const char* textureName, GP2D::Pipeline::GP2D_HEX_COLOR color, unsigned int behavior)
		: Type(type), TextureName(textureName), Color(color), BehaviorUID(behavior)
	{

	}

	void Item::Release()
	{

	}
}