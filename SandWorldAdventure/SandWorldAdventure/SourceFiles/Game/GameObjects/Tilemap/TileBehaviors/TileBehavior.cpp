#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/BaseTileAction.h" // for tileactionqueue.h
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SandTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SolidTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Vector2Int.h" // for tilemap.h and tileactionqueue.h
#include "HeaderFiles/Vector2.h" // for tilemap.h and tileactionqueue.h

namespace SandboxEngine::Game::GameObject::Tilemap
{
	const TileBehavior* TileBehavior::TILE_BEHAVIORS[2] =
	{
		(TileBehavior*)new SolidTileBehavior(),
		(TileBehavior*)new SandTileBehavior(),
	};

	double TileBehavior::TryStepMoveTile(
		Tilemap* pTilemap,
		Tile* pTile,
		Vector2 tilePosition,
		Vector2 direction,
		double end,
		Time time,
		TileActionQueue::TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		//std::pair<double, Tile*> raycastPair = pTilemap->Raycast(tilePosition, direction, end);
		//if (raycastPair.first == 0)
		//	return 0;

		//Vector2 translation = direction * raycastPair.first;
		//Vector2 otherTilePosition = tilePosition + Vector2(floor(translation.X), floor(translation.Y));
		//if (!pTilemap->SwapTiles(tilePosition.X, tilePosition.Y, otherTilePosition.X, otherTilePosition.Y, time, false, pActionQueue))
		//	return 0;
		//// Move succeeded !

		//return raycastPair.first;
		return -1;
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
		Tile* pTile,
		Chunk* pChunk,
		TileActionQueue::TileActionQueue::ACTION_QUEUE* pTileBehaviorActionQueue,
		TileActionQueue::BaseTileActionArgument tileActionArguments,
		Time time)
	{
		if (pTile == nullptr) // pTile should never be nullptr
			return false;

		// Need to update adjacent if the action applied on this tile is Add or Remove
		return (TileActionQueue::TileActionType::Add | TileActionQueue::TileActionType::Remove) & tileActionArguments.ActionType;
	}
}