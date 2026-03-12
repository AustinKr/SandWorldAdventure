#include "SWA/Player/Item.h"

namespace SWA::Player
{
	Item::Item()
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