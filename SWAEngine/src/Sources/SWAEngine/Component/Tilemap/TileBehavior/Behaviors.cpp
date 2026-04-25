#include <SWAEngine/Component/Tilemap/TileBehavior/IBehavior.h>
#include <SWAEngine/Component/Tilemap/TileBehavior/Types.h>

#include <SWAEngine/Component/Tilemap/TileBehavior/Sand.h>
#include <SWAEngine/Component/Tilemap/TileBehavior/Fluid.h>

namespace SWAEngine::Component::Tilemap::TileBehavior
{
	// TODO: Behaviors could be defined in a .json file and loaded into this map
	std::unordered_map<unsigned int, IBehavior*> IBehavior::s_Behaviors =
	{
		{ SAND, new Sand() },
		{ FLUID, new Fluid() },
	};
}