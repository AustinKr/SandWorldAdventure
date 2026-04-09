#pragma once
#include "SWA/Player/Inventory/Manager.h"
#include "SWAEngine/Inventory/BaseItem.h"

#include "GP2D/GUI/Components/Button/ButtonEventData.h"
#include "GP2D/GraphicsTypes.h"

#include "GP2D/GUI/Element.h"

namespace SWA::Player::Inventory
{
	// a static class that creates and manages the player's gui for the inventory
	struct GUILayout
	{
		GUILayout() = delete;

	public:
		typedef SWAEngine::Inventory::BaseItem ITEM;
		typedef SWAEngine::Inventory::Inventory INVENTORY;

		// Used to parent all gui elements related to inventory system into one group
		static GP2D::GP2D_UID s_InventoryElementUID;
		// A button that is used to open or close the inventory
		static GP2D::GP2D_UID s_InventoryToggleButtonUID;
		// Slots to show items
		static GP2D::GP2D_UID s_StorageSlotsElementUID;
		// Slots to show tools
		static GP2D::GP2D_UID s_ToolSlotsElementUID;

		// Multiplied with the scale of a slot
		static float m_SlotScaleFactor;

		// Subscribes to assignment events in the given player inventory system, and assigns the slots
		static void Initialize(Manager& rInventory);

		static bool IsActive();

	private:
		static GP2D::GP2D_UID m_EnabledStorageSlot;
		static GP2D::GP2D_UID m_EnabledToolSlot;

		static const char* BACKGROUND_TEXTURE_NAME;
		static const char* SHADER_NAME;
		static const char* DEFAULT_SLOT_TEXTURE;

		// Creates the layout for the inventory gui
		static void CreateBackgroundLayout();

		// Creates elements for each slot where items can be displayed
		static void AssignSlots(const GP2D::GP2D_UID group, INVENTORY& rInventory);
		static void CreateSlot(const GP2D::GP2D_UID group, INVENTORY& rInventory, SWAEngine::Math::Vector2Int location, const char* textureName);
		static GP2D::GP2D_UID GetSlot(const GP2D::GP2D_UID group, float width, SWAEngine::Math::Vector2Int location);
		static void SetSlotOutline(GP2D::GUI::Element& slot, GP2D::Math::Int2 thickness);

		// Called when an item is changed
		static void OnInventoryAssignment(const GP2D::GP2D_UID group, SWAEngine::Math::Vector2Int inventorySize, INVENTORY::ASSIGNMENT_EVENT_ARGS arguments);
		// Called when an item slot gui is clicked on
		static void OnItemSlotButtonClicked(const GP2D::GP2D_UID group, SWAEngine::Math::Vector2Int location, INVENTORY& rInventory);
		// Called when the inventory toggle button gui is clicked on
		static void OnToggleButtonClicked();
	};
}