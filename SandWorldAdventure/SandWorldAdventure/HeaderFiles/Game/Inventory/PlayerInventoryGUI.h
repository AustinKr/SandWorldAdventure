#pragma once
#include "HeaderFiles/Event/EventHandler.h"
#include "HeaderFiles/Event/DelegateTypes/IGUIButtonEventDelegate.h"
#include "HeaderFiles/Game/Inventory/BasicItem.h"
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
	struct PlayerInventoryGUI
	{
	private:
		static const char* BACKGROUND_BMP_DIRECTORY;
		// The number of item slots in between each slot
		static const int SLOTS_PADDING;

		GameObject::Player* mp_Player;
		// Used to parent all gui elements related to inventory system into one group
		unsigned long m_InventoryElementID;
		// A button that is used to open or close the inventory
		unsigned long m_InventoryToggleButtonID;
		// Slots to show items
		unsigned long m_StorageSlotsElementID;
		
		struct InventoryToggleEvent : public Event::IGUIButtonEventDelegate
		{
		private:
			GameObject::Player* mp_Player;
		public:
			InventoryToggleEvent(GameObject::Player* pPlayer);

			// Toggles the visibilitly of the inventory gui
			virtual void Invoke(void*) override;
		};

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
			void CreateSlot(Vector2Int location, BasicItem& rItem);
		};

		// Creates the layout for the inventory gui
		void CreateBackgroundLayout();
	public:
		bool IsActive();
		//// Deletes all gui
		//void EraseGUI();

		// Subscribes to assignment event in player.inventory
		PlayerInventoryGUI(GameObject::Player* pPlayer);
	};
}