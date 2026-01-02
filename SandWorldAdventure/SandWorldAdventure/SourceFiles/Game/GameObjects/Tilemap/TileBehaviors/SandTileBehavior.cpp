#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SandTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{

	void SandTileBehavior::TryApplyMotion(
		Tilemap* pTilemap,
		Vector2Int tilePosition,
		TilemapContainer::TILE_INFO tileInfo,
		Time time, 
		TileActionTypes action)
	{
		double tileDeltaTime = time.CurrentTime - tileInfo.second->LastMoveTime;
								//+ (action & TileBehavior::TILE_ACTION_ADD ? 1 : 0); // +1 is so tiles immediately start falling
		Vector2 translation = tileInfo.second->Motion * tileDeltaTime;
		if (abs(translation.X) < 1 && abs(translation.Y) < 1) // Is there enough momentum to move?
		{
			return;
			pTilemap->RefreshTile(tilePosition);
		}

		// Try move
		bool succeeded = true;
		Vector2Int hitPosition = pTilemap->TryStepMoveTile(tilePosition, translation, &succeeded);
		if (succeeded)
			return;

		// Try alternate pathway (by rotating motion line)
		int x = int(((double)rand() / (double)RAND_MAX) * 2) - 1;
		double angle = 3.14 / 3.0;
		Vector2 motionRotated = { tileInfo.second->Motion.X * cos(angle * x) - tileInfo.second->Motion.Y * sin(angle * x),
								  tileInfo.second->Motion.X * sin(angle * x) + tileInfo.second->Motion.Y * cos(angle * x) };
		pTilemap->TryStepMoveTile(tilePosition, motionRotated, &succeeded);
		if (succeeded)
			return;

		// Try opposite direction
		motionRotated = { tileInfo.second->Motion.X * cos(angle * -x) - tileInfo.second->Motion.Y * sin(angle * -x),
						  tileInfo.second->Motion.X * sin(angle * -x) + tileInfo.second->Motion.Y * cos(angle * -x) };
		pTilemap->TryStepMoveTile(tilePosition, motionRotated, &succeeded);
		if (succeeded)
			return;

		// Failed to move because something was in the way and absorbed the momentum
		tileInfo.second->Motion = {};


		//// Try to apply motion
		//if (abs(translation.X) >= 1 || abs(translation.Y) >= 1)
		//{
		//	// We have enough energy to move!
		//	bool succeeded = true;
		//	Vector2Int hitPosition = pTilemap->TryStepMoveTile(tilePosition, translation, &succeeded);
		//	if (succeeded)
		//	{
		//		// We moved! (this tile is now treated as removed)
		//		 //tileInfo.second->Motion = tileInfo.second->Motion - (hitPosition - tilePosition); // We converted potential energy into kinetic, so subtract how much we were able to use
		//		//tileInfo.second->LastMoveTime = time.CurrentTime; // Update move time
		//		return;
		//	}
		//	// Something was blocking movement

		//	tileInfo.second->Motion = {};
		//	return;
		//}
		//// The motion for this update was too small to register.
		//// We thus have to refresh this tile next cycle when delta time is greater, otherwise it will not be updated.
		//pTilemap->RefreshTile(tilePosition);
	}

	bool SandTileBehavior::TryUpdate(
		Tilemap* pTilemap,
		Vector2Int tilePosition,
		TilemapContainer::TILE_INFO tileInfo,
		Time time,
		TileActionTypes action)
	{
		if (tileInfo.second != nullptr && tileInfo.second->HasValue)
		{
			// Time
			double deltaTime = time.CurrentTime - tileInfo.second->LastMoveTime;

			// Gravity with a little deviation
			//pTile->Motion.X -= 0.5 + ((double)rand() / RAND_MAX) * 0.0;
			tileInfo.second->Motion.Y -= (double)GRAVITY / 10.0 * deltaTime;// +((double)rand() / RAND_MAX) * 0.0;

			TryApplyMotion(pTilemap, tilePosition, tileInfo, time, action);
		}



		//if (tileInfo.second != nullptr && tileInfo.second->HasValue)
		//{
		//	if (!pTilemap->WillContainTile(tilePosition - Vector2Int(0, 1)))
		//		pTilemap->SwapTiles(tilePosition, tilePosition - Vector2Int(0, 1)); // Note: nothing has actually changed yet at this point
		//	else if (floor((double)rand() / RAND_MAX) * 3 == 0) // 1 in 3 chance
		//	{
		//		if (!pTilemap->WillContainTile(tilePosition - Vector2Int(1, 0)) && !pTilemap->WillContainTile(tilePosition - Vector2Int(1, 1)))
		//			pTilemap->SwapTiles(tilePosition, tilePosition - Vector2Int(1, 0));
		//		else if (!pTilemap->WillContainTile(tilePosition + Vector2Int(1, 0)) && !pTilemap->WillContainTile(tilePosition + Vector2Int(1, -1)))
		//			pTilemap->SwapTiles(tilePosition, tilePosition + Vector2Int(1, 0));
		//	}
		//}


		return TileBehavior::TryUpdate(pTilemap, tilePosition, tileInfo, time, action);
	}
}