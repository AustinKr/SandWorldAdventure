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

	double TileBehavior::TryStepMoveTile(
		Tilemap* pTilemap,
		Vector2 tilePosition,
		Vector2 direction,
		double end)
	{
		std::pair<double, TilemapContainer::TILE_INFO> hit = pTilemap->Raycast(tilePosition, direction, end);
		if (hit.first < 1)
			return 0;
		// Raycast succeeded !

		Vector2 newTilePosition = direction * hit.first + tilePosition;
		pTilemap->SwapTiles(tilePosition, newTilePosition);
		return hit.first;
	}

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
		Time time,
		TileActionTypes action)
	{
		// Need to update adjacent if the action applied on this tile is Add or Remove
		return (TILE_ACTION_ADD | TILE_ACTION_REMOVE) & action;
	}
}