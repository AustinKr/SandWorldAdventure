#include "SWA/Player/Inventory/Items/PaintTool.h"
#include "SWA/Game.h"

#include "SWAEngine/Math/ColorUtility.h"

#include "GP2D/Pipeline/GenericPipeline.h"
#include <algorithm>

using namespace GP2D;
using namespace SWAEngine::Math;

namespace SWA::Player::Inventory::Items
{
	PaintTool::PaintTool()
		: UseableItem(), Color(0xffffffff), Strength(.3f), Radius(5)
	{}
	PaintTool::PaintTool(GP2D::GP2D_HEX_COLOR color, float strength, int radius)
		: UseableItem(ITEM_PAINT_TOOL, "paint_tool_slot"), Color(color), Strength(strength), Radius(radius)
	{}

	void PaintTool::TryUse(SWAEngine::Inventory::Inventory& rItemInventory, Inputs& rInputs, SWAEngine::Time time)
	{
		Math::Float2 mouseWorld = Pipeline::GenericPipeline::s_ActiveCamera.GetMouseWorld();
		Vector2Int mouseTile = Game::p_Tilemap->WorldToTile({ mouseWorld.X, mouseWorld.Y });
		
		// paint in a circle
		for (int i = -Radius; i < Radius; i++)
		{
			for (int j = -Radius; j < Radius; j++)
			{
				float distanceSqrd = i * i + j * j;
				if (distanceSqrd >= Radius * Radius)
					continue;

				Vector2Int position = mouseTile + Vector2Int(i, j);
				if (position.X < 0 || position.Y < 0)
					continue;
				if (rInputs.AddTile)
				{
					auto newTile = Game::p_Tilemap->GetTile(position);

					float factor = 1.0f / (distanceSqrd + .1) * Strength;
					newTile.Color = SWAEngine::Math::MixColor(Color, newTile.Color, std::clamp(factor, 0.0f, 1.0f));
					
					Game::p_Tilemap->SetTile(position, newTile);
				}
			}
		}
	}
}