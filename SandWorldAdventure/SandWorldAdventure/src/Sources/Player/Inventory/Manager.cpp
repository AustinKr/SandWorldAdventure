#include "SWA/Player/Inventory/Manager.h"
#include "SWAEngine/Tilemap/TileBehavior/Types.h"
#include "SWA/Player/Inventory/Items/TileItem.h"
#include "SWA/Player/Inventory/Items/TileTool.h"
#include "SWA/Player/Inventory/Items/PaintTool.h"

using namespace SWAEngine::Math;

namespace SWA::Player::Inventory
{
	Manager::Manager()
	{
		StorageInventory = { { 4,8 } };
		StorageInventory.SetItemAt({ 1,1 }, new Items::TileItem("wet_sand_tile_slot", 0xeccc70ff, 0xc0a65aff, SWAEngine::Tilemap::TileBehavior::SOLID));
		StorageInventory.SetItemAt({ 2,1 }, new Items::TileItem("sand_tile_slot", 0xffe79eff, 0xebcb68ff, SWAEngine::Tilemap::TileBehavior::SAND));
		StorageInventory.SetItemAt({ 1,2 }, new Items::TileItem("water_tile_slot", 0x426dffff, 0x1147ffff, SWAEngine::Tilemap::TileBehavior::FLUID));

		ToolInventory = { {3,3} };
		ToolInventory.SetItemAt({ 1,1 }, new Items::TileTool(Items::TILE_TOOL_ALL_TILES));
		ToolInventory.SetItemAt({ 1,2 }, new Items::PaintTool(0xff0000ff, .3f, 10));
	}

	void Manager::TryUseSelectedItem(Inputs& rInputs, SWAEngine::Time time)
	{
		auto selectedItem = ToolInventory.GetItemAt(ToolInventory.SelectedItemPosition);
		if (selectedItem != nullptr)
			static_cast<Items::TileTool*>(selectedItem)->TryUse(StorageInventory, rInputs, time);
	}
	void Manager::Release()
	{
		StorageInventory.Release();
		ToolInventory.Release();
	}
}