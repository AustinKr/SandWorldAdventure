#pragma once
#include <string>
#include "HeaderFiles/Game/Inventory/Inventory.h"

namespace SandboxEngine::Game::Inventory
{
	struct BasicItem
	{
		friend Inventory<BasicItem>;

	public:
		const char* ImagePath;
		void* p_Data;
		int ExtraFlags;

		// Empty item (placeholder)
		BasicItem();
		// Creates an item
		BasicItem(const char* imagePath, void* pData, int extraFlags);

	private:
		void Release();
	};
}