#pragma once
#include "SWAEngine/Inventory/Inventory.h"
#include "SWA/Player/Inputs.h"
#include "SWAEngine/Time.h"

namespace SWA::Player::Inventory
{
	struct Manager
	{
		// TODO: Make multiple inventories for different sections. Equipment, hotbar, tools, and items
		SWAEngine::Inventory::Inventory StorageInventory;
		SWAEngine::Inventory::Inventory ToolInventory;

		// Initializes the inventories with default items
		Manager();

		void TryUseSelectedItem(Inputs& rInputs, SWAEngine::Time time);
		
		void Release();
	};
}