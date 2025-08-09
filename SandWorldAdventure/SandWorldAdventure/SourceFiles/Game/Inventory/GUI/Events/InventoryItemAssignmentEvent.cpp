#include "HeaderFiles/Game/Inventory/GUI/Events/InventoryItemAssignmentEvent.h"
#include "HeaderFiles/Game/GameObjects/Player.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/GUISystem/GUISystemFramework.h"
#include "HeaderFiles/GUISystem/Components/GUISpriteComponent.h"

namespace SandboxEngine::Game::Inventory
{
	InventoryItemAssignmentEvent::InventoryItemAssignmentEvent(GameObject::Player* pPlayer)
	{
		mp_Player = pPlayer;
	}
	void InventoryItemAssignmentEvent::Invoke(void* pArgs)
	{
		// Get arguments
		Vector2Int position = *reinterpret_cast<Vector2Int*>(pArgs);
		BasicItem item = *reinterpret_cast<BasicItem*>((char*)pArgs + sizeof(Vector2Int));

		// Update slot

		GUISystem::GUIElement* pStorageElement = MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(mp_Player->InventoryGUI.StorageSlotsElementID);
		int itemID = mp_Player->CurrentInventory.GetSize().X * position.Y + position.X;
		unsigned long slotElementID = *(pStorageElement->GetChildrenBegin() + itemID);

		GUISystem::GUIElement* pSlotElement = MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(slotElementID);
		GraphicsPipeline::Mesh* pSlotMesh = pSlotElement->GetComponent<GUISystem::Components::GUISpriteComponent>(GUISystem::ComponentTags::COMPONENT_TAG_SPRITE)->GetMesh();
		pSlotMesh->Texture.Release();
		pSlotMesh->Texture = GraphicsPipeline::Texture::LoadBMPTextureFromFile(item.ImagePath);
	}
}