#include "HeaderFiles/Game/Inventory/GUI/PlayerInventoryGUI.h"
#include "HeaderFiles/Game/GameObjects/Player.h"
#include "HeaderFiles/GUISystem/GUISystemFramework.h"
#include "HeaderFiles/GUISystem/Components/GUISpriteComponent.h"
#include "HeaderFiles/GUISystem/Components/GUIButtonComponent.h"
#include "HeaderFiles/GUISystem/ComponentTags.h"
#include "HeaderFiles/GUISystem/ElementAlignmentFlags.h"
#include "HeaderFiles/MasterWindow.h"

// Events
#include "HeaderFiles/Game/Inventory/GUI/Events/InventoryItemAssignmentEvent.h"
#include "HeaderFiles/Game/Inventory/GUI/Events/InventoryStorageAssignmentEvent.h"
#include "HeaderFiles/Game/Inventory/GUI/Events/InventoryToggleEvent.h"
#include "HeaderFiles/Game/Inventory/GUI/Events/ItemSlotButtonEvent.h"

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
		pElement->RegisterComponent(new GUISystem::Components::GUIButtonComponent())->ButtonEventHandler.SubscribeEvent(new InventoryToggleEvent(mp_Player));
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
		pElement->SetTransform({ { .05, 0.05 }, true}, { { .9, .9 }, true });
		StorageSlotsElementID = pElement->Identifier;
	}
	bool PlayerInventoryGUI::IsActive()
	{
		return MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(InventoryElementID)->GetActiveState();
	}
	PlayerInventoryGUI::PlayerInventoryGUI(GameObject::Player* pPlayer) : mp_Player(pPlayer), SlotScale(40), SlotPadding(15)
	{
		// Register for assignment of inventory
		mp_Player->CurrentInventory.AssignmentEventHandler.SubscribeEvent(new InventoryStorageAssignmentEvent(mp_Player));
		mp_Player->CurrentInventory.ItemAssignmentEventHandler.SubscribeEvent(new InventoryItemAssignmentEvent(mp_Player));

		// Create elements
		CreateBackgroundLayout();
	}
}