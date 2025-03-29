#pragma once
#include <unordered_map>
#include <optional>
#include "HeaderFiles/Math.h"
#include "HeaderFiles/Time.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/ActionArguments/BaseTileActionArgument.h"

#ifndef TILEMAP_H
#define TILEMAP_H

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class Tilemap;
	//{
	//	// Inherited via IGameObject
	//	virtual void Update(Time time) override;
	//	// Inherited via IGameObject
	//	virtual void CopyScreen(Render::ScreenState* pMainScreen) override;
	//	// Inherited via IGameObject
	//	virtual void Release() override;
	//};
}
#endif


#define TILE_ACTION_QUEUE_H
namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
#ifndef BASE_TILE_ACTION_H
#define BASE_TILE_ACTION_H
	class BaseTileAction;
#endif
	
	/// <summary>
	/// Basicallly a wrapper on the unordered_map actionQueue
	/// </summary>
	class TileActionQueue
	{
	public:
		typedef std::unordered_map<Vector2Int, const BaseTileAction*, Vector2Hasher> ACTION_QUEUE;
		// A vector that contains a key and an action
		typedef std::vector<std::pair<Vector2Int, BaseTileActionArgument>> ACTION_QUEUE_COPY;

	private:
		static const std::vector<Vector2Int> TILE_UPDATE_GROUP;
		// Just a helper function that will append tilePosition to each position of the TILE_UPDATE_GROUP
		static std::vector<Vector2Int> CreateUpdateGroup(Vector2Int tilePosition);

		// Calls behavior.update() and adds to the adjacenttilesvector from an action
		void TryUpdateTile(ACTION_QUEUE_COPY* pAdjacentTiles, ACTION_QUEUE* pTileBehaviorActionQueue, Tilemap* pTilemap, std::pair<Vector2Int, BaseTileActionArgument> actionPair, std::pair<Chunk*, Tile*> tileInfo, Time time);
	public:
		static const int MAX_TILES_PER_ACTION_QUEUE;
		static const int MAX_ADJACENT_TILES;

		TileActionQueue();

		// TODO: Could add Try prefix to these two registry functions
		
		/// <summary>
		/// Used for every external operation on tiles. Redrawing, adding, removing, etc. You should use this function instead of directly calling register from the actions because this function uses the limit
		/// </summary>
		/// <param name="position">position of tile to act on</param>
		/// <param name="pAction">memory allocated on heap that points to an action</param>
		/// <param name="pActionQueue">The action queue to use</param>
		/// <returns></returns>
		static bool RegisterTileAction(Tilemap* pTilemap, Vector2Int tilePosition, const BaseTileAction* pAction, ACTION_QUEUE* pQueue);
		// Trys to cleans up memeory and removes from queue by called BaseTileAction*->Unregister()
		static bool UnregisterTileAction(Vector2Int tilePosition, ACTION_QUEUE* pQueue);
		
		ACTION_QUEUE* GetQueue();

		/// <summary>
		/// Since the record of tiles being removed or added is the update queue, this along with the current tile value in the container perhaps must be checked if the state of a tile is desired
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <returns>Undefined means there is no action at the given key, true means a tile with .hasValue == true will exist, false means no tile or .hasValue == false</returns>
		static std::optional<bool> WillContainTile(Vector2Int tilePosition, ACTION_QUEUE* pQueue);
		std::optional<bool> WillContainTile(Vector2Int tilePosition);
		
		/// <summary>
		/// Commits actions, copies to a list, cleans up memory, and returns the list of actions to update. Does not clear the queue, only sets actions to nullptr
		/// </summary>
		/// <param name="pTilemap"></param>
		/// <param name="time"></param>
		/// <returns></returns>
		ACTION_QUEUE_COPY CommitActions(Tilemap* pTilemap, Time time);
		/// <summary>
		/// Loops through a collection, initiallly created through game logic, and updates the tiles. The tile will then return adjacent tiles that will be strung onto a collection to loop over.
		/// The action queue is used to check positions and the copy (param tilesToUpdate) initially represents the positions to update, and possibly an action that may have changed the tile which is just passed to the TileBehavior code.
		/// </summary>
		/// <param name="tilesToUpdate"> this should be a copy of the action queue (returned by CommitActions)</param>
		/// <param name="pTileBehaviorActionQueue">The tile behavior action queue that will replace the 'current' action queue and is for TileBehaviors to add too</param>
		/// <param name="pTilemap"></param>
		/// <param name="time"></param>
		/// <param name="pUpdated">should be pointer to a value that is 0</param>
		/// <returns>True if it updated all tiles, False if it reached the limit MAX_ADJACENT_TILES </returns>
		bool UpdateAdjacentTiles(
			ACTION_QUEUE_COPY tilesToUpdate,
			ACTION_QUEUE* pTileBehaviorActionQueue,
			Tilemap* pTilemap,
			Time time,
			int* pUpdated);

		static void Release(ACTION_QUEUE* pQueue);
		void Release();

	private:
		//TODO: Could make the update queue into something like a sparse quadtree
		ACTION_QUEUE m_CurrentActionQueue;
	};
}


// GOOD FOR NEW IMPLEMENTATION