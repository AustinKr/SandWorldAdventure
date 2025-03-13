#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/TileActionPointer.h"
#include "HeaderFiles/Vector2Int.h" // For forward decl from BaseTileAction.h
#include "HeaderFiles/Vector2.h" // For forward decl from BaseTileAction.h
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"


namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	TileActionPointer::TileActionPointer() { ActionType = TileActionType::ActionPointer; pAction = nullptr; ShouldUpdateAdjacent = false; }
	TileActionPointer::TileActionPointer(const BaseTileAction* p_action) { ActionType = TileActionType::ActionPointer; pAction = p_action; ShouldUpdateAdjacent = pAction->ShouldUpdateAdjacent;}

	bool TileActionPointer::Register(Tilemap* pTilemap, Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		pTilemap->ActionQueueInstance.UnregisterTileAction(tilePosition, pActionQueue);
		return BaseTileAction::Register(pTilemap, tilePosition, pActionQueue);
	}
	void TileActionPointer::Unregister(Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		if (pAction != nullptr)
			return ((BaseTileAction*)pAction)->Unregister(tilePosition, pActionQueue);
		BaseTileAction::Unregister(tilePosition, pActionQueue);
	}

	void TileActionPointer::Apply(Tilemap* pTilemap, Vector2Int tilePosition, Time time, TileActionQueue::ACTION_QUEUE* pCurrentActionQueue)
	{
		if (pAction != nullptr)
			((BaseTileAction*)pAction)->Apply(pTilemap, tilePosition, time, pCurrentActionQueue);
		
	}
	void TileActionPointer::Release()
	{
		if (pAction != nullptr)
			((BaseTileAction*)pAction)->Release();

		pAction = nullptr;
		ShouldUpdateAdjacent = false;
		delete((TileActionPointer*)this);
	}
}