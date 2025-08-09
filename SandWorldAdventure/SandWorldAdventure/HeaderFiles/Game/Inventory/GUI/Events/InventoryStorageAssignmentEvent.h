#pragma once
#include "HeaderFiles/Event/EventHandler.h"
#include "HeaderFiles/Game/Inventory/BasicItem.h"

#ifndef PLAYER_H
#define PLAYER_H
namespace SandboxEngine::Game::GameObject
{
	class Player;
}
#endif

namespace SandboxEngine::GUISystem
{
#ifndef GUIELEMENT_H
#define GUIELEMENT_H
	class GUIElement;
#endif
}

namespace SandboxEngine::Game::Inventory
{
	struct InventoryStorageAssignmentEvent : public Event::IEventDelegate
	{
	public:
		InventoryStorageAssignmentEvent(GameObject::Player* pPlayer);

		// Updates the layout of gui slots for the items
		virtual void Invoke(void*) override;

	private:
		GameObject::Player* mp_Player;

		// Creates elements for each slot where items can be displayed
		void AssignSlots();
		void CreateSlot(GUISystem::GUIElement* pStorageElement, Vector2Int location, BasicItem& rItem);
	};
}