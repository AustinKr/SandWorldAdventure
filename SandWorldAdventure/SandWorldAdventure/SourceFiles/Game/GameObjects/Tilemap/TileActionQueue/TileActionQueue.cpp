#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/BaseTileAction.h" // for forward declarations in TileActionQueue.h
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h" // for forward declarations in TileActionQueue.h
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"
#include "HeaderFiles/Game/GameObjects/DebugObject.h" 
#include "HeaderFiles/Render/Renderer.h" 
#include "HeaderFiles/Vector2Int.h"
#include "HeaderFiles/Vector2.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/Game/GameInstance.h"


namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	const std::vector<Vector2Int> TileActionQueue::TILE_UPDATE_GROUP = {
			Vector2Int(1,0),
			Vector2Int(0,1),
			Vector2Int(1,1),
			Vector2Int(-1,0),
			Vector2Int(0,-1),
			Vector2Int(-1,-1),
			Vector2Int(1,-1),
			Vector2Int(-1,1),
	};
	std::vector<Vector2Int> TileActionQueue::CreateUpdateGroup(Vector2Int tilePosition)
	{
		std::vector<Vector2Int> v = std::vector<Vector2Int>(TILE_UPDATE_GROUP.size(), {});
		for (int i = 0; i < TILE_UPDATE_GROUP.size(); i++)
		{
			v[i] = (Vector2Int)TILE_UPDATE_GROUP[i] + tilePosition;
		}
		return v;
	}
	void TileActionQueue::TryUpdateTile(ACTION_QUEUE_COPY* pAdjacentTiles, ACTION_QUEUE* pTileBehaviorActionQueue, Tilemap* pTilemap, std::pair<Vector2Int, BaseTileActionArgument> actionPair, std::pair<Chunk*, Tile*> tileInfo, Time time)
	{
		if (tileInfo.second == nullptr)
			tileInfo = pTilemap->Container.GetTileInChunk(actionPair.first);
		if (tileInfo.second == nullptr)
			return;

		bool shouldUpdateAdjacent = actionPair.second.ShouldUpdateAdjacent;
		if (actionPair.second.ActionType & TileActionType::Update) // Assumed to be true if the type is an update action
			shouldUpdateAdjacent = true;

		// Update the tile
		if (!TileBehavior::GetTileBehavior(tileInfo.second->BehaviorIndex)->TryUpdate(pTilemap, actionPair.first, tileInfo.second, tileInfo.first, pTileBehaviorActionQueue, actionPair.second, time)
			|| !shouldUpdateAdjacent) return;
		// There are adjacent tiles to also update

		std::vector<Vector2Int> adjacentTilePositions = CreateUpdateGroup(actionPair.first);

		// Try add adjacent tiles
		for (int i = 0; i < adjacentTilePositions.size(); i++)
		{
			// TODO: There is probably an optimization to be made here because of the limit you could terminate the loop here!! MAX_TILES_PER_ACTION_QUEUE

			// using nullptr in the queue as an update action

			auto insertion = m_CurrentActionQueue.insert(std::make_pair(adjacentTilePositions[i], nullptr)); // Will return false if anything already with that key
			if (insertion.second)
				pAdjacentTiles->push_back(std::make_pair(adjacentTilePositions[i], BaseTileActionArgument(TileActionType::Update, true))); // Actually add to the vector to iterate next
		}
	}

	const int TileActionQueue::MAX_TILES_PER_ACTION_QUEUE = 2000;
	const int TileActionQueue::MAX_ADJACENT_TILES = 800;

	TileActionQueue::TileActionQueue()
	{
		m_CurrentActionQueue = {};
	}


	bool TileActionQueue::RegisterTileAction(Tilemap* pTilemap, Vector2Int tilePosition, const BaseTileAction* pAction, TileActionQueue::ACTION_QUEUE* pQueue)
	{
		if (pQueue->size() >= MAX_TILES_PER_ACTION_QUEUE || tilePosition.X < 0 || tilePosition.Y < 0)
			return false;

		return ((BaseTileAction*)pAction)->Register(pTilemap, tilePosition, pQueue);
	}
	bool TileActionQueue::UnregisterTileAction(Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pQueue)
	{
		if (!pQueue->contains(tilePosition))
			return false;

		BaseTileAction* p = (BaseTileAction*)pQueue->at(tilePosition);
		if (p == nullptr)
		{
			pQueue->erase(tilePosition);
			return false;
		}
		p->Unregister(tilePosition, pQueue);
		return true;
	}


	TileActionQueue::ACTION_QUEUE* TileActionQueue::GetQueue()
	{
		return &m_CurrentActionQueue;
	}

	std::optional<bool> TileActionQueue::WillContainTile(Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pQueue)
	{
		if (!pQueue->contains(tilePosition) || pQueue->at(tilePosition) == nullptr)
			return {};

		TileActionType type = pQueue->at(tilePosition)->ActionType;
		if (type & (TileActionType::Add | TileActionType::AddBlock))
			return true;
		if (type & (TileActionType::Remove))
			return true;
		return {};
	}
	std::optional<bool> TileActionQueue::WillContainTile(Vector2Int tilePosition)
	{
		return WillContainTile(tilePosition, &m_CurrentActionQueue);
	}

	TileActionQueue::ACTION_QUEUE_COPY TileActionQueue::CommitActions(Tilemap* pTilemap, Time time)
	{
		//// Apply all the actions
		//ACTION_QUEUE::const_iterator queueIterator;
		//MasterWindow::LogToConsole(L"actionQueue: " + std::to_wstring(m_CurrentActionQueue.size()) + L"; tiles: " + std::to_wstring(pTilemap->Container.GetTilesDimensions().X * pTilemap->Container.GetTilesDimensions().Y) + L"\n");
		//for (queueIterator = m_CurrentActionQueue.cbegin(); queueIterator != m_CurrentActionQueue.cend(); queueIterator++)
		//{
		//	// TODO: Remove debug feature
		//	IGameObject* p_debugService = GameInstance::Layers[0].Objects["DebugService"];
		//	if (p_debugService != nullptr)
		//	{
		//		// TODO: Make debug functions to draw to a screen
		//		Vector2 tileScreenPos = GameInstance::MainCamera.FromViewportToScreen(GameInstance::MainCamera.FromWorldToViewport(pTilemap->FromTileToWorld(queueIterator->first)));
		//		Vector2 tileScreenSize = GameInstance::MainCamera.FromViewportToScreen(GameInstance::MainCamera.FromWorldToViewport(pTilemap->TileSize, false), false);
		//		Render::Renderer::DrawRectPixels(&((GameObject::DebugObject*)p_debugService)->DebugScreen, tileScreenPos.X, tileScreenPos.Y, tileScreenSize.X, tileScreenSize.Y, 0xff00f9);
		//	}

		//	// Apply
		//	((BaseTileAction*)queueIterator->second)->Apply(pTilemap, queueIterator->first, time, &m_CurrentActionQueue);
		//	// TODO: Have to delete actions as they are applied to make tile action pointers work
		//}



		
		// TODO: remove debug feature
		printf_s(("actionQueue: " + std::to_string(m_CurrentActionQueue.size()) + "; tiles: " + std::to_string(pTilemap->Container.GetTileBounds().X * pTilemap->Container.GetTileBounds().Y) + "\n").c_str());

		// apply actions, update tiles, and release memory as we go

		ACTION_QUEUE_COPY actions = ACTION_QUEUE_COPY(m_CurrentActionQueue.size()); // A copy of the action queue
		int index = 0;
		ACTION_QUEUE::const_iterator queueIterator;
		for (queueIterator = m_CurrentActionQueue.cbegin(); queueIterator != m_CurrentActionQueue.cend(); queueIterator++)
		{
			// TODO: Remove debug feature
			IGameObject* p_debugService = GameInstance::Layers[0].Objects["DebugService"];
			if (p_debugService != nullptr)
			{
				// TODO: Make debug functions to draw to a screen
				Vector2 tileScreenPos = GameInstance::MainCamera.FromViewportToScreen(GameInstance::MainCamera.FromWorldToViewport(pTilemap->FromTileToWorld(queueIterator->first)));
				Vector2 tileScreenSize = GameInstance::MainCamera.FromViewportToScreen(GameInstance::MainCamera.FromWorldToViewport(pTilemap->TileSize, false), false);
				Render::Renderer::DrawRectPixels(&((GameObject::DebugObject*)p_debugService)->DebugScreen, tileScreenPos.X, tileScreenPos.Y, tileScreenSize.X, tileScreenSize.Y, 0xff00f9);
			}

			if (queueIterator->second == nullptr) // Shouldnt ever be null in this stage. (unless I changed the design so that game logic may need to add these to the queue)
				continue;

			actions.at(index++) = std::make_pair(queueIterator->first, BaseTileActionArgument(queueIterator->second->ActionType, queueIterator->second->ShouldUpdateAdjacent)); // Copy
			
			((BaseTileAction*)queueIterator->second)->Apply(pTilemap, queueIterator->first, time, &m_CurrentActionQueue);

			((BaseTileAction*)queueIterator->second)->Release(); // Note: We have to release actions as they are applied to make tile action pointers work
			m_CurrentActionQueue[queueIterator->first] = nullptr; // so no corrupt memory is accessed
		}

		return actions; // Return list so that Tilemap may call TileActionQueue::UpdateAdjacentTiles initially with the action queue to loop over
	}
	bool TileActionQueue::UpdateAdjacentTiles(
		ACTION_QUEUE_COPY tilesToUpdate,
		ACTION_QUEUE* pTileBehaviorActionQueue,
		Tilemap* pTilemap,
		Time time,
		int* pUpdated)
	{
		/* . Go through each tile in elementsToLoopOver
		//		. UpdateTile, that adds to the behavior action queue
		//		. Try add adjacent tile postions to map and newElements, with nullptr as the action which represents an update
		// . Repeat for new list

	PSUDO CODE
	IN unordered_map actionQueue;
	list elementsToLoopOver = actionQueue.ToList()
	list newElements = {};

	loop through elementsToLoopOver
		Update(element, &newElements);
		if(!actionQueue.contains(key)
			newElements.add(...);
			actionQueue.insert(...);
		delete(element);

		if(thisIsLastElement)
			elementsToLoopOver.clear()
			elementsToLoopOver =move(newElements)
			newElements = {}
	*/

		std::pair<Vector2Int, BaseTileActionArgument>* pCurrent;
		int id = 0, count = 0;

		// Tiles that will replace the tilesToUpdate
		ACTION_QUEUE_COPY nextTiles = {};

		// Iterate through as much as needed to update adjacent tiles
		for (id = 0, count = tilesToUpdate.size(); id < count;)
		{
			pCurrent = &tilesToUpdate.at(id);

			// Behavior will add, to the end of the vector, any adjacent tiles

			// Update the tile
			TryUpdateTile(&nextTiles, pTileBehaviorActionQueue, pTilemap, *pCurrent, std::make_pair(nullptr, nullptr), time);

			if (++id >= count)
			{
				// Reset and startover with the nextTiles to update
				count = nextTiles.size();
				id = 0;

				tilesToUpdate.clear();
				tilesToUpdate = std::move(nextTiles);
				nextTiles = {};
			}

			// counts as updating a tile (even if null);
			if (++(*pUpdated) >= MAX_ADJACENT_TILES)
				return false; // TODO; this limit is easily hit and so many tiles do not get updated. Could possibly help if you didn't count null tiles, although tiles really shouldn't ever be null
		}
		return true;
	}

	void TileActionQueue::Release(ACTION_QUEUE* pQueue)
	{
		if (pQueue == nullptr)
			return;

		// Release actions
		for (auto&& queueIterator : *pQueue)
		{
			if (queueIterator.second != nullptr)
			{
				((BaseTileAction*)queueIterator.second)->Release(); // Let the action release the appropriate size of data as each may store differently sized blocks of memory
				queueIterator.second = nullptr; // Just a precaution
			}
		}

		pQueue->clear();
	}
	void TileActionQueue::Release()
	{
		Release(&m_CurrentActionQueue);

	}
}