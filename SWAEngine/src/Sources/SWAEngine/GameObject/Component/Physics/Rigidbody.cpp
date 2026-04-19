#include <SWAEngine/GameObject/Component/Physics/Rigidbody.h>

using namespace SWAEngine::Math;

namespace SWAEngine::GameObject::Component::Physics
{
	const int Rigidbody::MAX_COLLISION_STEPS = 5;
	Rigidbody::Rigidbody() : 
		p_Collider(nullptr), m_LastVelocity{}, m_Velocity{}, m_Acceleration{}, m_Dampening(.98),
		m_IsTouchingGround{}, m_Time{}, Coordinates{}
	{
	}

	std::string const Rigidbody::GetName()
	{
		return "physics";
	}
	void Rigidbody::Update(Math::Time time)
	{
		m_Time = time;

		// Apply physics
		m_Velocity += m_Acceleration * m_Time.RealDeltaTime;
		m_Velocity *= m_Dampening;

		m_LastVelocity = m_Velocity;

		TryApplyVelocity();
	}
	void Rigidbody::Release()
	{
		delete(this);
	}
	

	bool Rigidbody::StepMove(SWAEngine::Math::Vector2 movement)
	{
		Vector2 origin = Coordinates.GetPosition();
		double factor = 1.0;
		for (int step = 0; step < MAX_COLLISION_STEPS; step++)
		{
			// Set to current position to try
			Coordinates.SetPosition(origin + movement * factor);

			if (p_Collider->IsColliding())
				factor /= 2; // Move back
			else if (factor < 1.0)
				factor *= 1.5; // Move forward
			else
				return false; // Reached end without colliding
		}

		// Move back to start if collision wasn't resolved
		if (p_Collider->IsColliding())
			Coordinates.SetPosition(origin);

		return true;
	}
	void Rigidbody::TryApplyVelocity()
	{
		//if (IsColliding())
		//	return; // Fail; TODO: Handle when already colliding

		Vector2 movement = m_Velocity * m_Time.RealDeltaTime;

		// Step in each direction
		if (StepMove({ movement.X, 0 }))
		{
			m_Velocity.X *= .01;
			m_Acceleration.X = 0;
		}
		if (StepMove({ 0, movement.Y }))
		{
			m_Velocity.Y *= .01;
			m_Acceleration.Y = 0;

			if (movement.Y < 0)
				m_IsTouchingGround = true;
		}
		else
			m_IsTouchingGround = false;
	}

	// Returns the currently applied velocity
	Math::Vector2 Rigidbody::GetVelocity()
	{
		return m_Velocity;
	}
	// Returns the actual change in velocity over change in time
	Math::Vector2 Rigidbody::GetAcceleration()
	{
		return (m_Velocity - m_LastVelocity) / m_Time.RealDeltaTime;
	}

	// Moves by an impluse
	// Note that this is only applied at the end of every Player::Update()
	void Rigidbody::AddVelocity(Math::Vector2 vel)
	{
		m_Velocity += vel;
	}
	// Continually accelerates by the given amount
	void Rigidbody::Accelerate(Math::Vector2 acc)
	{
		m_Acceleration += acc;
	}

	void Rigidbody::Jump(double height, double gravity)
	{
		AddVelocity({ 0,sqrt(2.0 * gravity * height) });
		m_IsTouchingGround = false;
	}
	bool Rigidbody::IsTouchingGround()
	{
		return m_IsTouchingGround;
	}
}