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
	struct ItemSlotButtonEvent : public Event::IGUIButtonEventDelegate
	{
	private:
		GameObject::Player* mp_Player;
		Vector2Int m_SlotLocation;

	public:
		virtual void Invoke(void*) override;
		ItemSlotButtonEvent(GameObject::Player* pPlayer, Vector2Int slotLocation, int flags = GUISystem::BUTTON_EVENT_MOUSE_1 | GUISystem::BUTTON_EVENT_MOUSE_DOWN);
	};
}
