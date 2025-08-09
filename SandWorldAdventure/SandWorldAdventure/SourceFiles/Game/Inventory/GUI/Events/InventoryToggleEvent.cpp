#include "HeaderFiles/Game/Inventory/GUI/Events/InventoryToggleEvent.h"
#include "HeaderFiles/Game/GameObjects/Player.h"
#include "HeaderFiles/MasterWindow.h"

namespace SandboxEngine::Game::Inventory
{
	InventoryToggleEvent::InventoryToggleEvent(GameObject::Player* pPlayer) : mp_Player(pPlayer) {/*nothing*/ }
	void InventoryToggleEvent::Invoke(void*)
	{
		GUISystem::GUIElement* pElement = MasterWindow::GraphicalUserInterfaceSystem.p_Hierarchy->GetElement(mp_Player->InventoryGUI.InventoryElementID);

		// Toggle and iterate through meshes and set active
		pElement->SetActiveState(!pElement->GetActiveState());
	}
}