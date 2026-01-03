#pragma once
#include "HeaderFiles/Math.h"
#include <Windows.h>

namespace SandboxEngine::Game::GameObject::Tilemap
{
	struct Tile
	{
	public:
		static const Tile EMPTY;

		UINT Color;
		int BehaviorIndex;

		bool HasValue;

		Vector2 Motion; // The velocity

		double LastMoveTime; // The time since the tile physically got replaced

		// Creates a tile
		Tile(UINT color, int behavior);
		// Creates an empty tile
		Tile();
	};
}