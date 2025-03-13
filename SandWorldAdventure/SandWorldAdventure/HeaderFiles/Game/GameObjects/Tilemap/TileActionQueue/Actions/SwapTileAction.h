#pragma once
#include "BaseTileAction.h"
#include "ActionArguments/SwapTileActionArgument.h"
#include "TileActionPointer.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	/// <summary>
	/// This action uses two pointer actions in the queue
	/// </summary>
	class SwapTileAction : public BaseTileAction
	{
	private:
		std::pair<Chunk*, Tile*> m_CurrentTileInfoA;
		std::pair<Chunk*, Tile*> m_CurrentTileInfoB;
	public:
		// TODO: THese members probably dont need to be public!!

		std::pair<Vector2Int, TileActionPointer*> ActionPointerA;
		std::pair<Vector2Int, TileActionPointer*> ActionPointerB;

		bool ForceSwap;

		SwapTileAction(SwapTileActionArgument arguments = {});
		//TODO: Need a way to reduce action pointers back into basic actions, (add and remove)

		// Note that this function does not utilize the tilePosition parameter
		virtual bool Register(Tilemap* pTilemap, Vector2Int, TileActionQueue::ACTION_QUEUE* pActionQueue) override;
		// Calls this->Release() and removes and releases pointer actions. Note that this function does not utilize the tilePosition parameter
		virtual void Unregister(Vector2Int, TileActionQueue::ACTION_QUEUE* pActionQueue) override; 
		
		/// Note that this function does not utilize the tilePosition parameter
		virtual void Apply(Tilemap* pTilemap, Vector2Int, Time time, TileActionQueue::ACTION_QUEUE* pCurrentActionQueue) override;
		// Releases this action and sets pointers to point to nullptr. This will not ever be called by CommitActions directly, but only through its TileActionPointers
		virtual void Release() override;
	};
}

// GOOD FOR NEW IMPLEMENTATION