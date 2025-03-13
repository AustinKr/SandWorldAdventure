#pragma once
#include "BaseTileAction.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	class RedrawTileAction : public BaseTileAction
	{
	public:
		RedrawTileAction();
		
		virtual bool Register(Tilemap* pTilemap, Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue) override;
		virtual void Apply(Tilemap* pTilemap, Vector2Int tilePosition, Time time, TileActionQueue::ACTION_QUEUE* pCurrentActionQueue) override;
		virtual void Release() override;
	};
}

// GOOD FOR NEW IMPLEMENTATION