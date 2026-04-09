#include "SWA/Player/Inventory/Items/TileTool.h"
#include "SWA/Player/Inventory/Items/TileItem.h"

#include "SWA/Game.h"
#include "GP2D/Pipeline/Window/Window.h"
#include "GP2D/Pipeline/GenericPipeline.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace GP2D::Pipeline;
using namespace GP2D::Math;
using namespace SWAEngine::Math;

namespace SWA::Player::Inventory::Items
{
	TileTool::TileTool()
		: UseableItem(), Flags(TILE_TOOL_NO_TILES)
	{}
	TileTool::TileTool(const char* textureName, TileToolFlags flags)
		: UseableItem(ITEM_TILE_TOOL, textureName), Flags(flags)
	{}

	void TileTool::TryUse(SWAEngine::Inventory::Inventory& rStorageInventory, Inputs& rInputs, SWAEngine::Time time)
	{
		if (!rInputs.AddTile && !rInputs.BreakTile)
			return;

		// Try get resource to use
		auto pSelectedItem = rStorageInventory.GetItemAt(rStorageInventory.SelectedItemPosition);
		if (pSelectedItem == nullptr || pSelectedItem->Type != ITEM_TILE)
			return;

		// Get the mouse
		Float2 mouseWorldPosition = GenericPipeline::s_ActiveCamera.GetMouseWorld();
		Vector2Int mouseTilePosition = Game::p_Tilemap->WorldToTile({ mouseWorldPosition.X, mouseWorldPosition.Y });

		// remove/add tiles at cursor
		double radius = 5;
		for (int i = -radius; i < radius; i++)
		{
			for (int j = -radius; j < radius; j++)
			{
				if (i * i + j * j >= radius * radius)
					continue;

				Vector2Int position = mouseTilePosition + Vector2Int(i, j);
				if (position.X < 0 || position.Y < 0)
					continue;
				if (rInputs.AddTile)
				{
					TileItem* pTileItem = static_cast<TileItem*>(pSelectedItem);
					Game::p_Tilemap->SetTile(position, pTileItem->CreateTile(time));
				}
				else
					Game::p_Tilemap->SetTile(position, {});
			}
		}
	}
}