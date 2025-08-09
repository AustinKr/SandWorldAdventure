#pragma once

#ifndef PLAYER_H
#define PLAYER_H
namespace SandboxEngine::Game::GameObject
{
	class Player;
}
#endif

namespace SandboxEngine::Game::Inventory
{
	struct PlayerInventoryGUI
	{
	private:
		static const char* BACKGROUND_BMP_DIRECTORY;

		GameObject::Player* mp_Player;

		// Creates the layout for the inventory gui
		void CreateBackgroundLayout();
	public:
		// Used to parent all gui elements related to inventory system into one group
		unsigned long InventoryElementID;
		// A button that is used to open or close the inventory
		unsigned long InventoryToggleButtonID;
		// Slots to show items
		unsigned long StorageSlotsElementID;

		// Size in pixels of square slot
		int SlotScale;
		// Pixels in between each slot
		int SlotPadding;


		bool IsActive();
		//// Deletes all gui
		//void EraseGUI();

		// Subscribes to assignment event in player.inventory
		PlayerInventoryGUI(GameObject::Player* pPlayer);
	};
}