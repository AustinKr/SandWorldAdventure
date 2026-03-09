#include "Player/PlayerInventoryGUI.h"
#include "RenderLayerNames.h"
#include "ShaderNames.h"

#include "GP2D/GUI/Hierarchy.h"
#include "GP2D/GUI/Components/Button/ButtonComponent.h"
#include "GP2D/GUI/Components/SpriteComponent.h"

using namespace GP2D::GUI;
using namespace GP2D::GUI::Components;
using namespace GP2D::GUI::Components::Button;

namespace SWA::Player
{
	unsigned long PlayerInventoryGUI::s_InventoryElementUID = NULL;
	unsigned long PlayerInventoryGUI::s_InventoryToggleButtonUID = NULL;
	unsigned long PlayerInventoryGUI::s_StorageSlotsElementUID = NULL;

	int PlayerInventoryGUI::s_SlotScale = 40;
	int PlayerInventoryGUI::s_SlotPadding = 15;

	const char* PlayerInventoryGUI::BACKGROUND_TEXTURE_NAME = "gui_background";


	void PlayerInventoryGUI::Initialize(INVENTORY& rInventory)
	{
		// Register for assignment of inventory
		rInventory.ResizeEventHandler.SubscribeEvent([&](auto) { AssignSlots(rInventory); });
		rInventory.AssignmentEventHandler.SubscribeEvent([&](auto args) {OnInventoryAssignment(rInventory, args); });

		// Create elements
		CreateBackgroundLayout();
	}

	bool PlayerInventoryGUI::IsActive()
	{
		return Hierarchy::sp_ActiveInstance->GetElement(s_InventoryElementUID).GetActiveState();
	}


	void PlayerInventoryGUI::CreateBackgroundLayout()
	{
		auto pHierarchy = Hierarchy::sp_ActiveInstance;

		// Create empty group for inventory
		s_InventoryElementUID = pHierarchy->RegisterElement({ false })
			.SetTransform({ {0.05, 0.05}, true, }, { {.9, .9 }, true })
			.GetIdentifier();

		// Create close button
		auto pButton = new ButtonComponent();
		s_InventoryToggleButtonUID = pHierarchy->RegisterElement({ true }, s_InventoryElementUID)
			.SetTransform({ { -0.1, -0.1}, true, ALIGNMENT_TOP | ALIGNMENT_CENTER_HORIZONTAL }, { { .2, 0.075 }, true })
			.RegisterComponent(new SpriteComponent(BACKGROUND_TEXTURE_NAME, RENDERLAYERS_GUI, SWA_TEXTURE_SHADER))
			.RegisterComponent(pButton)
			.GetIdentifier();
		// this must be done after the button is initialized
		pButton->ButtonEventHandler.Subscribe({
			[](auto, auto) {OnToggleButtonClicked(); } });

		// Create menus background
		pHierarchy->RegisterElement({ true }, s_InventoryElementUID)
			.SetTransform({ { 0.0, 0.0}, true }, { { .3, 1.0 }, true })
			.RegisterComponent(new SpriteComponent(BACKGROUND_TEXTURE_NAME, RENDERLAYERS_GUI, SWA_TEXTURE_SHADER));
			

		// Create storage background
		unsigned long backgroundUID = pHierarchy->RegisterElement({ true }, s_InventoryElementUID)
			.SetTransform({ { -.3, 0.0 }, true, ALIGNMENT_RIGHT | ALIGNMENT_BOTTOM }, { { .3, 1.0 }, true })
			.RegisterComponent(new SpriteComponent(BACKGROUND_TEXTURE_NAME, RENDERLAYERS_GUI, SWA_TEXTURE_SHADER))
			.GetIdentifier();

		// Create storage group parented to the storage background
		s_StorageSlotsElementUID = pHierarchy->RegisterElement({ false }, s_InventoryElementUID)
			.SetTransform({ { .05, 0.05 }, true }, { { .9, .9 }, true })
			.GetIdentifier();
	}

	void PlayerInventoryGUI::AssignSlots(INVENTORY& rInventory)
	{
		if (s_StorageSlotsElementUID == NULL)
			return;

		// Clear elements
		Hierarchy::sp_ActiveInstance->EraseChildren(s_StorageSlotsElementUID);

		// Create new elements for each item slot
		int width = rInventory.GetSize().X, height = rInventory.GetSize().Y;
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				CreateSlot(rInventory, { w,h }, *(rInventory.GetBegin() + w + h * width));
			}
		}
	}
	void PlayerInventoryGUI::CreateSlot(INVENTORY& rInventory, SWAEngine::Math::Vector2Int location, ITEM& rItem)
	{
		auto& rInventoryElement = Hierarchy::sp_ActiveInstance->GetElement(s_InventoryElementUID);
		auto& rStorageElement = Hierarchy::sp_ActiveInstance->GetElement(s_StorageSlotsElementUID);

		// Encode item location into int value
		int extraFlags = (location.X << 8) | (location.Y);
		
		// Create a button
		auto pButton = new ButtonComponent();
		pButton->ButtonEventHandler.Subscribe({ [&](auto data, auto) {OnItemSlotButtonClicked(rInventory, data); }, extraFlags });
		
		// Create element
		auto& rElement = Hierarchy::sp_ActiveInstance->RegisterElement({ true }, s_StorageSlotsElementUID)
			.RegisterComponent(new SpriteComponent(rItem.TextureName, RENDERLAYERS_GUI, SWA_TEXTURE_SHADER))
			.RegisterComponent(pButton);
		
		// the bottom left corner of the slot, starting from the top of the screen
		GP2D::Math::Float2 position = GP2D::Math::Float2(
			(float)location.X / rInventory.GetSize().X,
			(float)(-location.Y - 1) / rInventory.GetSize().Y);
		// Set transform
		rElement.SetTransform({ position, true, ALIGNMENT_TOP | ALIGNMENT_LEFT }, { GP2D::Math::Float2(s_SlotScale, s_SlotScale), false });
	}

	void PlayerInventoryGUI::OnInventoryAssignment(INVENTORY& rInventory, INVENTORY::ASSIGNMENT_EVENT_ARGS arguments)
	{
		// Update item mesh

		int itemID = rInventory.GetSize().X * arguments.first.Y + arguments.first.X;
		auto& rStorageElement = Hierarchy::sp_ActiveInstance->GetElement(s_StorageSlotsElementUID);
		auto& rSlotElement = Hierarchy::sp_ActiveInstance->GetElement(rStorageElement.Children.at(itemID));

		auto pSprite = static_cast<SpriteComponent*>(rSlotElement.GetComponent(COMPONENT_TAG_SPRITE));
		pSprite->m_TextureName = arguments.second.TextureName;

	}
	void PlayerInventoryGUI::OnItemSlotButtonClicked(INVENTORY& rInventory, ButtonEventData data)
	{
		char x = data.ExtraFlags >> 8, y = data.ExtraFlags & 0xff; // Decode integer to get item id
		rInventory.SelectedItemID = { x, y };
	}
	void PlayerInventoryGUI::OnToggleButtonClicked()
	{
		auto& rElement = Hierarchy::sp_ActiveInstance->GetElement(s_InventoryElementUID);
		fprintf(stdout, "Toggle!\n");
		// Toggle and thus iterate through meshes and set active
		rElement.SetActiveState(!rElement.GetActiveState());
	}
}