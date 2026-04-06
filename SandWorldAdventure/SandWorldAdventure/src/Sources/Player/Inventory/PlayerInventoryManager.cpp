#include "SWA/Player/Inventory/PlayerInventoryManager.h"
#include "SWAEngine/Tilemap/TileBehavior/Types.h"
#include "SWA/Player/Inventory/TileItem.h"

using namespace SWAEngine::Math;

namespace SWA::Player::Inventory
{
	PlayerInventoryManager::PlayerInventoryManager()
	{
		ItemInventory = {};
		ItemInventory.Assign({ 4,8 });
		ItemInventory.SetItemAt({ 1,1 }, new TileItem("wet_sand_tile_slot", 0xeccc70ff, 0xc0a65aff, SWAEngine::Tilemap::TileBehavior::SOLID));
		ItemInventory.SetItemAt({ 2,1 }, new TileItem("sand_tile_slot", 0xffe79eff, 0xebcb68ff, SWAEngine::Tilemap::TileBehavior::SAND));
		ItemInventory.SetItemAt({ 1,2 }, new TileItem("water_tile_slot", 0x426dffff, 0x1147ffff, SWAEngine::Tilemap::TileBehavior::FLUID));

		ToolInventory = {};
	}

	void PlayerInventoryManager::TryUseSelectedItem(Inputs& rInputs, SWAEngine::Time time)
	{
		auto selectedItem = ItemInventory.GetItemAt(ItemInventory.SelectedItemPosition);
		if (selectedItem != nullptr)
			static_cast<TileItem*>(selectedItem)->TryUse(rInputs, time);
	}
	void PlayerInventoryManager::Release()
	{
		ItemInventory.Release();
		ToolInventory.Release();
	}
}