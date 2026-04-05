#pragma once
#include "SWAEngine/Math/vector2.h"

namespace SWA::Player
{
	struct Inputs
	{
		bool TryJump;

		bool BreakTile;
		bool AddTile;

		SWAEngine::Math::Vector2 Movement;

		void SetInputs();
	};
}