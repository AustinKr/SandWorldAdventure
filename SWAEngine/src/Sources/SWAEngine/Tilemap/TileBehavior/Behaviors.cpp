#include "SWAEngine/Tilemap/TileBehavior/IBehavior.h"
#include "SWAEngine/Tilemap/TileBehavior/Types.h"

#include "SWAEngine/Tilemap/TileBehavior/Sand.h"
#include "SWAEngine/Tilemap/TileBehavior/Fluid.h"

namespace SWAEngine::Tilemap::TileBehavior
{
	// TODO: Behaviors could be defined in a .json file and loaded into this map
	std::unordered_map<unsigned int, IBehavior*> IBehavior::s_Behaviors =
	{
		{ SAND, new Sand() },
		{ FLUID, new Fluid() },
	};
}