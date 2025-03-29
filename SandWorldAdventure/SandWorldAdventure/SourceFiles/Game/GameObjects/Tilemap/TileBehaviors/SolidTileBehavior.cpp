#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SolidTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/BaseTileAction.h" // for tileactionqueue.h and this .cpp
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionType.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	bool SolidTileBehavior::TryUpdate(
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

		// Update bitmap
		if (pChunk->Bitmap.has_value())
			TilemapRenderer::ChangeTileData(pTilemap, tilePosition.X, tilePosition.Y, pTile->HasValue ? pTile->Color : TILEMAP_BITMAP_NULL_TILE_COLOR);
		
		// Give adjacent tiles to update with base behavior
		return TileBehavior::TryUpdate(pTilemap, tilePosition, pTile, pChunk, pTileBehaviorActionQueue, tileActionArguments, time);
	}
}