#include "SWA/Player/Inventory/GUILayout.h"
#include "SWA/RenderLayerNames.h"
#include "SWA/SpriteShaderProperties.h"

#include "GP2D/GUI/Hierarchy.h"
#include "GP2D/GUI/Components/Button/ButtonComponent.h"
#include "GP2D/GUI/Components/SpriteComponent.h"

using namespace GP2D::GUI;
using namespace GP2D::GUI::Components;
using namespace GP2D::GUI::Components::Button;

namespace SWA::Player::Inventory
{
	GP2D::GP2D_UID GUILayout::s_InventoryElementUID = NULL;
	GP2D::GP2D_UID GUILayout::s_InventoryToggleButtonUID = NULL;
	GP2D::GP2D_UID GUILayout::s_StorageSlotsElementUID = NULL;
	GP2D::GP2D_UID GUILayout::s_ToolSlotsElementUID = NULL;
	GP2D::GP2D_UID GUILayout::m_EnabledStorageSlot = NULL;
	GP2D::GP2D_UID GUILayout::m_EnabledToolSlot = NULL;

	float GUILayout::m_SlotScaleFactor = 0.9f;

	const char* GUILayout::BACKGROUND_TEXTURE_NAME = "gui_background";
	const char* GUILayout::SHADER_NAME = "DefaultSpriteShader";
	const char* GUILayout::DEFAULT_SLOT_TEXTURE = "empty_slot";


	void GUILayout::Initialize(Manager& rInventory)
	{
		// Create elements
		CreateBackgroundLayout();

		// Register for assignment of inventories
		rInventory.StorageInventory.ResizeEventHandler.SubscribeEvent([&](auto) { AssignSlots(s_StorageSlotsElementUID, rInventory.StorageInventory); });
		rInventory.StorageInventory.AssignmentEventHandler.SubscribeEvent([&](auto args) {OnInventoryAssignment(s_StorageSlotsElementUID, rInventory.StorageInventory.GetSize(), args); });

		rInventory.ToolInventory.ResizeEventHandler.SubscribeEvent([&](auto) { AssignSlots(s_ToolSlotsElementUID, rInventory.ToolInventory); });
		rInventory.ToolInventory.AssignmentEventHandler.SubscribeEvent([&](auto args) {OnInventoryAssignment(s_ToolSlotsElementUID, rInventory.ToolInventory.GetSize(), args); });

		AssignSlots(s_StorageSlotsElementUID, rInventory.StorageInventory);
		AssignSlots(s_ToolSlotsElementUID, rInventory.ToolInventory);
	}

	bool GUILayout::IsActive()
	{
		return Hierarchy::sp_ActiveInstance->GetElement(s_InventoryElementUID).GetActiveState();
	}


	void GUILayout::CreateBackgroundLayout()
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
			.RegisterComponent(new SpriteComponent(SpriteShaderProperties::CreateProperties(BACKGROUND_TEXTURE_NAME), SHADER_NAME, RENDERLAYERS_GUI, true))
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
			.RegisterComponent(new SpriteComponent(SpriteShaderProperties::CreateProperties(BACKGROUND_TEXTURE_NAME), SHADER_NAME, RENDERLAYERS_GUI, true));
		// Create tools background
		s_ToolSlotsElementUID = pHierarchy->RegisterElement({ true }, equippmentGroupID)
			.SetTransform({ { 0.0, 0.0}, true }, { { 1.0, 0.5}, true })
			.RegisterComponent(new SpriteComponent(SpriteShaderProperties::CreateProperties(BACKGROUND_TEXTURE_NAME), SHADER_NAME, RENDERLAYERS_GUI, true))
			.GetIdentifier();

		// Create storage background
		unsigned long backgroundUID = pHierarchy->RegisterElement({ true }, s_InventoryElementUID)
			.SetTransform({ { -200, 0}, false, ALIGNMENT_RIGHT | ALIGNMENT_BOTTOM }, { { 200, 0}, false, ALIGNMENT_LEFT | ALIGNMENT_TOP })
			.RegisterComponent(new SpriteComponent(SpriteShaderProperties::CreateProperties(BACKGROUND_TEXTURE_NAME), SHADER_NAME, RENDERLAYERS_GUI, true))
			.GetIdentifier();

