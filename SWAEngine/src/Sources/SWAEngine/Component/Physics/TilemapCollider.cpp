#include <SWAEngine/Component/Physics/TilemapCollider.h>
#include <SWAEngine/Component/Physics/BoxCollider.h>
#include <SWAEngine/SceneManager.h>
#include <SWAEngine/GameObject.h>
#include <SWAEngine/Component/ComponentTags.h>

namespace SWAEngine::Component::Physics
{
	unsigned int const TilemapCollider::GetTag()
	{
		return COMP_TAG_COLLIDER | COMP_TAG_TILEMAP_COLLIDER;
	}
	void TilemapCollider::Initialize(std::string objName)
	{
		GameObject& linkedObject = SWAEngine::SceneManager::GetScene().GetGameObject(objName);
		p_LinkedTilemap = linkedObject.GetComponent<TilemapComponent>();

		Collider::Initialize(objName);
	}
	void TilemapCollider::Release()
	{
		delete(this);
	}

	bool TilemapCollider::IsColliding(Collider* other)
	{
		unsigned int tag = other->GetTag();
		if (tag & COMP_TAG_BOX_COLLIDER)
		{
			BoxCollider* pBox = static_cast<BoxCollider*>(other);
			Math::Vector2Int bottomLeft = p_LinkedTilemap->WorldToTile(pBox->p_LinkedTransform->GetPosition());
			Math::Vector2Int topRight = bottomLeft + p_LinkedTilemap->WorldToTile(pBox->p_LinkedTransform->GetScale(), false);

			return p_LinkedTilemap->DetectCollisionRect(bottomLeft, topRight);
		}

		// Handle default
		bool isOtherColliding = other->IsColliding(this);

		// TODO: handle other. Could use test points to be semi-accurate with unknown colliders
		//if (!isOtherColliding)
		//{
		//	// try handle other
		//	//...
		//	return approximate;
		//}

		return isOtherColliding;
	}
	bool TilemapCollider::IsColliding(Math::Vector2 point)
	{
		return p_LinkedTilemap->GetActiveTile(p_LinkedTilemap->WorldToTile(point)).HasValue;
	}
}