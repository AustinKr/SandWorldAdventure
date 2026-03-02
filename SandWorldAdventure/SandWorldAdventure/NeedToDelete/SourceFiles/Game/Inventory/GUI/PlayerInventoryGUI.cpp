#include "HeaderFiles/Game/Inventory/PlayerInventoryGUI.h"
#include "HeaderFiles/Game/GameObjects/Player.h"

#include "HeaderFiles/GUISystem/GUISystemFramework.h"
#include "HeaderFiles/GUISystem/Components/GUISpriteComponent.h"
#include "HeaderFiles/GUISystem/Components/GUIButtonComponent.h"
#include "HeaderFiles/GUISystem/ComponentTags.h"
#include "HeaderFiles/GUISystem/ElementAlignmentFlags.h"

#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/Game/GameInstance.h"

namespace SandboxEngine::Game::Inventory
{
	const char* PlayerInventoryGUI::BACKGROUND_BMP_DIRECTORY = "C:/dev/C++ Projects/SandWorldAdventure/SandWorldAdventure/SandWorldAdventure/Resources/GUI/Background.bmp";


	void PlayerInventoryGUI::CreateBackgroundLayout()
	{
		// Create empty group for inventory
		GUISystem::GUIElement* pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem, false);
		pElement->SetTransform({ {0.05, 0.05}, true, }, { {.9, .9 }, true });
		InventoryElementID = pElement->Identifier;

