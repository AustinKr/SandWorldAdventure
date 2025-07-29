#pragma once

namespace SandboxEngine::Game::Inventory
{
	struct BasicItem
	{
		const char* ImagePath;
		void* p_Data;

		// Empty item (placeholder)
		inline BasicItem() : ImagePath("C:/dev/C++ Projects/SandWorldAdventure/SandWorldAdventure/SandWorldAdventure/Resources/GUI/Lava24.bmp"), p_Data(nullptr)
		{

		}
		// Creates an item
		inline BasicItem(char* imagePath, void* pData) : ImagePath(imagePath), p_Data(pData)
		{
			
		}
	};
}