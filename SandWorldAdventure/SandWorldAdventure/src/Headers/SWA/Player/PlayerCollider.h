#pragma once
#include <SWAEngine/Component/Physics/BoxCollider.h>

namespace SWA::Player
{
	// TODO: make PlayerCollider somehow inhert BoxCollider
	struct PlayerCollider : SWAEngine::Component::Physics::BoxCollider
	{
		static const unsigned int PLAYER_COLLIDER_TAG = 2048; // TODO: This will conflict if there are 11 Components in SWAEngine
		static const unsigned int PLAYER_WORLD_BOUNDS = 1; // One world unit away from the origin of the Tilemap

		virtual unsigned int const GetTag() override;

		// Checks collision with the tilemap bounds, and all registered colliders
		virtual bool IsColliding(unsigned int tag = SELECT_ALL) override;
	};
}