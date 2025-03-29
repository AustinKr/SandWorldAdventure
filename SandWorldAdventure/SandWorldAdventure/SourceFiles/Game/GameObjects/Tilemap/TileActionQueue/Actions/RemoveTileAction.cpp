#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/RemoveTileAction.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	RemoveTileAction::RemoveTileAction(BaseTileActionArgument arguments)
	{
		ActionType = TileActionType::Remove;
		ShouldUpdateAdjacent = arguments.ShouldUpdateAdjacent;
	}

	bool RemoveTileAction::Register(Tilemap* pTilemap, Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		const BaseTileAction** ppExistingAct = nullptr;
		bool tileAddActionExists = false;
		if (pActionQueue->contains(tilePosition))
		{
			ppExistingAct = &pActionQueue->at(tilePosition);
		}
		if(ppExistingAct != nullptr)
		{
			if (*ppExistingAct != nullptr && ((*ppExistingAct)->ActionType & (TileActionType::Add | TileActionType::AddBlock)))
			{
				tileAddActionExists = true;
			}
			// Unregister any unecessary operation of adding a tile (or any other action)	
			pTilemap->ActionQueueInstance.UnregisterTileAction(tilePosition, pActionQueue);
		}
		if (!tileAddActionExists) // TODO: This may, in some cases, have removed an action and yet failed this check so that action was never actually applied
		{
			TilemapContainer::TILE_INFO existingTileInfo = pTilemap->Container.GetTileInChunk(tilePosition);
			if (existingTileInfo.second == nullptr || !existingTileInfo.second->HasValue)
			{
				Release();
				return false;
			}
		}

		return BaseTileAction::Register(pTilemap, tilePosition, pActionQueue);
	}

	void RemoveTileAction::Apply(Tilemap* pTilemap, Vector2Int tilePosition, Time time, TileActionQueue::ACTION_QUEUE* pCurrentActionQueue)
	{
		pTilemap->Container.RemoveTile(tilePosition);
	}
	
	void RemoveTileAction::Release()
	{
		delete((RemoveTileAction*)this);
	}
}