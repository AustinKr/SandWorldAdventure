#pragma once
#include "HeaderFiles/Event/EventHandler.h"

#ifndef PLAYER_H
#define PLAYER_H
namespace SandboxEngine::Game::GameObject
{
	class Player;
}
#endif

namespace SandboxEngine::Game::Inventory
{
	struct InventoryItemAssignmentEvent : public Event::IEventDelegate
	{
	public:
		InventoryItemAssignmentEvent(GameObject::Player* pPlayer);

		// Updates the layout of gui slots for the items
		virtual void Invoke(void* pArgs) override;

	private:
		GameObject::Player* mp_Player;
	};
}