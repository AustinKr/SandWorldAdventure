#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/AddTileAction.h"
#include "HeaderFiles/Vector2Int.h"
#include "HeaderFiles/Vector2.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	AddTileAction::AddTileAction(AddTileActionArgument arguments)
	{
		ActionType = TileActionType::Add;
		
		ShouldUpdateAdjacent = arguments.ShouldUpdateAdjacent;
		
		NewTile = arguments.NewTile; NewTile.HasValue = true;
		ShouldOverrideExisting = arguments.ShouldOverrideExisting;
		ShouldExpandBounds = arguments.ShouldExpandBounds;
	}
	

	bool AddTileAction::Register(Tilemap* pTilemap, Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		const BaseTileAction** ppExistingAct = nullptr;
		if (pActionQueue->contains(tilePosition))
			ppExistingAct = &pActionQueue->at(tilePosition);

		if (!ShouldOverrideExisting)
		{
			// Make sure we don't override an existing add action or add over an existing tile

			// If the point the the action isn;t nullptr, the action stored isnt nullptr(meaning an update), and the action type of the action is add, then return false.
			if (ppExistingAct != nullptr && *ppExistingAct != nullptr && (*ppExistingAct)->ActionType & TileActionType::Add)
			{
				Release();
				return false;
			}
			// Return if there is an existing tile
			TilemapContainer::TILE_INFO existingTileInfo = pTilemap->Container.GetTileInChunk(tilePosition);
			if (existingTileInfo.second != nullptr && existingTileInfo.second->HasValue)
			{
				Release();
				return false;
			}
		}
		// Unregister unecessary operation as this should override anything already in place
		if (ppExistingAct != nullptr)
			TileActionQueue::UnregisterTileAction(tilePosition, pActionQueue);

		return BaseTileAction::Register(pTilemap, tilePosition, pActionQueue);
	}

	void AddTileAction::Apply(Tilemap* pTilemap, Vector2Int tilePosition, Time time, TileActionQueue::ACTION_QUEUE* pCurrentActionQueue)
	{
		pTilemap->Container.AddTile(tilePosition, NewTile, time.CurrentTime, ShouldExpandBounds);
	}

	void AddTileAction::Release()
	{
		delete((AddTileAction*)this);
	}
}