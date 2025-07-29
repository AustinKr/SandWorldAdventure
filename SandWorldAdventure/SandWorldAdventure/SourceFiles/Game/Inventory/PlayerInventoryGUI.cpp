#include "HeaderFiles/Game/Inventory/PlayerInventoryGUI.h"
#include "HeaderFiles/Game/GameObjects/Player.h"
#include "HeaderFiles/GUISystem/GUISystemFramework.h"
#include "HeaderFiles/GUISystem/Components/GUISpriteComponent.h"
#include "HeaderFiles/GUISystem/Components/GUIButtonComponent.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/GUISystem/ElementAlignmentFlags.h"

namespace SandboxEngine::Game::Inventory
{
	// - InventoryToggleEvent -
	PlayerInventoryGUI::InventoryToggleEvent::InventoryToggleEvent(GameObject::Player* pPlayer) : mp_Player(pPlayer)	{/*nothing*/ }
	void PlayerInventoryGUI::InventoryToggleEvent::Invoke(void*)
	{
		GUISystem::GUIElement* pElement = MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(mp_Player->InventoryGUI.m_InventoryElementID);

		// Toggle and iterate through meshes and set active
		pElement->SetActiveState(!pElement->GetActiveState());
	}

	// - InventoryStorageAssignmentEvent -
	PlayerInventoryGUI::InventoryStorageAssignmentEvent::InventoryStorageAssignmentEvent(GameObject::Player* pPlayer)
	{
		mp_Player = pPlayer;
	}
	void PlayerInventoryGUI::InventoryStorageAssignmentEvent::Invoke(void*)
	{
		AssignSlots();
	}
	void PlayerInventoryGUI::InventoryStorageAssignmentEvent::AssignSlots()
	{
		GUISystem::GUIElement* pStorageElement = MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(mp_Player->InventoryGUI.m_StorageSlotsElementID);

		// Clear elements
		MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->EraseChildren(mp_Player->InventoryGUI.m_StorageSlotsElementID);

		// Create new elements for each item slot
		for (int w = 0; w < mp_Player->CurrentInventory.GetSize().X; w++)
		{
			for (int h = 0; h < mp_Player->CurrentInventory.GetSize().Y; h++)
			{
				CreateSlot({ w,h }, *(mp_Player->CurrentInventory.GetBegin() + w + h * mp_Player->CurrentInventory.GetSize().X));
			}
		}
	}
	void PlayerInventoryGUI::InventoryStorageAssignmentEvent::CreateSlot(Vector2Int location, BasicItem& rItem)
	{
		GUISystem::GUIElement *pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem, mp_Player->InventoryGUI.m_StorageSlotsElementID);
		pElement->RegisterComponent(new GUISystem::Components::GUISpriteComponent(-1, rItem.ImagePath));
		//pElement->RegisterComponent(new GUISystem::Components::GUIButtonComponent())->ButtonEventHandler.SubscribeEvent(new ItemSlotButtonEvent(mp_Player));
		Vector2 position = (Vector2)location / (Vector2)mp_Player->CurrentInventory.GetSize();
		Vector2 scale = Vector2(1,1) / (Vector2)mp_Player->CurrentInventory.GetSize() / (double)SLOTS_PADDING;
		pElement->SetTransform({position, true}, { scale, true });
	}


	// - PlayerInventoryGUI -

	const char* PlayerInventoryGUI::BACKGROUND_BMP_DIRECTORY = "C:/dev/C++ Projects/SandWorldAdventure/SandWorldAdventure/SandWorldAdventure/Resources/GUI/Background.bmp";
	const int PlayerInventoryGUI::SLOTS_PADDING = 2;

	void PlayerInventoryGUI::CreateBackgroundLayout()
	{
		// Register for assignment of inventory
		mp_Player->CurrentInventory.AssignmentEventHandler.SubscribeEvent(new InventoryStorageAssignmentEvent(mp_Player));

		// Create empty group for inventory
		GUISystem::GUIElement* pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem);
		pElement->SetTransform({ {0.05, 0.05}, true, }, { {.9, .9 }, true });
		m_InventoryElementID = pElement->Identifier;

		// Create close button
		pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem);
		pElement->RegisterComponent(new GUISystem::Components::GUISpriteComponent(-1, BACKGROUND_BMP_DIRECTORY));
		pElement->RegisterComponent(new GUISystem::Components::GUIButtonComponent())->ButtonEventHandler.SubscribeEvent(new InventoryToggleEvent(mp_Player));
		pElement->SetTransform({ { -0.1, -0.1}, true, GUISystem::ALIGNMENT_TOP | GUISystem::ALIGNMENT_CENTER_HORIZONTAL }, { { .2, 0.075 }, true });

		// Create menus background
		pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem, m_InventoryElementID);
		pElement->RegisterComponent(new GUISystem::Components::GUISpriteComponent(-1, BACKGROUND_BMP_DIRECTORY));
		pElement->SetTransform({ { 0.0, 0.0}, true }, { { .3, 1.0 }, true });

		// Create storage background
		pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem, m_InventoryElementID);
		pElement->RegisterComponent(new GUISystem::Components::GUISpriteComponent(-1, BACKGROUND_BMP_DIRECTORY));
		pElement->SetTransform({ { -.3, 0.0 }, true, GUISystem::ALIGNMENT_RIGHT | GUISystem::ALIGNMENT_BOTTOM }, { { .3, 1.0 }, true });

		// Create storage group parented to the storage background
		pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem, pElement->Identifier);
		pElement->SetTransform({ { .05, 0.05 }, true}, { { .9, .9 }, true });
		m_StorageSlotsElementID = pElement->Identifier;
	}
	bool PlayerInventoryGUI::IsActive()
	{
		return MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(m_InventoryElementID)->GetActiveState();
	}
	PlayerInventoryGUI::PlayerInventoryGUI(GameObject::Player* pPlayer) : mp_Player(pPlayer)
	{
		mp_Player = pPlayer;
		CreateBackgroundLayout();
	}
}