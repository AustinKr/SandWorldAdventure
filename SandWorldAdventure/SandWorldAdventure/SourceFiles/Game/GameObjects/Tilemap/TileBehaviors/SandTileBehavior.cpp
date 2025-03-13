#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SandTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/BaseTileAction.h" // for tileactionqueue.h and this .cpp
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TilemapRenderer.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionType.h"
#include "HeaderFiles/Vector2Int.h" // for tilemap.h and tileactionqueue.h
#include "HeaderFiles/Vector2.h" // for tilemap.h and tileactionqueue.h

namespace SandboxEngine::Game::GameObject::Tilemap
{
	bool SandTileBehavior::TryUpdate(
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

		// Update bitmap (order doesnt matter in this case)
		if (pChunk->Bitmap.has_value())
			TilemapRenderer::ChangeTileData(pTilemap, tilePosition.X, tilePosition.Y, pTile->HasValue ? pTile->Color : TILEMAP_BITMAP_NULL_TILE_COLOR);

		if (!pTilemap->Container.ContainsTile(tilePosition - Vector2Int(0, 1)) && pTilemap->SwapTiles(TileActionQueue::SwapTileActionArgument(true, tilePosition, tilePosition - Vector2(0, 1), false), pTileBehaviorActionQueue))
			return true;
		return TileBehavior::TryUpdate(pTilemap, tilePosition, pTile, pChunk, pTileBehaviorActionQueue, tileActionArguments, time);

		////TODO: Can probably simplify this check
		//// Basically, it just checks if a swap is valid
		//// If the tile is empty, the tile below isn't within bounds, the tilemap contains a tile at the below position and there wont be a tile removed from there, or the register a remove tile action fails, then return base behavior.
		//if (!pTile->HasValue ||
		//	!pTilemap->Container.IsTileInBounds(tilePosition - Vector2Int(0, 1)) ||
		//	(pTilemap->Container.ContainsTile(tilePosition - Vector2Int(0, 1))
		//		/*&& pTilemap->ActionQueueInstance.WillContainTile(Vector2Int(tilePosition.X, tilePosition.Y - 1), pTileBehaviorActionQueue) != false*/ // shouldnt need to check
		//		)
		//	|| !pTilemap->RemoveTile(tilePosition.X, tilePosition.Y, time, {}, pTileBehaviorActionQueue))
		//	return TileBehavior::TryUpdate(pTilemap, tilePosition, pTile, pChunk, pTileBehaviorActionQueue, tileActionArguments, time);
		//// Otherwise, add the tile in the below position and return true

		//if (!pTilemap->AddTile(tilePosition.X, tilePosition.Y - 1, time,
		//	TileActionQueue::AddTileActionArgument(true, *pTile, false, false),
		//	pTileBehaviorActionQueue))
		//{
		//	TileActionQueue::TileActionQueue::UnregisterTileAction(tilePosition, pTileBehaviorActionQueue); // Unregister the remove
		//	return TileBehavior::TryUpdate(pTilemap, tilePosition, pTile, pChunk, pTileBehaviorActionQueue, tileActionArguments, time);
		//}

		//// Swap succeeded
		//return true;




		//// Time
		//double deltaTime = time.CurrentTime - pTile->LastMoveTime;

		//// Gravity with a little deviation
		//pTile->Motion.X -= 0.5 + ((double)rand() / RAND_MAX) * 0.0;
		//pTile->Motion.Y -= 0.5 + ((double)rand() / RAND_MAX) * 0.0;

		//Vector2 translation = pTile->Motion * deltaTime;
		//double end = sqrt(translation.GetMagnitudeSqrd());
		//Vector2 direction = translation / end;
		//double distance = TileBehavior::TryStepMoveTile(pTilemap, pTile, tilePosition, direction, end, time, pTileBehaviorActionQueue);
		//if (distance >= 1)
		//{
		//	// Move succeeded !
		//	Vector2 newTilePosition = direction * distance + tilePosition;

		//	pTile->LastMoveTime = time.CurrentTime;

		//	if (pAdjacentTiles != nullptr)
		//	{
		//		// Update adjacent tiles around root position and new position
		//		std::vector<Vector2> adjacentFromRootPosition = TileBehavior::CreateTileUpdateGroup(tilePosition);
		//		std::vector<Vector2> adjacentFromNewPosition = TileBehavior::CreateTileUpdateGroup(tilePosition + direction * distance);
		//		adjacentFromRootPosition.insert(adjacentFromRootPosition.end(), adjacentFromNewPosition.begin(), adjacentFromNewPosition.end());
		//TODO: There is overlap in the update groups
		//		*pAdjacentTiles = adjacentFromRootPosition;
		//	}
		//}

		//// If something was forced to happen to this tile, return surroundings to try to update
		//if (pAdjacentTiles != nullptr && pTileAction != nullptr && (TileActionQueue::TileActionType::Add & pTileAction->ActionType || TileActionQueue::TileActionType::Remove & pTileAction->ActionType))
		//{
		//	*pAdjacentTiles = TileBehavior::CreateTileUpdateGroup(tilePosition);
		//}
	}
}