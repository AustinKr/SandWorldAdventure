#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/RedrawTileAction.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	RedrawTileAction::RedrawTileAction()
	{
		ActionType = TileActionType::Redraw;
	}

	bool RedrawTileAction::Register(Tilemap* pTilemap, Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pCurrentQueue)
	{
		return BaseTileAction::Register(pTilemap, tilePosition, pCurrentQueue);
	}

	void RedrawTileAction::Apply(Tilemap* pTilemap, Vector2Int tilePosition, Time time, TileActionQueue::ACTION_QUEUE* pCurrentActionQueue)
	{
		TilemapContainer::TILE_INFO tileInfo = pTilemap->Container.GetTileInChunk(tilePosition);

		// Update bitmap
		if (tileInfo.second != nullptr && tileInfo.first->Bitmap.has_value())
			TilemapRenderer::ChangeTileData(pTilemap, tilePosition.X, tilePosition.Y, tileInfo.second->HasValue ? tileInfo.second->Color : TILEMAP_BITMAP_NULL_TILE_COLOR);

	}

	void RedrawTileAction::Release()
	{
		delete((RedrawTileAction*)this);
	}
}