		// Create close button
		pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem);
		pElement->RegisterComponent(new GUISystem::Components::GUISpriteComponent(-1, BACKGROUND_BMP_DIRECTORY));
		pElement->RegisterComponent(new GUISystem::Components::GUIButtonComponent())
			->ButtonEventHandler.SubscribeEvent(Event::BaseEventDelegate<void*, GUISystem::ButtonEventData>(OnToggleButtonClicked, {}));
		pElement->SetTransform({ { -0.1, -0.1}, true, GUISystem::ALIGNMENT_TOP | GUISystem::ALIGNMENT_CENTER_HORIZONTAL }, { { .2, 0.075 }, true });

		// Create menus background
		pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem, InventoryElementID);
		pElement->RegisterComponent(new GUISystem::Components::GUISpriteComponent(-1, BACKGROUND_BMP_DIRECTORY));
		pElement->SetTransform({ { 0.0, 0.0}, true }, { { .3, 1.0 }, true });

		// Create storage background
		pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem, InventoryElementID);
		pElement->RegisterComponent(new GUISystem::Components::GUISpriteComponent(-1, BACKGROUND_BMP_DIRECTORY));
		pElement->SetTransform({ { -.3, 0.0 }, true, GUISystem::ALIGNMENT_RIGHT | GUISystem::ALIGNMENT_BOTTOM }, { { .3, 1.0 }, true });

		// Create storage group parented to the storage background
		pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem, pElement->Identifier, false);
		pElement->SetTransform({ { .05, 0.05 }, true }, { { .9, .9 }, true });
		StorageSlotsElementID = pElement->Identifier;
	}

	void PlayerInventoryGUI::AssignSlots()
	{
		if (StorageSlotsElementID < 0)
			return;

		GUISystem::GUIElement* pStorageElement = MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(StorageSlotsElementID);

		// Clear elements
		MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->EraseChildren(StorageSlotsElementID);

		// Create new elements for each item slot
		for (int h = 0; h < mp_Player->CurrentInventory.GetSize().Y; h++)
		{
			for (int w = 0; w < mp_Player->CurrentInventory.GetSize().X; w++)
			{
				CreateSlot(pStorageElement, { w,h }, *(mp_Player->CurrentInventory.GetBegin() + w + h * mp_Player->CurrentInventory.GetSize().X));
			}
		}
	}
	void PlayerInventoryGUI::CreateSlot(GUISystem::GUIElement* pStorageElement, Vector2Int location, BasicItem& rItem)
	{
		// Create the slot
		GUISystem::GUIElement* pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem, StorageSlotsElementID); // Create element
		pElement->RegisterComponent(new GUISystem::Components::GUISpriteComponent(-1, rItem.ImagePath)); // register sprite
		int extraFlags = (location.X << 8) | (location.Y);																// Encode item location into int value
		pElement->RegisterComponent(new GUISystem::Components::GUIButtonComponent())									// Register buttom component
			->ButtonEventHandler.SubscribeEvent(																		// Subscribe event
				Event::BaseEventDelegate<void*, GUISystem::ButtonEventData>(OnItemSlotButtonClicked, { extraFlags }));  // Pass function pointer, and item location

		// Set coordinates
		Vector2 totalScale = pStorageElement->ConvertGlobalToLocal(Vector2(SlotScale + SlotPadding, SlotScale + SlotPadding), false); // total size of slot
		Vector2 position = Vector2(location.X, -location.Y - 1) * totalScale; // the top left corner of the slot
		pElement->SetTransform({ position, true, GUISystem::ALIGNMENT_TOP | GUISystem::ALIGNMENT_LEFT }, { Vector2(SlotScale, SlotScale), false });
	}


	void PlayerInventoryGUI::OnInventoryAssignment(Vector2Int oldSize, void*)
	{
		if (GameInstance::p_Player != nullptr)
			GameInstance::p_Player->InventoryGUI.AssignSlots();
	}
	void PlayerInventoryGUI::OnItemAssignment(ItemAssignmentEventArguments<BasicItem> arguments, void*)
	{
		// Update item mesh
		
		// Update slot

		GUISystem::GUIElement* pStorageElement = MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(GameInstance::p_Player->InventoryGUI.StorageSlotsElementID);
		int itemID = GameInstance::p_Player->CurrentInventory.GetSize().X * arguments.Location.Y + arguments.Location.X;
		unsigned long slotElementID = *(pStorageElement->GetChildrenBegin() + itemID);

		GUISystem::GUIElement* pSlotElement = MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(slotElementID);
		GraphicsPipeline::Mesh* pSlotMesh = pSlotElement->GetComponent<GUISystem::Components::GUISpriteComponent>(GUISystem::ComponentTags::COMPONENT_TAG_SPRITE)->GetMesh();
		pSlotMesh->Texture.Release();
		pSlotMesh->Texture = GraphicsPipeline::Texture::LoadBMPTextureFromFile(arguments.Item.ImagePath);
	}
	void PlayerInventoryGUI::OnItemSlotButtonClicked(void*, GUISystem::ButtonEventData data)
	{
		char x = data.ExtraFlags >> 8, y = data.ExtraFlags & 0xff; // Decode integer to get item id
		GameInstance::p_Player->CurrentInventory.SelectedItemID = { x, y};
	}
	void PlayerInventoryGUI::OnToggleButtonClicked(void*, GUISystem::ButtonEventData)
	{
		GUISystem::GUIElement* pElement = MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(GameInstance::p_Player->InventoryGUI.InventoryElementID);

		// Toggle and iterate through meshes and set active
		pElement->SetActiveState(!pElement->GetActiveState());
	}


	bool PlayerInventoryGUI::IsActive()
	{
		return MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(InventoryElementID)->GetActiveState();
	}
	PlayerInventoryGUI::PlayerInventoryGUI(GameObject::Player* pPlayer) : mp_Player(pPlayer), SlotScale(40), SlotPadding(15), InventoryElementID(-1), InventoryToggleButtonID(-1), StorageSlotsElementID(-1)
	{
		// Register for assignment of inventory
		mp_Player->CurrentInventory.AssignmentEventHandler.SubscribeEvent(Event::BaseEventDelegate<Vector2Int, void*>(OnInventoryAssignment, nullptr));
		mp_Player->CurrentInventory.ItemAssignmentEventHandler.SubscribeEvent(Event::BaseEventDelegate<ItemAssignmentEventArguments<BasicItem>, void*>(OnItemAssignment, nullptr));

		// Create elements
		CreateBackgroundLayout();
	}
}