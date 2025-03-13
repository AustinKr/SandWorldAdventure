#pragma once
#include "HeaderFiles/Game/GameObjects/Tilemap/TileActionQueue/TileActionType.h"

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	/// <summary>
	/// A structure that contains the base member variables of BaseTileAction to be passed as arguments
	/// </summary>
	struct BaseTileActionArgument // TODO: Could add a member named IsInitialized to make sure no corrupt memory is attemtped to be deleted
	{
	public:
		// This is only used as reference and is not an actual arguement
		TileActionType ActionType;

		// Should updates to adjacent tiles be included?
		bool ShouldUpdateAdjacent;

		inline BaseTileActionArgument(bool shouldUpdateAdjacent = true)
		{
			ActionType = (TileActionType)-1;
			ShouldUpdateAdjacent = shouldUpdateAdjacent;
		}
		// This is only used to get the base members of an action
		inline BaseTileActionArgument(TileActionType type, bool shouldUpdateAdjacent)
		{
			ActionType = type;
			ShouldUpdateAdjacent = shouldUpdateAdjacent;
		}
	};
}