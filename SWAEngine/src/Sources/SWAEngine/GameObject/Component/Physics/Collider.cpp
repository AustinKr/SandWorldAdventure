#include <SWAEngine/GameObject/Component/Physics/Collider.h>
#include <SWAEngine/GameObject/GameObject.h>
#include <SWAEngine/SceneManager.h>

namespace SWAEngine::GameObject::Component::Physics
{
	std::set<Collider*> Collider::ms_CollidersRegistry = {};

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

	bool Collider::IsColliding(int tag)
	{
		// Compare all objects other than this with this
		for (auto& iter : ms_CollidersRegistry)
		{
			if (iter == this)
				continue;
			if (tag != SELECT_ALL && !(iter->GetType() & tag))
				continue;

			if (iter->IsColliding(this))
				return true;
		}
		return false;
	}

	
}