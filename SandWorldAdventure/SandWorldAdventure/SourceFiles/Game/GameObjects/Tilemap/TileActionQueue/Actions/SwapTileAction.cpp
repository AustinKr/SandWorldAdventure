#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/SwapTileAction.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"


namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	// TODO:  update other actions code to this new standard(action pointers) and swap function and this too

	SwapTileAction::SwapTileAction(SwapTileActionArgument arguments)
	{
		ActionType = TileActionType::Swap;

		ShouldUpdateAdjacent = arguments.ShouldUpdateAdjacent;

		ActionPointerA = std::make_pair(arguments.TilePositionA, (TileActionPointer*)nullptr);
		ActionPointerB = std::make_pair(arguments.TilePositionB, (TileActionPointer*)nullptr);
		m_CurrentTileInfoA = std::make_pair((Chunk*)nullptr, (Tile*)nullptr);
		m_CurrentTileInfoB = std::make_pair((Chunk*)nullptr, (Tile*)nullptr);
		ForceSwap = arguments.ForceSwap;
	}

	bool SwapTileAction::Register(Tilemap* pTilemap, Vector2Int, TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		// Create pointer actions that are stored in the queue but point to this.
		
		// TODO: Have swap functions interact with action pointers, and other actions and swaps(possibly through this reduction method which I believe always produces  one (add)/[remove] and another (add)/[remove].)

		if (ActionPointerA.first == ActionPointerB.first)
		{
			delete((SwapTileAction*)this);
			return false;
		}
		
		// These check if it is within bounds
		m_CurrentTileInfoA = pTilemap->Container.GetTileInChunk(ActionPointerA.first);
		if (m_CurrentTileInfoA.second == nullptr)
		{
			delete((SwapTileAction*)this);
			return false;
		}
		m_CurrentTileInfoB = pTilemap->Container.GetTileInChunk(ActionPointerB.first);
		if (m_CurrentTileInfoB.second == nullptr || (!m_CurrentTileInfoA.second->HasValue && !m_CurrentTileInfoB.second->HasValue)) // Make sure that both tiles are not empty
		{
			delete((SwapTileAction*)this);
			return false;
		}
		
		if (!ForceSwap)
		{
			// Make sure we dont override an existing AddAction
			std::optional<bool> tileAExists = pTilemap->ActionQueueInstance.WillContainTile(ActionPointerA.first, pActionQueue);
			std::optional<bool> tileBExists = pTilemap->ActionQueueInstance.WillContainTile(ActionPointerB.first, pActionQueue);
			if ((tileAExists.has_value() && tileAExists.value()) ||
				(tileBExists.has_value() && tileBExists.value()))
			{
				delete((SwapTileAction*)this);
				return false;
			}
		}

		// Allocate on heap
		ActionPointerA.second = new TileActionPointer(this);
		ActionPointerB.second = new TileActionPointer(this);
		// Add pointers to queue. These will override any existing action
		pTilemap->ActionQueueInstance.RegisterTileAction(pTilemap, ActionPointerA.first, (BaseTileAction*)ActionPointerA.second, pActionQueue);
		pTilemap->ActionQueueInstance.RegisterTileAction(pTilemap, ActionPointerB.first, (BaseTileAction*)ActionPointerB.second, pActionQueue);
		
		return true;
	}
	void SwapTileAction::Unregister(Vector2Int, TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		// We need to make sure that delete(this) is called last, because otherwise we would be accessing data(ActionPointerA and B) that is deleted.

		// Set their pointers to nullptr
		ActionPointerA.second->pAction = (const BaseTileAction*)nullptr;
		ActionPointerB.second->pAction = (const BaseTileAction*)nullptr;
		ActionPointerA.second->ShouldUpdateAdjacent = false;
		ActionPointerB.second->ShouldUpdateAdjacent = false;

		// Now erase from queue
		ActionPointerA.second->Unregister(ActionPointerA.first, pActionQueue);
		ActionPointerA.second = nullptr;

		ShouldUpdateAdjacent = false;
		ActionPointerB.second->Unregister(ActionPointerB.first, pActionQueue);
		ActionPointerB.second = nullptr;

		// Now delete this
		delete((SwapTileAction*)this);
	}

	void SwapTileAction::Apply(Tilemap* pTilemap, Vector2Int, Time time, TileActionQueue::ACTION_QUEUE* pCurrentActionQueue)
	{
		// If there is an add action at A or B, should that be used instead of the tile in the container? The answer is no because this would complicate things and is really more of a preference.
		pTilemap->Container.SwapTiles(m_CurrentTileInfoA, ActionPointerA.first, m_CurrentTileInfoB, ActionPointerB.first);
		
	}
	void SwapTileAction::Release()
	{
		ActionPointerA.second->pAction = (const BaseTileAction*)nullptr;
		ActionPointerB.second->pAction = (const BaseTileAction*)nullptr;
		delete((SwapTileAction*)this);
	}
}