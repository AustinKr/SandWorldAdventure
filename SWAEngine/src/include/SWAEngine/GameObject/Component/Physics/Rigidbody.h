#pragma once
#include <SWAEngine/dllClause.h>
#include <SWAEngine/Math/Transform.h>
#include <SWAEngine/Math/Time.h>

#include <SWAEngine/GameObject/Component/IComponent.h>
#include "Collider.h"

namespace SWAEngine::GameObject::Component::Physics
{
	// TODO: allow collision tags to be configured with physics objects
	struct SWA_ENGINE_API Rigidbody : IComponent
	{
	private:
		Math::Vector2 m_LastVelocity;
		Math::Vector2 m_Velocity;
		Math::Vector2 m_Acceleration;

		Math::Time m_Time;

		double m_Dampening;

		bool m_IsTouchingGround;

	protected:
		// Step move algorithm (only accurate for small movements)
		bool StepMove(SWAEngine::Math::Vector2 movement);
		void TryApplyVelocity();

		Rigidbody();
	public:
		static const int MAX_COLLISION_STEPS;
		
		Collider* p_Collider; /// TODO: Make collider a component and have physics object check for it
		Math::Transform Coordinates;
		
		virtual std::string const GetName() override;
		virtual void Update(Math::Time time) override;
		virtual void Release() override;

		// Returns the currently applied velocity
		Math::Vector2 GetVelocity();
		// Returns the actual change in velocity over change in time
		Math::Vector2 GetAcceleration();

		// Moves by an impluse
		// Note that this is only applied at the end of every Player::Update()
		void AddVelocity(Math::Vector2 vel);
		// Continually accelerates by the given amount
		void Accelerate(Math::Vector2 acc);

		void Jump(double height, double gravity);
		bool IsTouchingGround();
	};
}