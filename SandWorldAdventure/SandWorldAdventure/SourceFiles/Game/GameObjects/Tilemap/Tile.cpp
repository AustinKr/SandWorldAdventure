#include "HeaderFiles/Game/GameObjects/Tilemap/Tile.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{

	const Tile Tile::EMPTY = Tile();

	Tile::Tile(UINT color, int behavior)
	{
		Color = color;
		BehaviorIndex = behavior;
		HasValue = true;
		Motion = Vector2(0, 0);
		LastMoveTime = 0;
	}
	Tile::Tile()
	{
		Color = 0x0;
		BehaviorIndex = 0;
		HasValue = false;
		Motion = Vector2(0, 0);
		LastMoveTime = 0;
	}
}