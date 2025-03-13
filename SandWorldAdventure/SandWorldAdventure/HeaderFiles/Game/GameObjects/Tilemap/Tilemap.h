#pragma once

#include "HeaderFiles/Vector2Hasher.h"
#include "HeaderFiles/Game/GameObjects/IGameObject.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/TilemapRenderInfo.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TilemapContainer.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionQueue.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/ActionArguments/AddTileActionArgument.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/Actions/ActionArguments/SwapTileActionArgument.h"
#include <unordered_map>

#define TILEMAP_H
namespace SandboxEngine::Game::GameObject::Tilemap
{
	const int MAX_RAYCAST_STEPS = 600; // TODO: Move this into tilemap

	class Tilemap : public IGameObject
	{
	public:
		TilemapRenderInfo RenderInformation;
		TileActionQueue::TileActionQueue ActionQueueInstance;
		TilemapContainer Container;
		Vector2 Position;
		Vector2 TileSize;

	public:
		Tilemap(Vector2Int chunkBounds);
		Tilemap();

		/// <summary>
		///  Inherited via IGameObject
		/// Trys to update all chunks and tiles
		/// </summary>
		virtual void Update(Time time) override;
		// Inherited via IGameObject
		virtual void CopyScreen(Render::ScreenState* pMainScreen) override;
		// Inherited via IGameObject
		virtual void Release() override;

		bool AddTile(Vector2Int tilePosition, TileActionQueue::AddTileActionArgument arguments = {}, TileActionQueue::TileActionQueue::ACTION_QUEUE* pActionQueue = nullptr);
		bool RemoveTile(Vector2Int tilePosition, TileActionQueue::BaseTileActionArgument arguments = {}, TileActionQueue::TileActionQueue::ACTION_QUEUE* pActionQueue = nullptr);
		bool SwapTiles(TileActionQueue::SwapTileActionArgument swapArguments, TileActionQueue::TileActionQueue::ACTION_QUEUE* pActionQueue = nullptr);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="origin"></param>
		/// <param name="direction"></param>
		/// <param name="end"></param>
		/// <returns>the distance traveled before it hit the tile(or null) in front which is returned through tile info</returns>
		std::pair<double, TilemapContainer::TILE_INFO> Raycast(Vector2 origin, Vector2 direction, double end);
		
		Vector2 FromWorldToTile(Vector2 world);
		Vector2 FromTileToWorld(Vector2 tile);

	};
}

// GOOD FOR NEW IMPLEMENTATION