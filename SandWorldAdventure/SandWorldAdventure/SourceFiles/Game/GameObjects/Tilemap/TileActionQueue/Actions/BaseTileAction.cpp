#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/BaseTileAction.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	bool BaseTileAction::Register(Tilemap* pTilemap, Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		// insert returns false if there is something in the place already
		if (!pActionQueue->insert(std::make_pair(tilePosition, (BaseTileAction*)this)).second)
		{
			Release();
			return false;
		}
		return true;
	}

	void BaseTileAction::Unregister(Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		Release();
		pActionQueue->erase(tilePosition);
	}
}