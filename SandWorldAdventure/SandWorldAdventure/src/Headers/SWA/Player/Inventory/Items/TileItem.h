#pragma once
#include "SWAEngine/Inventory/BaseItem.h"
#include "SWAEngine/Tilemap/Tile.h"
#include "SWAEngine/Time.h"

#include "GP2D/GraphicsTypes.h"

namespace SWA::Player::Inventory::Items
{
	// Stores tile information
	struct TileItem : SWAEngine::Inventory::BaseItem
	{
		GP2D::GP2D_HEX_COLOR Color; // Base color
		GP2D::GP2D_HEX_COLOR ColorDeviation; // Mixed with base color randomly
		unsigned int BehaviorUID;

		TileItem();
		TileItem(const char* textureName, GP2D::GP2D_HEX_COLOR color, GP2D::GP2D_HEX_COLOR colorDeviation, unsigned int behavior);
	
		// Creates a tile with the stored information
		SWAEngine::Tilemap::Tile CreateTile(SWAEngine::Time time);
	};
}