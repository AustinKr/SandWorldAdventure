#pragma once

namespace SandboxEngine::Game::GameObject::Tilemap::TileActionQueue
{
	enum TileActionType
	{
		Update = 0, // This is not an action, only used as reference to represent updates
		ActionPointer = 1, // Does not function like an actual action
		Redraw = 2,
		Swap = 4,
		Remove = 8,
		Add = 16,
		AddBlock = 32, // TODO: Implement this
		RemoveBlock = 64, // TODO: Implement this

	};
}
