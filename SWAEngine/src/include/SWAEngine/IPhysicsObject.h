#pragma once
#include "dllClause.h"
#include "Time.h"
#include "Math/vector2.h"

namespace SWAEngine
{
	struct SWA_ENGINE_API IPhysicsObject
	{
	private:
		SWAEngine::Math::Vector2 m_LastVelocity;
		SWAEngine::Math::Vector2 m_Velocity;
		SWAEngine::Math::Vector2 m_Acceleration;

		SWAEngine::Time m_Time;

		double m_Dampening;

		bool m_IsTouchingGround;

	protected:
		// Step move algorithm (only accurate for small movements)
		bool StepMove(SWAEngine::Math::Vector2 movement);
		void TryApplyVelocity();

	public:
		static const int MAX_COLLISION_STEPS;

		void UpdatePhysics(Time time);

		// Returns the currently applied velocity
		SWAEngine::Math::Vector2 GetVelocity();
		// Returns the actual change in velocity over change in time
		SWAEngine::Math::Vector2 GetAcceleration();

		// Moves by an impluse
		// Note that this is only applied at the end of every Player::Update()
		void AddVelocity(SWAEngine::Math::Vector2 vel);
		// Continually accelerates by the given amount
		void Accelerate(SWAEngine::Math::Vector2 acc);

		void Jump(double height, double gravity);
		bool IsTouchingGround();

		// TODO: can replace with a collider object
		virtual bool IsColliding() = 0;

		// TODO Can replace with a transform object
		virtual Math::Vector2 GetPosition() = 0;
		virtual void SetPosition(Math::Vector2 newPosition) = 0;
		virtual Math::Vector2 GetScale() = 0;
		virtual void SetScale(Math::Vector2 newScale) = 0;
	};
}