#include "SWA/Player/PlayerInventoryGUI.h"
#include "SWA/RenderLayerNames.h"
#include "SWA/ShaderNames.h"

#include "GP2D/GUI/Hierarchy.h"
#include "GP2D/GUI/Components/Button/ButtonComponent.h"
#include "GP2D/GUI/Components/SpriteComponent.h"

#include "GP2D/Pipeline/GenericPipeline.h"
#include "GP2D/Pipeline/Camera.h"
#include <string>

using namespace GP2D::GUI;
using namespace GP2D::GUI::Components;
using namespace GP2D::GUI::Components::Button;

namespace SWA::Player
{
	unsigned long PlayerInventoryGUI::s_InventoryElementUID = NULL;
	unsigned long PlayerInventoryGUI::s_InventoryToggleButtonUID = NULL;
	unsigned long PlayerInventoryGUI::s_StorageSlotsElementUID = NULL;

	float PlayerInventoryGUI::m_SlotScaleFactor = 0.9f;

	const char* PlayerInventoryGUI::BACKGROUND_TEXTURE_NAME = "gui_background";
	const char* PlayerInventoryGUI::DEFAULT_SLOT_TEXTURE = "empty_slot";
	

	void PlayerInventoryGUI::Initialize(INVENTORY& rInventory)
	{
		// Create elements
		CreateBackgroundLayout();

		// Register for assignment of inventory
		rInventory.ResizeEventHandler.SubscribeEvent([&](auto) { AssignSlots(rInventory); });
		rInventory.AssignmentEventHandler.SubscribeEvent([&](auto args) {OnInventoryAssignment(rInventory, args); });

		AssignSlots(rInventory);
	}

	bool PlayerInventoryGUI::IsActive()
	{
		return Hierarchy::sp_ActiveInstance->GetElement(s_InventoryElementUID).GetActiveState();
	}


	void PlayerInventoryGUI::CreateBackgroundLayout()
	{
		// TODO: These elemnts really shouldn't scale with the screen- they should be defined with all screen coords

		auto pHierarchy = Hierarchy::sp_ActiveInstance;

		// Create empty group for inventory
		s_InventoryElementUID = pHierarchy->RegisterElement({ false })
			.SetTransform({ {20, 20}, false}, { {-40, -40 }, false, ALIGNMENT_RIGHT | ALIGNMENT_TOP})
			.GetIdentifier();

		// Create close button
		auto pButton = new ButtonComponent();
		s_InventoryToggleButtonUID = pHierarchy->RegisterElement({ true })
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
		s_StorageSlotsElementUID = pHierarchy->RegisterElement({ false }, backgroundUID)
			.SetTransform({ { .1, .1 }, true }, { { .8, .8 }, true })
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
	void PlayerInventoryGUI::CreateSlot(INVENTORY& rInventory, SWAEngine::Math::Vector2Int location, ITEM item)
	{
		auto& rInventoryElement = Hierarchy::sp_ActiveInstance->GetElement(s_InventoryElementUID);
		auto& rStorageElement = Hierarchy::sp_ActiveInstance->GetElement(s_StorageSlotsElementUID);

		// Encode item location into int value
		int extraFlags = (location.X << 8) | (location.Y);

		// space to offset
		float padding = (1.0f - m_SlotScaleFactor) / 2.0f;
		// the bottom left corner of the slot, starting from the top of the screen
		GP2D::Math::Float2 centeredPosition = GP2D::Math::Float2(
			(float)location.X + padding,
			-((float)location.Y + padding) - .5f);
		// get 0-1
		GP2D::Math::Float2 localPosition = centeredPosition / GP2D::Math::Float2(rInventory.GetSize().X, rInventory.GetSize().Y);

		// evenly divide inventory into slots, also scale with gui, and contain square-shaped slots
		float scaleX = 1.0f / (float)rInventory.GetSize().X * m_SlotScaleFactor; // evenly divided space

		// Create element
		auto pButton = new ButtonComponent(); // Create a button	
		auto& rElement = Hierarchy::sp_ActiveInstance->RegisterElement({ true }, s_StorageSlotsElementUID)
			.SetTransform({ localPosition, true, ALIGNMENT_TOP | ALIGNMENT_LEFT }, { {scaleX, 1}, true,  ALIGNMENT_BOTTOM | ALIGNMENT_LEFT | ASPECT_RATIO_WIDTH})
			.RegisterComponent(new SpriteComponent(item.TextureName == nullptr ? DEFAULT_SLOT_TEXTURE : item.TextureName, RENDERLAYERS_GUI, SWA_TEXTURE_SHADER))
			.RegisterComponent(pButton);
		pButton->ButtonEventHandler.Subscribe({ [&](auto data, auto) {OnItemSlotButtonClicked(rInventory, data); }, extraFlags });
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
		rInventory.SelectedItemPosition = { x, y };
	}
	void PlayerInventoryGUI::OnToggleButtonClicked()
	{
		auto& rElement = Hierarchy::sp_ActiveInstance->GetElement(s_InventoryElementUID);
		// Toggle and thus iterate through meshes and set active
		rElement.SetActiveState(!rElement.GetActiveState());
	}
}