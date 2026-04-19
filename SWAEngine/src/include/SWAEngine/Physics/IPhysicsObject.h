#pragma once
#include <SWAEngine/dllClause.h>
#include <SWAEngine/Math/Transform.h>
#include <SWAEngine/Math/Time.h>

namespace SWAEngine::Physics
{
	struct SWA_ENGINE_API IPhysicsObject
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

		IPhysicsObject();
	public:
		static const int MAX_COLLISION_STEPS;
		
		Math::Transform Coordinates;

		void UpdatePhysics(Math::Time time);

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

		// TODO: can replace with a collider object
		virtual bool IsColliding() = 0;
	};
}