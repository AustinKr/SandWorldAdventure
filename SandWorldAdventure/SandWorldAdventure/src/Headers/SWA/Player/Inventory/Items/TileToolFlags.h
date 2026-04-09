#pragma once

namespace SWA::Player::Inventory::Items
{
	enum TileToolFlags
	{
		TILE_TOOL_NO_TILES = 0, // Restricts all tiles
		TILE_TOOL_ALL_TILES = 1, // Places/removes all types of tiles
		TILE_TOOL_ALL_SAND_TILES = 2, // Places/removes all types of sand tiles
	};
}