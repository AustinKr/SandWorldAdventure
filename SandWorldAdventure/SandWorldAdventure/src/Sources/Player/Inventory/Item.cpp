#include "SWA/Player/Inventory/TileItem.h"

#include "SWAEngine/Tilemap/TileBehavior/IBehavior.h"
#include "SWAEngine/Tilemap/Tile.h"
#include "SWAEngine/Tilemap/TileBehavior/Types.h"
#include "SWAEngine/Math/ColorUtility.h"

#include "SWA/Game.h"
#include "GP2D/Pipeline/Window/Window.h"
#include "GP2D/Pipeline/GenericPipeline.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace GP2D::Pipeline;
using namespace GP2D::Math;
using namespace SWAEngine::Math;
using namespace SWAEngine::Tilemap;
using namespace SWAEngine::Tilemap::TileBehavior;

namespace SWA::Player::Inventory
{
	TileItem::TileItem()
		: Item(), Color(0x0), ColorDeviation(0x0), BehaviorUID(0)
	{}
	TileItem::TileItem(const char* textureName, GP2D::GP2D_HEX_COLOR color, GP2D::GP2D_HEX_COLOR colorDeviation, unsigned int behavior)
		: Item(PLAYER_ITEM_TYPE_TILE, textureName), Color(color), ColorDeviation(colorDeviation), BehaviorUID(behavior)
	{}

	void TileItem::TryUse(Inputs& rInputs, SWAEngine::Time time)
	{
		if (!rInputs.AddTile && !rInputs.BreakTile)
			return;

		// Get cursor position
		double cursorX, cursorY;
		glfwGetCursorPos(Window::Window::sp_Window, &cursorX, &cursorY);
		cursorY = GenericPipeline::s_ActiveCamera.GetScreenSize().Y - cursorY; // Invert so that now it's from the bottom left of the screen

		// Do conversions
		Float2 mouseWorldPosition =
			GenericPipeline::s_ActiveCamera.ViewportToWorld(
				GenericPipeline::s_ActiveCamera.ScreenToViewport({ int(cursorX), int(cursorY) }));
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
					Tile tile = { SOLID, 0x0, true };
					if (BehaviorUID != SOLID)
						tile = IBehavior::s_Behaviors.at(BehaviorUID)->CreateNew(time);
					tile.Color = MixColor(Color, ColorDeviation, (float)rand() / (float)RAND_MAX);

					Game::p_Tilemap->SetTile(position, tile);
				}
				else
					Game::p_Tilemap->SetTile(position, {});
			}
		}
	}
}