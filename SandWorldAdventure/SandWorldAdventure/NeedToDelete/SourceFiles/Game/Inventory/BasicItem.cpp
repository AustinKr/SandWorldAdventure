#include "HeaderFiles/Game/Inventory/BasicItem.h"

namespace SandboxEngine::Game::Inventory
{
	// Empty item (placeholder)
	BasicItem::BasicItem() :
		ImagePath("C:/dev/C++ Projects/SandWorldAdventure/SandWorldAdventure/SandWorldAdventure/Resources/GUI/Slots/EmptySlot.bmp"),
		p_Data(nullptr)
	{

	}
	// Creates an item
	BasicItem::BasicItem(const char* imagePath, void* pData, int extraFlags) : ImagePath(imagePath), p_Data(pData), ExtraFlags(extraFlags)
	{

	}
	void BasicItem::Release()
	{
		if (p_Data != nullptr)
			delete(p_Data);
		p_Data = 0;
	}
}