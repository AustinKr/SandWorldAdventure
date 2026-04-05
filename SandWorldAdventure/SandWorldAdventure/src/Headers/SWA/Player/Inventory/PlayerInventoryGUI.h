#pragma once
#include "SWAEngine/Inventory/Inventory.h"
#include "SWA/Player/Inventory/TileItem.h"

#include "GP2D/GUI/Components/Button/ButtonEventData.h"

namespace SWA::Player::Inventory
{
	// a static class that creates and manages the player's gui for the inventory
	struct PlayerInventoryGUI
	{
		PlayerInventoryGUI() = delete;

	public:
		typedef TileItem ITEM;
		typedef SWAEngine::Inventory::Inventory<ITEM> INVENTORY;

		// Used to parent all gui elements related to inventory system into one group
		static unsigned long s_InventoryElementUID;
		// A button that is used to open or close the inventory
		static unsigned long s_InventoryToggleButtonUID;
		// Slots to show items
		static unsigned long s_StorageSlotsElementUID;

		// Multiplied with the scale of a slot
		static float m_SlotScaleFactor;

		// Subscribes to assignment event in the given player inventory, and assigns the slots
		static void Initialize(INVENTORY& rInventory);

		static bool IsActive();

	private:
		static const char* BACKGROUND_TEXTURE_NAME;
		static const char* SHADER_NAME;
		static const char* DEFAULT_SLOT_TEXTURE;

		// Creates the layout for the inventory gui
		static void CreateBackgroundLayout();

		// Creates elements for each slot where items can be displayed
		static void AssignSlots(INVENTORY& rInventory);
		static void CreateSlot(INVENTORY& rInventory, SWAEngine::Math::Vector2Int location, ITEM item);

		// Called when an item is changed
		static void OnInventoryAssignment(INVENTORY& rInventory, INVENTORY::ASSIGNMENT_EVENT_ARGS arguments);
		// Called when an item slot gui is clicked on
		static void OnItemSlotButtonClicked(INVENTORY& rInventory, GP2D::GUI::Components::Button::ButtonEventData data);
		// Called when the inventory toggle button gui is clicked on
		static void OnToggleButtonClicked();
	};
}