		// Create storage group parented to the storage background
		s_StorageSlotsElementUID = pHierarchy->RegisterElement({ false }, backgroundUID)
			.SetTransform({ { .1, .1 }, true }, { { .8, .8 }, true })
			.GetIdentifier();
	}

	void GUILayout::AssignSlots(const GP2D::GP2D_UID group, INVENTORY& rInventory)
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
				ITEM* currentItem = *(rInventory.GetBegin() + w + h * width);
				const char* textureName = 
					currentItem == nullptr ? DEFAULT_SLOT_TEXTURE
					: currentItem->TextureName == nullptr ? DEFAULT_SLOT_TEXTURE
					: currentItem->TextureName;

				CreateSlot(group, rInventory, { w,h }, textureName);
			}
		}
	}
	void GUILayout::CreateSlot(const GP2D::GP2D_UID group, INVENTORY& rInventory, SWAEngine::Math::Vector2Int location, const char* textureName)
	{
		SWAEngine::Math::Vector2Int inventorySize = rInventory.GetSize();

		//auto& rInventoryElement = Hierarchy::sp_ActiveInstance->GetElement(s_InventoryElementUID);
		auto& rStorageElement = Hierarchy::sp_ActiveInstance->GetElement(group);

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
		auto& rElement = Hierarchy::sp_ActiveInstance->RegisterElement({ true }, group)
			.SetTransform({ localPosition, true, ALIGNMENT_TOP | ALIGNMENT_LEFT }, { {scaleX, 1}, true,  ALIGNMENT_BOTTOM | ALIGNMENT_LEFT | ASPECT_RATIO_WIDTH })
			.RegisterComponent(new SpriteComponent(SpriteShaderProperties::CreateProperties(textureName), SHADER_NAME, RENDERLAYERS_GUI, true))
			.RegisterComponent(pButton);

		// Encode item location and group into int value
		int extraFlags = (location.X << 16) | (location.Y << 8) | (group);
		// Subscribe
		pButton->ButtonEventHandler.Subscribe({ [&](auto data, auto) {OnItemSlotButtonClicked(data.ExtraFlags & 0xff, { data.ExtraFlags >> 16, data.ExtraFlags >> 8 & 0xff }, rInventory); }, extraFlags });
	
		// Try enable outline
		if (rInventory.SelectedItemPosition == location)
			OnItemSlotButtonClicked(group, location, rInventory);
	}
	GP2D::GP2D_UID GUILayout::GetSlot(const GP2D::GP2D_UID group, float width, SWAEngine::Math::Vector2Int location)
	{
		return Hierarchy::sp_ActiveInstance->GetElement(group).Children.at(location.X + location.Y * width);
	}
	void GUILayout::SetSlotOutline(GP2D::GUI::Element& slot, GP2D::Math::Int2 thickness)
	{
		// Display/hide outline
		SpriteComponent* sprite = static_cast<SpriteComponent*>(slot.GetComponent(COMPONENT_TAG_SPRITE));
		SpriteShaderProperties* properties = static_cast<SpriteShaderProperties*>(sprite->p_Mesh->p_ExtraData);
		properties->OutlineThickness = thickness;
	}

	void GUILayout::OnInventoryAssignment(const GP2D::GP2D_UID group, SWAEngine::Math::Vector2Int inventorySize, INVENTORY::ASSIGNMENT_EVENT_ARGS arguments)
	{
		// Update item mesh
		Element& rSlot = Hierarchy::sp_ActiveInstance->GetElement(GetSlot(group, inventorySize.X, arguments.first));
		auto pSprite = static_cast<SpriteComponent*>(rSlot.GetComponent(COMPONENT_TAG_SPRITE));
		static_cast<SpriteShaderProperties*>(pSprite->p_Mesh->p_ExtraData)->TextureName = arguments.second->TextureName;

	}
	void GUILayout::OnItemSlotButtonClicked(const GP2D::GP2D_UID group, SWAEngine::Math::Vector2Int location, INVENTORY& rInventory)
	{
		rInventory.SelectedItemPosition = location;

		// Get the enabled slot id
		GP2D::GP2D_UID* slot = &m_EnabledStorageSlot;
		if (group == s_ToolSlotsElementUID)
			slot = &m_EnabledToolSlot;

		// Disable
		if (*slot != NULL)
			SetSlotOutline(GP2D::GUI::Hierarchy::sp_ActiveInstance->GetElement(*slot), { });

		// Enable
		*slot = GetSlot(group, rInventory.GetSize().X, location);
		SetSlotOutline(GP2D::GUI::Hierarchy::sp_ActiveInstance->GetElement(*slot), {2,2});
	}
	void GUILayout::OnToggleButtonClicked()
	{
		auto& rElement = Hierarchy::sp_ActiveInstance->GetElement(s_InventoryElementUID);
		// Toggle and thus iterate through meshes and set active
		rElement.SetActiveState(!rElement.GetActiveState());
	}
}