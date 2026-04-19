#include <SWAEngine/Physics/ICollider.h>

namespace SWAEngine::Physics
{
	std::set<ICollider*> ICollider::ms_CollidersRegistry = {};

	void ICollider::Release() 
	{
		ms_CollidersRegistry.erase(this);
		delete(this);
	}
}