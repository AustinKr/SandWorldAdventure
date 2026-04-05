#include "SWA/Player/Inventory/PlayerInventoryManager.h"
#include "SWAEngine/Tilemap/TileBehavior/Types.h"

using namespace SWAEngine::Math;

namespace SWA::Player::Inventory
{
	PlayerInventoryManager::PlayerInventoryManager()
	{
		TilesInventory.Assign({ 4,8 }, {});
		TilesInventory.SetItemAt({ 1,1 }, TileItem("wet_sand_tile_slot", 0xeccc70ff, 0xc0a65aff, SWAEngine::Tilemap::TileBehavior::SOLID));
		TilesInventory.SetItemAt({ 2,1 }, TileItem("sand_tile_slot", 0xffe79eff, 0xebcb68ff, SWAEngine::Tilemap::TileBehavior::SAND));
		TilesInventory.SetItemAt({ 1,2 }, TileItem("water_tile_slot", 0x426dffff, 0x1147ffff, SWAEngine::Tilemap::TileBehavior::FLUID));
	}

	void PlayerInventoryManager::TryUseSelectedItem(Inputs& rInputs, SWAEngine::Time time)
	{
		TilesInventory.GetItemAt(TilesInventory.SelectedItemPosition).TryUse(rInputs, time);
	}
	void PlayerInventoryManager::Release()
	{
		TilesInventory.Release();
	}
}