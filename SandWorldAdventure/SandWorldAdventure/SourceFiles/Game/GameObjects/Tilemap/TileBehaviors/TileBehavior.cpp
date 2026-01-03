#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SandTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SolidTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	const TileBehavior* TileBehavior::TILE_BEHAVIORS[2] =
	{
		(TileBehavior*)new SolidTileBehavior(),
		(TileBehavior*)new SandTileBehavior(),
	};


	TileBehavior* TileBehavior::GetTileBehavior(int behaviorIndex)
	{
		return (TileBehavior*)TileBehavior::TILE_BEHAVIORS[behaviorIndex];
	}
	void TileBehavior::ReleaseTileBehaviors()
	{
		delete(TileBehavior::TILE_BEHAVIORS[0]);
		delete(TileBehavior::TILE_BEHAVIORS[1]);
	}

	bool TileBehavior::TryUpdate(
		Tilemap* pTilemap,
		Vector2Int tilePosition,
		TilemapContainer::TILE_INFO tileInfo,
		Time time)
	{
		return false;
	}
}