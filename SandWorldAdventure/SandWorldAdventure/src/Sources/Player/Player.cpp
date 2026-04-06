#include "SWA/Player/Player.h"
#include "SWA/Player/Inventory/PlayerInventoryGUI.h"

#include "SWA/Game.h"

#include "SWA/RenderLayerNames.h"
#include "GP2D/Pipeline/GenericPipeline.h"



using namespace GP2D::Pipeline;
using namespace GP2D::Math;
using namespace SWAEngine::Math;
using namespace SWAEngine::Tilemap;
using namespace SWA::Player::Inventory;

namespace SWA::Player
{
	const int Player::MAX_COLLISION_STEPS = 5;

	bool Player::StepMove(Vector2 movement)
	{
		Vector2 origin = GetPosition();
		double factor = 1.0;
		for (int step = 0; step < MAX_COLLISION_STEPS; step++)
		{
			// Set to current position to try
			SetPosition(origin + movement * factor);

			if (IsColliding())
				factor /= 2; // Move back
			else if (factor < 1.0)
				factor *= 1.5; // Move forward
			else
				return false; // Reached end without colliding
		}

		// Move back to start if collision wasn't resolved
		if (IsColliding())
			SetPosition(origin);

		return true;
	}
	void Player::TryApplyVelocity()
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

	

	Player::Player() :
		BaseGameObject("Player"),
		m_Inputs{},
		Gravity(0.9), m_LastVelocity{}, m_Velocity{}, m_Dampening(.98), Speed(1.0), JumpHeight(0.6),
		m_IsTouchingGround{}, m_Time{}, CameraFollowSpeed{5}
	{
		// Create the mesh
		mp_Mesh = new Mesh::Mesh();
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Characters).RegisterMesh(mp_Mesh);
		mp_Mesh->Scale = { 0.05f, 0.1f};
		mp_Mesh->Vertices =
		{
			{{ 0, 0 }, {0, 0, 0}},
			{{ 1, 0 }, {1, 0, 0}},
			{{ 1, 1 }, {1, 1, 0}},
			{{ 0, 1 }, {0, 1, 0}},
		};
		mp_Mesh->Triangles =
		{
			0, 1, 2,
			0, 2, 3
		};
		mp_Mesh->Shaders =
		{
			{0, 6, "DefaultSpriteShader"}
		};

		// Create player inventory and gui
		Inventory = {};
		PlayerInventoryGUI::Initialize(Inventory);
	}

	void Player::Move()
	{
		m_Inputs.SetInputs();
		// Try jump
		if (m_Inputs.TryJump && IsTouchingGround())
			Jump(JumpHeight);
		// Move
		AddVelocity(Vector2::Normalize(m_Inputs.Movement) * Speed * m_Time.RealDeltaTime); // Accerate but as an impulse
		// Add gravity
		AddVelocity({ 0, -Gravity * m_Time.RealDeltaTime }); // Accerate but as an impulse
	}

	void Player::Update(SWAEngine::Time time)
	{
		m_Time = time;

		// Use selected item
		Inventory.TryUseSelectedItem(m_Inputs, time);
		
		Move();

		// Apply physics
		m_Velocity += m_Acceleration * m_Time.RealDeltaTime;
		m_Velocity *= m_Dampening;

		m_LastVelocity = m_Velocity;

		TryApplyVelocity();

		// Move camera
		GenericPipeline::s_ActiveCamera.Origin += (mp_Mesh->Origin - GenericPipeline::s_ActiveCamera.Origin) * float(m_Time.RealDeltaTime * CameraFollowSpeed);
	}
	void Player::Release()
	{
		Inventory.Release(); // Clears/releases everything associated with the player inventory
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Characters).UnregisterMesh(mp_Mesh); // Unregister the mesh

		BaseGameObject::Release();
	}

	Vector2 Player::GetPosition()
	{
		return Vector2(mp_Mesh->Origin.X, mp_Mesh->Origin.Y);
	}
	void Player::SetPosition(Vector2 newPosition)
	{
		mp_Mesh->Origin = { (float)newPosition.X, (float)newPosition.Y };
	}
	Vector2 Player::GetScale()
	{
		return Vector2(mp_Mesh->Scale.X, mp_Mesh->Scale.Y);
	}
	void Player::SetScale(Vector2 newScale)
	{
		mp_Mesh->Scale = { (float)newScale.X, (float)newScale.Y };
	}


	SWAEngine::Math::Vector2 Player::GetVelocity()
	{
		return m_Velocity;
	}
	void Player::AddVelocity(SWAEngine::Math::Vector2 vel)
	{
		m_Velocity += vel;
	}
	void Player::Accelerate(SWAEngine::Math::Vector2 acc)
	{
		m_Acceleration += acc;
	}
	SWAEngine::Math::Vector2 Player::GetAcceleration()
	{
		return (m_Velocity - m_LastVelocity) / m_Time.RealDeltaTime;
	}

	void Player::Jump(double height)
	{
		AddVelocity({ 0,sqrt(2.0 * Gravity * height) });
		m_IsTouchingGround = false;
	}
	bool Player::IsTouchingGround()
	{
		return m_IsTouchingGround;
	}

	bool Player::IsColliding()
	{
		return
			GetPosition().X < Game::p_Tilemap->Origin.X || GetPosition().Y < Game::p_Tilemap->Origin.Y ||
			Game::p_Tilemap->DetectCollisionRect(Game::p_Tilemap->WorldToTile(GetPosition()), Game::p_Tilemap->WorldToTile(GetPosition() + GetScale()));
	}
}