#include "HeaderFiles/Game/Inventory/GUI/Events/ItemSlotButtonEvent.h"
#include "HeaderFiles/Game/GameObjects/Player.h"

namespace SandboxEngine::Game::Inventory
{
	void ItemSlotButtonEvent::Invoke(void*)
	{
		mp_Player->CurrentInventory.SelectedItemID = m_SlotLocation;
	}
	ItemSlotButtonEvent::ItemSlotButtonEvent(GameObject::Player* pPlayer, Vector2Int slotLocation, int flags)
	{
		mp_Player = pPlayer;
		m_SlotLocation = slotLocation;
		ButtonEventFlags = flags;
	}
}