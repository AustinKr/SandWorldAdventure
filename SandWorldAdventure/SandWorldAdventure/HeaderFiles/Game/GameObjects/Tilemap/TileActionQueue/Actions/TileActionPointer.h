#pragma once
#include "BaseTileAction.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	// TODO: Make this and the action queue recognize and use action pointers. (Also update other actions code to this new standard and swap function)
	
	// An action to be used in an action queue as a placeholder that allows the actual action it points to occupy multiple positions in the map
	class TileActionPointer : public BaseTileAction
	{
	public:
		const BaseTileAction* pAction;
		TileActionPointer();
		TileActionPointer(const BaseTileAction* p_action);

		//TODO: Need a way to reduce action pointers back into basic actions, (add and remove)?

		// Unregisters any existing action in place of this one and inserts into the queue
		virtual bool Register(Tilemap* pTilemap, Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue) override;
		// Calls pAction->Unregister() which is responsible for removing all associated pointers and actions. If pAction == nullptr, calls base behavior
		virtual void Unregister(Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue) override;

		// Try to apply the pAction.
		virtual void Apply(Tilemap* pTilemap, Vector2Int tilePosition, Time time, TileActionQueue::ACTION_QUEUE* pCurrentActionQueue) override;
		// Calls pAction->Release() and delete(this) to release the associated pAction set other pointers to nullptr
		virtual void Release() override;
	};
}