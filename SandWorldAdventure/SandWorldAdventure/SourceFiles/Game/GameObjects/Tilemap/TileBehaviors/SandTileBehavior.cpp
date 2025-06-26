#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SandTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
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
			tileInfo.second->LastMoveTime = time.CurrentTime;

			// Gravity with a little deviation
			//pTile->Motion.X -= 0.5 + ((double)rand() / RAND_MAX) * 0.0;
			tileInfo.second->Motion.Y -= GRAVITY/10.0 * deltaTime;// +((double)rand() / RAND_MAX) * 0.0;

			Vector2 translation = tileInfo.second->Motion * deltaTime;

			double end = sqrt(translation.GetMagnitudeSqrd());
			Vector2 direction = translation / end;
			double distance = pTilemap->TryStepMoveTile(tilePosition, direction, end);
			
			tileInfo.second->Motion = tileInfo.second->Motion * ((end-distance) / (double)end); // We are converting this potential energy into kinetic, so set it to 0
			if (distance >= 1)
				return true;
			// Try move failed

			if (end <= 1)
			{
				// Move failed because for whatever reason, the motion for this update was too small to register.
				// We thus have to refresh this tile next cycle when delta time is greater, otherwise it will not be updated 
				//  again whether there is space to move.
				pTilemap->RefreshTile(tilePosition);
				return true;
			}
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