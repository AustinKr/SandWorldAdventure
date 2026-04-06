#pragma once
#include "SWA/Player/Inventory/PlayerInventoryManager.h"
#include "SWA/Player/Inventory/PlayerItem.h"

#include "GP2D/GUI/Components/Button/ButtonEventData.h"
#include "GP2D/GraphicsTypes.h"

namespace SWA::Player::Inventory
{
	// a static class that creates and manages the player's gui for the inventory
	struct PlayerInventoryGUI
	{
		PlayerInventoryGUI() = delete;

	public:
		typedef PlayerItem ITEM;
		typedef SWAEngine::Inventory::Inventory INVENTORY;

		// Used to parent all gui elements related to inventory system into one group
		static GP2D::GP2D_UID s_InventoryElementUID;
		// A button that is used to open or close the inventory
		static GP2D::GP2D_UID s_InventoryToggleButtonUID;
		// Slots to show items
		static GP2D::GP2D_UID s_StorageSlotsElementUID;

		// Multiplied with the scale of a slot
		static float m_SlotScaleFactor;

		// Subscribes to assignment events in the given player inventory system, and assigns the slots
		static void Initialize(PlayerInventoryManager& rInventory);

		static bool IsActive();

	private:
		static const char* BACKGROUND_TEXTURE_NAME;
		static const char* SHADER_NAME;
		static const char* DEFAULT_SLOT_TEXTURE;

		// Creates the layout for the inventory gui
		static void CreateBackgroundLayout();

		// Creates elements for each slot where items can be displayed
		static void AssignSlots(GP2D::GP2D_UID group, INVENTORY& rInventory);
		static void CreateSlot(GP2D::GP2D_UID group, INVENTORY& rInventory, SWAEngine::Math::Vector2Int location, const char* textureName);

		// Called when an item is changed
		static void OnInventoryAssignment(SWAEngine::Math::Vector2Int inventorySize, INVENTORY::ASSIGNMENT_EVENT_ARGS arguments);
		// Called when an item slot gui is clicked on
		static void OnItemSlotButtonClicked(INVENTORY& rInventory, GP2D::GUI::Components::Button::ButtonEventData data);
		// Called when the inventory toggle button gui is clicked on
		static void OnToggleButtonClicked();
	};
}