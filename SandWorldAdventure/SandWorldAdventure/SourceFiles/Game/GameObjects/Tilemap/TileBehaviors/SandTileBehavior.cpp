#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SandTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	bool SandTileBehavior::TryApplyMotion(
		Tilemap* pTilemap,
		Vector2Int tilePosition,
		TilemapContainer::TILE_INFO tileInfo,
		Time time)
	{
		double tileDeltaTime = time.CurrentTime - tileInfo.second->LastMoveTime;
								//+ (action & TileBehavior::TILE_ACTION_ADD ? 1 : 0); // +1 is so tiles immediately start falling
		Vector2 translation = tileInfo.second->Motion * tileDeltaTime;
		if (abs(translation.X) < 1 && abs(translation.Y) < 1) // Is there enough momentum to move?
		{
			pTilemap->PendingTileChangesRegistry.TryRefreshTile(tilePosition);
			return false;
		}

		// Try move
		bool succeeded = true;
		Vector2Int hitPosition = pTilemap->TryStepMoveTile(tilePosition, translation, &succeeded);
		if (succeeded)
			return true;

		// Try alternate pathway (by rotating motion line)
		int x = int(((double)rand() / (double)RAND_MAX) * 2) - 1;
		double angle = 3.14 / 3.0;
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
		if (tileInfo.second != nullptr && tileInfo.second->HasValue)
		{
			// Time
			double deltaTime = time.CurrentTime - tileInfo.second->LastMoveTime;

			// Gravity with a little deviation
			//pTile->Motion.X -= 0.5 + ((double)rand() / RAND_MAX) * 0.0;
			tileInfo.second->Motion.Y -= (double)GRAVITY / 10.0 * deltaTime;// +((double)rand() / RAND_MAX) * 0.0;

			return TryApplyMotion(pTilemap, tilePosition, tileInfo, time);
		}
		return false;
	}
}