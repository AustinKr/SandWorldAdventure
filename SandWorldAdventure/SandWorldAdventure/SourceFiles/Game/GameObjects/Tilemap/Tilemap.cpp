#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/BaseTileAction.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/RemoveTileAction.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/AddTileAction.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/SwapTileAction.h"

#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h" // Needed for IMesh.h
#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/RenderLayerNames.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	Tilemap::Tilemap()
	{
		p_Mesh = new GraphicsPipeline::TilemapMesh(this);
		GameInstance::Pipeline.GetLayer(RenderLayerNames::RENDERLAYERS_Tilemap0).RegisterMesh(p_Mesh);

		ActionQueueInstance = {};
		Container = {};
		Container.AssignChunks(Vector2Int());
		Position = Vector2(0, 0);
		TileSize = Vector2(0, 0);
	}
	Tilemap::Tilemap(Vector2Int chunkBounds)
	{
		p_Mesh = new GraphicsPipeline::TilemapMesh(this);
		GameInstance::Pipeline.GetLayer(RenderLayerNames::RENDERLAYERS_Tilemap0).RegisterMesh(p_Mesh);

		ActionQueueInstance = {};
		Container = {};
		Container.AssignChunks(chunkBounds);
		Position = Vector2(0, 0);
		TileSize = Vector2(0, 0);
	}

	// Inherited via IGameObject
	void Tilemap::Update(Time time)
	{
		//TileActionQueue::BaseTileAction::ACTION_QUEUE newActionQueue = {};
		//TileActionQueue::BaseTileAction::ACTION_QUEUE::const_iterator queueIterator;
		//MasterWindow::LogToConsole(L"actionQueue: " + std::to_wstring(m_ActionQueue.size()) + L"; tiles: " + std::to_wstring(Container.GetTilesDimensions().X * Container.GetTilesDimensions().Y) + L"\n");
		//for (queueIterator = m_ActionQueue.cbegin(); queueIterator != m_ActionQueue.cend(); queueIterator++)
		//{
		//	// TODO: Remove debug feature
		//	IGameObject* p_debugService = GameInstance::Layers[0].Objects["DebugService"];
		//	if (p_debugService != nullptr)
		//	{
		//		Vector2 tileScreenPos = GameInstance::MainCamera.FromViewportToScreen(GameInstance::MainCamera.FromWorldToViewport(FromTileToWorld(queueIterator->first)));
		//		Vector2 tileScreenSize = GameInstance::MainCamera.FromViewportToScreen(GameInstance::MainCamera.FromWorldToViewport(TileSize, false), false);
		//		Renderer::DrawRectPixels(&((GameObject::DebugObject*)p_debugService)->DebugScreen, tileScreenPos.X, tileScreenPos.Y, tileScreenSize.X, tileScreenSize.Y, 0xff00f9);
		//	}

		//	// Apply and unregister
		//	((TileActionQueue::BaseTileAction*)queueIterator->second)->Apply(this, queueIterator->first, &newActionQueue, time);
		//	delete(queueIterator->second);
		//}
		//m_ActionQueue.clear();
		//m_ActionQueue = newActionQueue;



		// With this implementation, tile behaviors do not immediately react to changes

		// Update the tiles and adjacent tiles of those actions
		TileActionQueue::TileActionQueue::ACTION_QUEUE tileBehaviorActionQueue = {};
		int updated = 0;
		ActionQueueInstance.UpdateAdjacentTiles(ActionQueueInstance.CommitActions(this, time), &tileBehaviorActionQueue, this, time, &updated);
		
		//if (!ActionQueueInstance.UpdateTilesFromActionQueue(ActionQueueInstance.CommitActions(this, time), &tileBehaviorActionQueue, this, time, &updated))
		//{
		//	// Did not make it through all actions, so there is possibilty that they did not get released
		//	TileActionQueue::TileActionQueue::Release(ActionQueueInstance.GetQueue()); // could also just use ActionQueueInstance.Release(), or stay with the explicit expression
		//}

		// Override the current action queue( the one that was just applied) with the tile behavior action queue that will be applied next cycle
		*ActionQueueInstance.GetQueue() = std::move(tileBehaviorActionQueue);
	}
	// Inherited via IGameObject
	void Tilemap::Release()
	{
		TileBehavior::ReleaseTileBehaviors();
		ActionQueueInstance.Release();

		if (p_Mesh != nullptr)
		{
			GameInstance::Pipeline.GetLayer(RenderLayerNames::RENDERLAYERS_Tilemap0).UnregisterMesh(p_Mesh);
			p_Mesh = nullptr;
		}
	}

	bool Tilemap::AddTile(Vector2Int tilePosition, TileActionQueue::AddTileActionArgument arguments, TileActionQueue::TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		TileActionQueue::AddTileAction* pTileAction = new TileActionQueue::AddTileAction(arguments);
		return TileActionQueue::TileActionQueue::RegisterTileAction(this, tilePosition, pTileAction, pActionQueue != nullptr ? pActionQueue : ActionQueueInstance.GetQueue());
	}
	bool Tilemap::RemoveTile(Vector2Int tilePosition, TileActionQueue::BaseTileActionArgument arguments, TileActionQueue::TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		TileActionQueue::RemoveTileAction* pTileAction = new TileActionQueue::RemoveTileAction(arguments);
		return TileActionQueue::TileActionQueue::RegisterTileAction(this, tilePosition, pTileAction, pActionQueue != nullptr ? pActionQueue : ActionQueueInstance.GetQueue());
	}

	bool Tilemap::SwapTiles(TileActionQueue::SwapTileActionArgument swapArguments, TileActionQueue::TileActionQueue::ACTION_QUEUE* pActionQueue)
	{
		TileActionQueue::SwapTileAction* pTileAction = new TileActionQueue::SwapTileAction(swapArguments);
		return pTileAction->Register(this, {}, pActionQueue != nullptr ? pActionQueue : ActionQueueInstance.GetQueue());
		
		//// If there is an add action at A or B, that should be used instead of the tile in the container? The answer is no because this would complicate things and is really more of a preference.

		//// These check if it is within bounds
		//Tile* pTile1 = Container.GetTileInChunk(positionA.X, positionA.Y);
		//if (pTile1 == nullptr)
		//	return false;
		//Tile* pTile2 = Container.GetTileInChunk(positionB.X, positionB.Y);
		//if (pTile2 == nullptr)
		//	return false;

		//// Edge cases
		//if ((!pTile1->HasValue && !pTile2->HasValue) ||
		//	(positionA == positionB))
		//	return false;

		//if (!forceSwap)
		//{
		//	// Make sure we dont override an existing AddAction
		//	std::optional<bool> tile1Exists = ActionQueueInstance.WillContainTile(positionA, pActionQueue);
		//	std::optional<bool> tile2Exists = ActionQueueInstance.WillContainTile(positionB, pActionQueue);
		//	if ((tile1Exists.has_value() && tile1Exists.value()) ||
		//		(tile2Exists.has_value() && tile2Exists.value()))
		//		return false;
		//}

		//// These will override any existing action
		//AddTile(positionA.X, positionA.Y, time, TileActionQueue::AddTileActionArgument(true, *pTile2, true, false), pActionQueue); //TODO: WHat about empty tiles?
		//AddTile(positionB.X, positionB.Y, time, TileActionQueue::AddTileActionArgument(true, *pTile1, true, false), pActionQueue);

		//return true;
	}

	std::pair<double, TilemapContainer::TILE_INFO> Tilemap::Raycast(Vector2 origin, Vector2 direction, double end)
	{
		// TODO: Implement effecient grid raycast method

		Vector2 currentPosition;
		double distance = 0;
		TilemapContainer::TILE_INFO hitTileInfo = std::make_pair(nullptr, nullptr);
		for (distance = 1; distance < floor(end) + 1 && distance < MAX_RAYCAST_STEPS; distance++)
		{
			currentPosition = origin + Vector2(.5, .5) + direction * distance;
			hitTileInfo = Container.GetTileInChunk(currentPosition);
			if (hitTileInfo.second == nullptr || hitTileInfo.second->HasValue)
				return std::make_pair(distance - 1, hitTileInfo);
		}
		return std::make_pair(distance, hitTileInfo); // Should always be 0, nullptr
	}

	// TODO: This can be Vector2Int
	Vector2 Tilemap::FromWorldToTile(Vector2 world)
	{
		world -= Position;
		world /= TileSize;
		return Vector2(floor(world.X), floor(world.Y));
	}
	Vector2 Tilemap::FromTileToWorld(Vector2 tile)
	{
		tile *= TileSize;
		tile += Position;
		return tile;
	}
}