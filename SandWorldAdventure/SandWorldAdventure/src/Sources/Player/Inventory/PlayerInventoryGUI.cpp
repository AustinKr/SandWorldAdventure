#include "SWA/Player/Inventory/PlayerInventoryGUI.h"
#include "SWA/RenderLayerNames.h"

#include "GP2D/GUI/Hierarchy.h"
#include "GP2D/GUI/Components/Button/ButtonComponent.h"
#include "GP2D/GUI/Components/SpriteComponent.h"

#include "GP2D/Pipeline/GenericPipeline.h"
#include "GP2D/Pipeline/Camera.h"
#include <string>

using namespace GP2D::GUI;
using namespace GP2D::GUI::Components;
using namespace GP2D::GUI::Components::Button;

namespace SWA::Player::Inventory
{
	unsigned long PlayerInventoryGUI::s_InventoryElementUID = NULL;
	unsigned long PlayerInventoryGUI::s_InventoryToggleButtonUID = NULL;
	unsigned long PlayerInventoryGUI::s_StorageSlotsElementUID = NULL;

	float PlayerInventoryGUI::m_SlotScaleFactor = 0.9f;

	const char* PlayerInventoryGUI::BACKGROUND_TEXTURE_NAME = "gui_background";
	const char* PlayerInventoryGUI::SHADER_NAME = "DefaultSpriteShader";
	const char* PlayerInventoryGUI::DEFAULT_SLOT_TEXTURE = "empty_slot";


	void PlayerInventoryGUI::Initialize(PlayerInventoryManager& rInventory)
	{
		// Create elements
		CreateBackgroundLayout();

		// Register for assignment of inventory
		rInventory.ItemInventory.ResizeEventHandler.SubscribeEvent([&](auto) { AssignSlots(s_StorageSlotsElementUID, rInventory.ItemInventory); });
		rInventory.ItemInventory.AssignmentEventHandler.SubscribeEvent([&](auto args) {OnInventoryAssignment(rInventory.ItemInventory.GetSize(), args); });

		AssignSlots(s_StorageSlotsElementUID, rInventory.ItemInventory);
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
			.SetTransform({ {20, 20}, false }, { {-40, -40 }, false, ALIGNMENT_RIGHT | ALIGNMENT_TOP })
			.GetIdentifier();

		// Create close button
		auto pButton = new ButtonComponent();
		s_InventoryToggleButtonUID = pHierarchy->RegisterElement({ true })
			.SetTransform({ { -0.1, -0.1}, true, ALIGNMENT_TOP | ALIGNMENT_CENTER_HORIZONTAL }, { { .2, 0.075 }, true })
			.RegisterComponent(new SpriteComponent(BACKGROUND_TEXTURE_NAME, SHADER_NAME, RENDERLAYERS_GUI))
			.RegisterComponent(pButton)
			.GetIdentifier();
		// this must be done after the button is initialized
		pButton->ButtonEventHandler.Subscribe({
			[](auto, auto) {OnToggleButtonClicked(); } });


		// Empty object to contain equippment and tools
		unsigned long equippmentGroupID = pHierarchy->RegisterElement({ true }, s_InventoryElementUID)
			.SetTransform({ { 0, 0}, false }, { { 200, 0}, false, ALIGNMENT_LEFT | ALIGNMENT_TOP })
			.GetIdentifier();
		// Create equippment background
		pHierarchy->RegisterElement({ true }, equippmentGroupID)
			.SetTransform({ { 0.0, 0.5}, true}, { { 1.0, 0.5}, true})
			.RegisterComponent(new SpriteComponent(BACKGROUND_TEXTURE_NAME, SHADER_NAME, RENDERLAYERS_GUI));
		// Create tools background
		pHierarchy->RegisterElement({ true }, equippmentGroupID)
			.SetTransform({ { 0.0, 0.0}, true }, { { 1.0, 0.5}, true })
			.RegisterComponent(new SpriteComponent(BACKGROUND_TEXTURE_NAME, SHADER_NAME, RENDERLAYERS_GUI));

		// Create storage background
		unsigned long backgroundUID = pHierarchy->RegisterElement({ true }, s_InventoryElementUID)
			.SetTransform({ { -200, 0}, false, ALIGNMENT_RIGHT | ALIGNMENT_BOTTOM }, { { 200, 0}, false, ALIGNMENT_LEFT | ALIGNMENT_TOP })
			.RegisterComponent(new SpriteComponent(BACKGROUND_TEXTURE_NAME, SHADER_NAME, RENDERLAYERS_GUI))
			.GetIdentifier();

		// Create storage group parented to the storage background
		s_StorageSlotsElementUID = pHierarchy->RegisterElement({ false }, backgroundUID)
			.SetTransform({ { .1, .1 }, true }, { { .8, .8 }, true })
			.GetIdentifier();
	}

