#pragma once
#include "SWAEngine/Inventory/Inventory.h"
#include "SWA/Player/Inputs.h"
#include "SWAEngine/Time.h"
#include "SWA/Player/Inventory/TileItem.h"

namespace SWA::Player::Inventory
{
	// TODO: Could create IPhysicsObject
	// A player object that is associated with its mesh
	struct PlayerInventoryManager
	{
		// TODO: Make multiple inventories for different sections. Equipment, hotbar, tools, and items
		SWAEngine::Inventory::Inventory<TileItem> TilesInventory;

		// Initializes the inventories with default items
		PlayerInventoryManager();

		void TryUseSelectedItem(Inputs& rInputs, SWAEngine::Time time);
		
		void Release();
	};
}