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
		//// Time
		//double deltaTime = time.CurrentTime - tileInfo.second->LastMoveTime;
		//tileInfo.second->LastMoveTime = time.CurrentTime;

		//// Gravity with a little deviation
		////pTile->Motion.X -= 0.5 + ((double)rand() / RAND_MAX) * 0.0;
		//tileInfo.second->Motion.Y -= 0.5;// +((double)rand() / RAND_MAX) * 0.0;

		//Vector2 translation = tileInfo.second->Motion * deltaTime;
		//double end = sqrt(translation.GetMagnitudeSqrd());
		//Vector2 direction = translation / end;
		//double distance = TileBehavior::TryStepMoveTile(pTilemap, tilePosition, direction, end);
		//if (distance >= 1)
		//	return true;

		//return tileInfo.second->HasValue;// TODO: !!Sand tiles disabled!! 

		if (tileInfo.second != nullptr && tileInfo.second->HasValue)
		{
			if (!pTilemap->WillContainTile(tilePosition - Vector2Int(0, 1)))
				pTilemap->AddTile(tilePosition - Vector2Int(0,1), *tileInfo.second, false);// pTilemap->SwapTiles(tilePosition, tilePosition - Vector2Int(0, 1)); // Note: nothing has actually changed yet at this point
		/*	else if(!pTilemap->WillContainTile(tilePosition - Vector2Int(1, 0)))
				pTilemap->SwapTiles(tilePosition, tilePosition - Vector2Int(1, 0));
			else if (!pTilemap->WillContainTile(tilePosition + Vector2Int(1, 0)))
				pTilemap->SwapTiles(tilePosition, tilePosition + Vector2Int(1, 0));*/
		}
		return TileBehavior::TryUpdate(pTilemap, tilePosition, tileInfo, time, action);
	}
}