	void PlayerInventoryGUI::AssignSlots(GP2D::GP2D_UID group, INVENTORY& rInventory)
	{
		if (group == NULL)
			return;

		// Clear any elements
		Hierarchy::sp_ActiveInstance->EraseChildren(group);

		// Create new elements for each item slot
		int width = rInventory.GetSize().X, height = rInventory.GetSize().Y;
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				// Get texture
				PlayerItem* currentItem = static_cast<PlayerItem*>(*(rInventory.GetBegin() + w + h * width));
				const char* textureName = 
					currentItem == nullptr ? DEFAULT_SLOT_TEXTURE
					: currentItem->TextureName == nullptr ? DEFAULT_SLOT_TEXTURE
					: currentItem->TextureName;

				CreateSlot(group, rInventory, { w,h }, textureName);
			}
		}
	}
	void PlayerInventoryGUI::CreateSlot(GP2D::GP2D_UID group, INVENTORY& rInventory, SWAEngine::Math::Vector2Int location, const char* textureName)
	{
		SWAEngine::Math::Vector2Int inventorySize = rInventory.GetSize();

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
		GP2D::Math::Float2 localPosition = centeredPosition / GP2D::Math::Float2(inventorySize.X, inventorySize.Y);

		// evenly divide inventory into slots, also scale with gui, and contain square-shaped slots
		float scaleX = 1.0f / (float)inventorySize.X * m_SlotScaleFactor; // evenly divided space

		// Create element
		auto pButton = new ButtonComponent(); // Create a button	
		auto& rElement = Hierarchy::sp_ActiveInstance->RegisterElement({ true }, s_StorageSlotsElementUID)
			.SetTransform({ localPosition, true, ALIGNMENT_TOP | ALIGNMENT_LEFT }, { {scaleX, 1}, true,  ALIGNMENT_BOTTOM | ALIGNMENT_LEFT | ASPECT_RATIO_WIDTH })
			.RegisterComponent(new SpriteComponent(textureName, SHADER_NAME, RENDERLAYERS_GUI))
			.RegisterComponent(pButton);
		pButton->ButtonEventHandler.Subscribe({ [&](auto data, auto) {OnItemSlotButtonClicked(rInventory, data); }, extraFlags });
	}

	void PlayerInventoryGUI::OnInventoryAssignment(SWAEngine::Math::Vector2Int inventorySize, INVENTORY::ASSIGNMENT_EVENT_ARGS arguments)
	{
		// TODO: This wont always be the item in the storage group

		// Update item mesh

		int itemID = inventorySize.X * arguments.first.Y + arguments.first.X;
		auto& rStorageElement = Hierarchy::sp_ActiveInstance->GetElement(s_StorageSlotsElementUID);
		auto& rSlotElement = Hierarchy::sp_ActiveInstance->GetElement(rStorageElement.Children.at(itemID));

		auto pSprite = static_cast<SpriteComponent*>(rSlotElement.GetComponent(COMPONENT_TAG_SPRITE));
		pSprite->TextureName = arguments.second->TextureName;

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