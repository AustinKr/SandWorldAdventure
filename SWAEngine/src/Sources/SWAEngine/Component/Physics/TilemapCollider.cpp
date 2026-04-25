#include <SWAEngine/Component/Physics/TilemapCollider.h>
#include <SWAEngine/Component/Physics/BoxCollider.h>
#include <SWAEngine/SceneManager.h>
#include <SWAEngine/GameObject.h>

namespace SWAEngine::Component::Physics
{
	std::string const TilemapCollider::GetName()
	{
		return "tilemap_collider";
	}
	void TilemapCollider::Initialize(std::string objName)
	{
		GameObject& linkedObject = SWAEngine::SceneManager::GetScene().GetGameObject(objName);
		p_LinkedTilemap = linkedObject.GetComponent<Tilemap::TilemapComponent>();

		Collider::Initialize(objName);
	}
	void TilemapCollider::Release()
	{
		delete(this);
	}

	bool TilemapCollider::IsColliding(Collider* other)
	{
		switch (other->GetType())
		{
		case COLLIDER_TYPE_BOX:
		{
			BoxCollider* pBox = static_cast<BoxCollider*>(other);
			Math::Vector2Int bottomLeft = p_LinkedTilemap->WorldToTile(pBox->p_LinkedTransform->GetPosition());
			Math::Vector2Int topRight = bottomLeft + p_LinkedTilemap->WorldToTile(pBox->p_LinkedTransform->GetScale(), false);

			return p_LinkedTilemap->DetectCollisionRect(bottomLeft, topRight);
		}
		default:
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

		return false;
	}

	ColliderTypes TilemapCollider::GetType()
	{
		return COLLIDER_TYPE_TILEMAP;
	}
}