#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SolidTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	bool SolidTileBehavior::TryUpdate(
		Tilemap* pTilemap,
		Vector2Int tilePosition,
		TilemapContainer::TILE_INFO tileInfo,
		Time time)
	{
		return false;
	}
}