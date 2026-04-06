#include "SWA/Player/Inventory/Manager.h"
#include "SWAEngine/Tilemap/TileBehavior/Types.h"
#include "SWA/Player/Inventory/Items/TileItem.h"

using namespace SWAEngine::Math;

namespace SWA::Player::Inventory
{
	Manager::Manager()
	{
		ItemInventory = {};
		ItemInventory.Assign({ 4,8 });
		ItemInventory.SetItemAt({ 1,1 }, new Items::TileItem("wet_sand_tile_slot", 0xeccc70ff, 0xc0a65aff, SWAEngine::Tilemap::TileBehavior::SOLID));
		ItemInventory.SetItemAt({ 2,1 }, new Items::TileItem("sand_tile_slot", 0xffe79eff, 0xebcb68ff, SWAEngine::Tilemap::TileBehavior::SAND));
		ItemInventory.SetItemAt({ 1,2 }, new Items::TileItem("water_tile_slot", 0x426dffff, 0x1147ffff, SWAEngine::Tilemap::TileBehavior::FLUID));

		ToolInventory = {};
	}

	void Manager::TryUseSelectedItem(Inputs& rInputs, SWAEngine::Time time)
	{
		auto selectedItem = ItemInventory.GetItemAt(ItemInventory.SelectedItemPosition);
		if (selectedItem != nullptr)
			static_cast<Items::TileItem*>(selectedItem)->TryUse(rInputs, time);
	}
	void Manager::Release()
	{
		ItemInventory.Release();
		ToolInventory.Release();
	}
}