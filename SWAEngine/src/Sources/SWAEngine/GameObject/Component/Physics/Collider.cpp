#include <SWAEngine/GameObject/Component/Physics/Collider.h>
#include <SWAEngine/GameObject/GameObject.h>
#include <SWAEngine/SceneManager.h>

namespace SWAEngine::GameObject::Component::Physics
{
	std::set<Collider*> Collider::ms_CollidersRegistry = {};

	Collider::Collider(std::string objName)
	{
		GameObject& linkedObject = SWAEngine::SceneManager::GetScene().GetGameObject(objName);
		p_LinkedTransform = linkedObject.GetComponent<Transform>("transform");

		ms_CollidersRegistry.insert(this);
	}

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
}