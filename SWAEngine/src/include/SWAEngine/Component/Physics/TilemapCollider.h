#pragma once
#include "Collider.h"
#include <SWAEngine/Component/TilemapComponent.h>

namespace SWAEngine::Component::Physics
{
	struct SWA_ENGINE_API TilemapCollider : Collider
	{
	public:
		TilemapComponent* p_LinkedTilemap;

		// Inherited/Required by IComponent

		static std::string const GetName();
		virtual void Initialize(std::string objName) override; // Calls Collider::Initialize() and requires the Tilemap Component
		virtual void Release() override;
		virtual bool IsColliding(Collider* other) override;

		// Inherited by Collider

		virtual ColliderTypes GetType() override;
	};
}