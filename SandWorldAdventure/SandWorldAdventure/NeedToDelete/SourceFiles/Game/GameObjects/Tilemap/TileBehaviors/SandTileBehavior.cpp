#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SandTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	bool SandTileBehavior::TryApplyMotion(
		Vector2 translation,
		Tilemap* pTilemap,
		Vector2Int tilePosition,
		TilemapContainer::TILE_INFO tileInfo)
	{
		//Realistic movement
		// Try move
		bool succeeded = true;
		Vector2Int hitPosition = pTilemap->TryStepMoveTile(tilePosition, translation, &succeeded);
		if (succeeded)
			return true;

		// Try alternate pathway (by rotating motion line)
		int x = int(((double)rand() / (double)RAND_MAX) * 2) - 1;
		double angle = 3.14 / 4.0;
		Vector2 motionRotated = { tileInfo.second->Motion.X * cos(angle * x) - tileInfo.second->Motion.Y * sin(angle * x),
								  tileInfo.second->Motion.X * sin(angle * x) + tileInfo.second->Motion.Y * cos(angle * x) };
		pTilemap->TryStepMoveTile(tilePosition, motionRotated, &succeeded);
		if (succeeded)
			return true;

		// Try opposite direction
		motionRotated = { tileInfo.second->Motion.X * cos(angle * -x) - tileInfo.second->Motion.Y * sin(angle * -x),
						  tileInfo.second->Motion.X * sin(angle * -x) + tileInfo.second->Motion.Y * cos(angle * -x) };
		pTilemap->TryStepMoveTile(tilePosition, motionRotated, &succeeded);
		if (succeeded)
			return true;

		// Failed to move because something was in the way and absorbed the momentum
		tileInfo.second->Motion = {};
		return false;
	}

	bool SandTileBehavior::TryUpdate(
		Tilemap* pTilemap,
		Vector2Int tilePosition,
		TilemapContainer::TILE_INFO tileInfo,
		Time time)
	{
		// Framerate dependent movement
		Vector2Int swapPosition = tilePosition - Vector2Int(0, 1);
		if (!pTilemap->Container.ContainsTile(swapPosition) || pTilemap->PendingTileChangesRegistry.WillRemoveTile(swapPosition))// TODO: Get fast way to retrieve tiles
		{
			// Swap
			pTilemap->PendingTileChangesRegistry.SwapTiles(tilePosition, swapPosition);
			return true;
		}
		return false;
		
		/*
		// Time
		double deltaTime = time.CurrentTime - tileInfo.second->LastMoveTime;

		// Gravity with a little deviation
		tileInfo.second->Motion.X -= 0.5 + ((double)rand() / RAND_MAX) * 0.0;
		tileInfo.second->Motion.Y -= (double)GRAVITY * deltaTime;// +((double)rand() / RAND_MAX) * 0.0;

		Vector2 translation = tileInfo.second->Motion * deltaTime; // Movement of the tile
		if (abs(translation.X) < 1 && abs(translation.Y) < 1) // Is there enough momentum to actually move?
		{
			pTilemap->PendingTileChangesRegistry.TryRefreshTile(tilePosition); // Try again next frame (when more time has elapsed)
			return false;
		}
		tileInfo.second->LastMoveTime = time.CurrentTime - .5; // Update time
		return TryApplyMotion(translation, pTilemap, tilePosition, tileInfo); // Apply motion */
	}
}