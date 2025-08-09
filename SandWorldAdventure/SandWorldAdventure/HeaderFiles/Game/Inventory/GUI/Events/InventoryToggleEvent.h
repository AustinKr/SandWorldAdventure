#pragma once
#include "HeaderFiles/Event/EventHandler.h"
#include "HeaderFiles/Event/DelegateTypes/IGUIButtonEventDelegate.h"
#include "HeaderFiles/Math.h"

#ifndef PLAYER_H
#define PLAYER_H
namespace SandboxEngine::Game::GameObject
{
	class Player;
}
#endif

namespace SandboxEngine::Game::Inventory
{
	struct InventoryToggleEvent : public Event::IGUIButtonEventDelegate
	{
	private:
		GameObject::Player* mp_Player;
	public:
		InventoryToggleEvent(GameObject::Player* pPlayer);

		// Toggles the visibilitly of the inventory gui
		virtual void Invoke(void*) override;
	};
}