#pragma once
#include "IGameObject.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "HeaderFiles/Vector2.h"

namespace SandboxEngine::Game::GameObject
{
	class Player : public IGameObject
	{
	private:
		GraphicsPipeline::Mesh* mp_Mesh;
		Vector2 m_Vel;
		double m_Dampening;
	public:
		double AccX;
		double AccY;

		Player();
		
		// Retrieves the position of the player object. Note: The player shares its position with its mesh origin
		Vector2 GetPosition();
		void SetPosition(Vector2 newPosition);

		// Inherited via IGameObject
		void Update(Time time) override;
		void Release() override;
	};
}