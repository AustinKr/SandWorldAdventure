#pragma once
#include "BaseTileActionArgument.h"
#include "HeaderFiles/Vector2Int.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	/// <summary>
	/// A structure that contains the member variables of SwapTileAction to be passed as arguments
	/// </summary>
	struct SwapTileActionArgument : public BaseTileActionArgument
	{
	public:
		Vector2Int TilePositionA;
		Vector2Int TilePositionB;
		bool ForceSwap;

		inline SwapTileActionArgument() : BaseTileActionArgument()
		{
			TilePositionA = {};
			TilePositionB = {};
			ForceSwap = false;
		}
		inline SwapTileActionArgument(bool shouldUpdateAdjacent, Vector2Int tilePositionA, Vector2Int tilePositionB, bool forceSwap) : BaseTileActionArgument(shouldUpdateAdjacent)
		{
			TilePositionA = tilePositionA;
			TilePositionB = tilePositionB;
			ForceSwap = forceSwap;
		}
	};
}