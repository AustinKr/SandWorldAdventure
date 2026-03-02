#pragma once
#include "HeaderFiles/Math.h"

namespace SandboxEngine::Game::Inventory
{
#ifndef INVENTORY_H
#define INVENTORY_H
	template<typename ItemType>
	struct Inventory;
#endif

	template<typename ItemType>
	struct ItemAssignmentEventArguments
	{
		// The column and row of the item in the inventory
		Vector2Int Location;
		// The item stored
		ItemType Item;
	};
}