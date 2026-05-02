#include <SWA/Player/PlayerCollider.h>
#include <SWAEngine/Component/Physics/BoxCollider.h>
#include <SWAEngine/Component/Physics/TilemapCollider.h>
#include <SWA/Game.h>

using namespace SWAEngine::Component;
using namespace Physics;

namespace SWA::Player
{
	unsigned int const PlayerCollider::GetTag()
	{
		return BoxCollider::GetTag() | PLAYER_COLLIDER_TAG;
	}
	bool PlayerCollider::IsColliding(unsigned int tag)
	{
		if (p_LinkedTransform->GetPosition().X < Game::p_Tilemap->p_LinkedTransform->GetPosition().X + PLAYER_WORLD_BOUNDS
			|| p_LinkedTransform->GetPosition().Y < Game::p_Tilemap->p_LinkedTransform->GetPosition().Y)
			return true;
		
		return Collider::IsColliding(tag);
	}
}