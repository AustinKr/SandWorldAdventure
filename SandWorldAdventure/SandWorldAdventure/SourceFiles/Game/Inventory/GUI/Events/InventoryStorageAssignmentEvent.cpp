#include "HeaderFiles/Game/Inventory/GUI/Events/InventoryStorageAssignmentEvent.h"
#include "HeaderFiles/Game/GameObjects/Player.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/GUISystem/GUISystemFramework.h"
#include "HeaderFiles/GUISystem/Components/GUIButtonComponent.h"
#include "HeaderFiles/GUISystem/Components/GUISpriteComponent.h"
#include "HeaderFiles/Game/Inventory/GUI/Events/ItemSlotButtonEvent.h"

namespace SandboxEngine::Game::Inventory
{
	InventoryStorageAssignmentEvent::InventoryStorageAssignmentEvent(GameObject::Player* pPlayer)
	{
		mp_Player = pPlayer;
	}
	void InventoryStorageAssignmentEvent::Invoke(void*)
	{
		AssignSlots();
	}
	void InventoryStorageAssignmentEvent::AssignSlots()
	{
		// TODO: How to update slot gui texture path when tile is changed in the inventory
		// . subscribe individual events for each slot for pre render and upate the texture (expensive but could be simplified if code is modified)
		// . Could force assignment and retrieval of the inventory to respective functions and have the set function invoke a handler which will have an event subscribed, which updates the given correspoding slot

		GUISystem::GUIElement* pStorageElement = MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(mp_Player->InventoryGUI.StorageSlotsElementID);

		// Clear elements
		MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->EraseChildren(mp_Player->InventoryGUI.StorageSlotsElementID);

		// Create new elements for each item slot
		for (int h = 0; h < mp_Player->CurrentInventory.GetSize().Y; h++)
		{
			for (int w = 0; w < mp_Player->CurrentInventory.GetSize().X; w++)
			{
				CreateSlot(pStorageElement, { w,h }, *(mp_Player->CurrentInventory.GetBegin() + w + h * mp_Player->CurrentInventory.GetSize().X));
			}
		}
	}
	void InventoryStorageAssignmentEvent::CreateSlot(GUISystem::GUIElement* pStorageElement, Vector2Int location, BasicItem& rItem)
	{
		// Create the slot
		GUISystem::GUIElement* pElement = new GUISystem::GUIElement(&MasterWindow::GraphicalUserInterfaceSystem, mp_Player->InventoryGUI.StorageSlotsElementID);
		pElement->RegisterComponent(new GUISystem::Components::GUISpriteComponent(-1, rItem.ImagePath));
		pElement->RegisterComponent(new GUISystem::Components::GUIButtonComponent())->ButtonEventHandler.SubscribeEvent(new ItemSlotButtonEvent(mp_Player, location));

		// Set coordinates
		Vector2 totalScale = pStorageElement->ConvertGlobalToLocal(Vector2(mp_Player->InventoryGUI.SlotScale + mp_Player->InventoryGUI.SlotPadding, mp_Player->InventoryGUI.SlotScale + mp_Player->InventoryGUI.SlotPadding), false); // total size of slot
		Vector2 position = Vector2(location.X, -location.Y - 1) * totalScale; // the top left corner of the slot
		pElement->SetTransform({ position, true, GUISystem::ALIGNMENT_TOP | GUISystem::ALIGNMENT_LEFT }, { Vector2(mp_Player->InventoryGUI.SlotScale,mp_Player->InventoryGUI.SlotScale), false });
	}
}