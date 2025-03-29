#pragma once
#include <optional>
#include "Time.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionType.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Math.h"
#include <unordered_map>

#ifndef TILEMAP_H
#define TILEMAP_H
class SandboxEngine::Game::GameObject::Tilemap::Tilemap;
#endif

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
#define BASE_TILE_ACTION_H

	// The base tile action struct. By default does nothing when applied. This is meant to be abstract but with some default implemetation and should not be added to an action queue
	class BaseTileAction
	{
	public:
		// The type of action to interpret this instance* as. (All classes that derive from this one should set this property)
		TileActionType ActionType;

		// Should updates to adjacent tiles be included?
		bool ShouldUpdateAdjacent;

		/// <summary>
		/// By default tries to insert into queue and releases if it failed. Derived actions are responsible for delete(this) in this function
		/// </summary>
		/// <param name="pTilemap"></param>
		/// <param name="tilePosition"></param>
		/// <param name="pActionQueue"></param>
		/// <returns>whether it registered</returns>
		virtual bool Register(Tilemap* pTilemap, Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue);
		/// <summary>
		/// Used to unregister this action from the queue and any pointer actions associated with it. By default, calls this->Release() and removes from the queue
		/// </summary>
		/// <param name="tilePosition"></param>
		/// <param name="pActionQueue"></param>
		/// <returns></returns>
		virtual void Unregister(Vector2Int tilePosition, TileActionQueue::ACTION_QUEUE* pActionQueue);
		/// <summary>
		/// applies actual change in tilemap, try apply physics, and try change bitmap data
		/// </summary>
		/// <param name="pTilemap"></param>
		/// <param name="position"></param>
		/// <param name="pCurrentActionQueue">The current queue being iterated through</param>
		/// <param name="time"></param>
		/// <returns> The new tile info</returns>
		virtual void Apply(Tilemap* pTilemap, Vector2Int tilePosition, Time time, TileActionQueue::ACTION_QUEUE* pCurrentActionQueue) = 0;

		/// <summary>
		/// Used to clean up any data, with the appropriate cast to type, and set any pointer actions associated with it to nullptr
		/// </summary>
		virtual void Release() = 0;
	};
}

// GOOD FOR NEW IMPLEMENTATION