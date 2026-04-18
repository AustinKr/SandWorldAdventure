#pragma once

#include "dllClause.h"
#include "Time.h"
#include "Math/vector2.h"

namespace SWAEngine
{
	struct SWA_ENGINE_API ICollider
	{
		//TODO:registry of colliders

		bool IsColliding();
	};
}
