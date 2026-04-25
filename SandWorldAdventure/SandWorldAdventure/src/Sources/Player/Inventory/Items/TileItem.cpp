#include <SWA/Player/Inventory/Items/TileItem.h>
#include <SWA/Player/Inventory/Items/Types.h>

#include <SWAEngine/Component/Tilemap/Tile.h>
#include <SWAEngine/Component/Tilemap/TileBehavior/IBehavior.h>
#include <SWAEngine/Component/Tilemap/TileBehavior/Types.h>
#include <SWAEngine/Math/ColorUtility.h>

using namespace SWAEngine;
using namespace SWAEngine::Component::Tilemap;
using namespace TileBehavior;

namespace SWA::Player::Inventory::Items
{
	TileItem::TileItem() : BaseItem(), Color(0x0), ColorDeviation(0x0), BehaviorUID(0) {}
	TileItem::TileItem(const char* textureName, GP2D::GP2D_HEX_COLOR color, GP2D::GP2D_HEX_COLOR colorDeviation, unsigned int behavior)
		: BaseItem(ITEM_TILE, textureName), Color(color), ColorDeviation(colorDeviation), BehaviorUID(behavior) {}

	Tile TileItem::CreateTile(Math::Time time)
	{
		Tile tile = { SOLID, 0x0, true };
		if (BehaviorUID != SOLID)
			tile = IBehavior::s_Behaviors.at(BehaviorUID)->CreateNew(time);
		tile.Color = Math::MixColor(Color, ColorDeviation, (float)rand() / (float)RAND_MAX);

		return tile;
	}
}