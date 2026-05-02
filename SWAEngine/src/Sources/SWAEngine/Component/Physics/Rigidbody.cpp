#include <SWAEngine/Component/Physics/Rigidbody.h>
#include <SWAEngine/Component/Physics/Collider.h>
#include <SWAEngine/SceneManager.h>
#include <SWAEngine/Component/ComponentTags.h>

using namespace SWAEngine::Math;

namespace SWAEngine::Component::Physics
{
	const int Rigidbody::MAX_COLLISION_STEPS = 5;
	Rigidbody::Rigidbody() :
		m_LastVelocity{}, m_Velocity{}, m_Acceleration{}, m_Dampening(.98),
		m_IsTouchingGround{}, m_Time{}, p_LinkedTransform(nullptr)
	{}
	unsigned int const Rigidbody::GetTag()
	{
		return COMP_TAG_RIGIDBODY;
	}
	void Rigidbody::Initialize(std::string objName)
	{
		GameObject& linkedObject = SWAEngine::SceneManager::GetScene().GetGameObject(objName);

		p_LinkedTransform = linkedObject.GetComponent<Transform>();
	}
	void Rigidbody::Release()
	{
		delete(this);
	}
	void Rigidbody::Update(std::string objectName, Math::Time time)
	{
		// Update some variables
		m_ObjectName = objectName;
		m_Time = time;

		// Apply physics
		m_Velocity += m_Acceleration * m_Time.RealDeltaTime;
		m_Velocity *= m_Dampening;

		m_LastVelocity = m_Velocity;

		TryApplyVelocity();
	}
	void Rigidbody::SetActive(bool state) {}
	bool Rigidbody::GetActive() { return true; }
	
	bool Rigidbody::StepMove(SWAEngine::Math::Vector2 movement)
	{
		// Get collider (assumed to have a value)
		Collider* pCollider = SceneManager::GetScene().GetGameObject(m_ObjectName).TryFindComponent<Collider>();

		Vector2 origin = p_LinkedTransform->GetPosition();
		double factor = 1.0;
		
		// Iterate
		for (int step = 0; step < MAX_COLLISION_STEPS; step++)
		{
			// Set to current position to try
			p_LinkedTransform->SetPosition(origin + movement * factor);

			if (pCollider->IsColliding())
				factor /= 2; // Move back
			else if (factor < 1.0)
				factor *= 1.5; // Move forward
			else
				return false; // Reached end without colliding
		}

		// Move back to start if collision wasn't resolved
		if (pCollider->IsColliding())
			p_LinkedTransform->SetPosition(origin);

		return true;
	}
	void Rigidbody::TryApplyVelocity()
	{
		//if (IsColliding())
		//	return; // Fail; TODO: Handle when already colliding

		Vector2 movement = m_Velocity * m_Time.RealDeltaTime;

		// Try to just move if we couldn't possibly collide
		if (!SceneManager::GetScene().GetGameObject(m_ObjectName).ContainsComponent<Collider>())
		{
			p_LinkedTransform->Translate(movement);
			return;
		}

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