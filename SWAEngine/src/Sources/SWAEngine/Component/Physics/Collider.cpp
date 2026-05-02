#include <SWAEngine/Component/Physics/Collider.h>
#include <SWAEngine/GameObject.h>
#include <SWAEngine/SceneManager.h>
#include <SWAEngine/Component/ComponentTags.h>

namespace SWAEngine::Component::Physics
{
	std::set<Collider*> Collider::ms_CollidersRegistry = {};
	
	unsigned int const Collider::GetTag()
	{
		return COMP_TAG_COLLIDER;
	}
	void Collider::Initialize(std::string objName)
	{
		GameObject& linkedObject = SWAEngine::SceneManager::GetScene().GetGameObject(objName);
		p_LinkedTransform = linkedObject.GetComponent<Transform>();

		ms_CollidersRegistry.insert(this);
	}
	void Collider::Release()
	{
		ms_CollidersRegistry.erase(this);
		delete(this);
	}
	
	bool Collider::IsColliding(Collider* other)
	{ return false; }
	bool Collider::IsColliding(unsigned int tag)
	{
		// Compare all objects other than this with this
		for (auto& iter : ms_CollidersRegistry)
		{
			if (iter == this)
				continue;
			if (tag != SELECT_ALL && !(iter->GetTag() & tag))
				continue;

			if (iter->IsColliding(this))
				return true;
		}
		return false;
	}
}