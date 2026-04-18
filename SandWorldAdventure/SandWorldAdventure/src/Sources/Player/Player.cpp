#include "SWA/Player/Player.h"
#include "SWA/Player/Inventory/GUILayout.h"
#include "SWA/SpriteShaderProperties.h"

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
	Player::Player() :
		BaseGameObject("Player"), IPhysicsObject(),
		m_Inputs{},
		Gravity(0.9), m_Time{}, CameraFollowSpeed{5}, Speed(1), JumpHeight(1)
	{
		// Create the mesh
		mp_Mesh = new Mesh::Mesh(true, SpriteShaderProperties::CreateProperties(), true);
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
		GUILayout::Initialize(Inventory);
	}

	void Player::Move()
	{
		m_Inputs.SetInputs();
		// Try jump
		if (m_Inputs.TryJump && IsTouchingGround())
			Jump(JumpHeight, Gravity);
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
		
		UpdatePhysics(time);

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

	bool Player::IsColliding()
	{
		return
			GetPosition().X < Game::p_Tilemap->Origin.X || GetPosition().Y < Game::p_Tilemap->Origin.Y ||
			Game::p_Tilemap->DetectCollisionRect(Game::p_Tilemap->WorldToTile(GetPosition()), Game::p_Tilemap->WorldToTile(GetPosition() + GetScale()));
	}
}