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

		virtual unsigned int const GetTag() override;
		virtual void Initialize(std::string objName) override; // Calls Collider::Initialize() and requires the Tilemap Component
		virtual void Release() override;

		// Inherited by Collider
		virtual bool IsColliding(Collider* other) override;
	};
}