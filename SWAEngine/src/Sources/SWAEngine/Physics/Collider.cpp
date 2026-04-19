#include <SWAEngine/Physics/Collider.h>

namespace SWAEngine::Physics
{
	std::set<Collider*> Collider::ms_CollidersRegistry = {};

	bool Collider::IsColliding(int tag)
	{
		// Compare all objects other than this with this
		for (auto& iter : ms_CollidersRegistry)
		{
			if (iter == this)
				continue;

			if (iter->IsColliding(this, tag))
				return true;
		}
		return false;
	}

	void Collider::Release() 
	{
		ms_CollidersRegistry.erase(this);
		delete(this);
	}
	void Collider::ReleaseAll()
	{
		for (auto& iter : ms_CollidersRegistry)
		{
			delete(iter);
		}
		ms_CollidersRegistry.clear();
	}
}