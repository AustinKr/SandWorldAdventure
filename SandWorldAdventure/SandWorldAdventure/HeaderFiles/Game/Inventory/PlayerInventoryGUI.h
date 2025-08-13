#pragma once
#include "HeaderFiles/Game/Inventory/Inventory.h"
#include "HeaderFiles/Game/Inventory/BasicItem.h"
#include "HeaderFiles/GUISystem/ButtonEventData.h"

#ifndef GUIELEMENT_H
#define GUIELEMENT_H
namespace SandboxEngine::GUISystem
{
	class GUIElement;
}
#endif
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

		// Creates elements for each slot where items can be displayed
		void AssignSlots();
		void CreateSlot(GUISystem::GUIElement* pStorageElement, Vector2Int location, BasicItem& rItem);

		// Called when the size of the inventory changes
		static void OnInventoryAssignment(Vector2Int oldSize, void*);
		// Called when an item is changed
		static void OnItemAssignment(ItemAssignmentEventArguments<BasicItem> arguments, void*);
		// Called when an item slot gui is clicked on
		static void OnItemSlotButtonClicked(void*, GUISystem::ButtonEventData);
		// Called when the inventory toggle button gui is clicked on
		static void OnToggleButtonClicked(void*, GUISystem::ButtonEventData);

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