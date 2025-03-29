#pragma once
#include "HeaderFiles/Math.h"
#include <Windows.h>

namespace SandboxEngine::Game::GameObject::Tilemap
{
	struct Tile
	{
	public:
		UINT Color;
		int BehaviorIndex;

		bool HasValue;

		Vector2 Motion; // The velocity

		double LastMoveTime; // The time since the tile physically got replaced

		inline Tile(UINT color, int behavior)
		{
			Color = color;
			BehaviorIndex = behavior;
			HasValue = true;
			Motion = Vector2(0, 0);
			LastMoveTime = 0;
		}
		inline Tile()
		{
			Color = 0x0;
			BehaviorIndex = 0;
			HasValue = false;
			Motion = Vector2(0, 0);
			LastMoveTime = 0;
		}
	};
}