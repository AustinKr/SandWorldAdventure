#pragma once
#include "IGameObject.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/IMesh.h"
#include <vector>

namespace SandboxEngine::Game::GameObject
{
	// An object that allows shapes to be drawn to the screen at anytime
	class DebugObject : public IGameObject
	{
	private:
		// <pair<mesh uid, lifetime>>
		std::vector<std::pair<unsigned long, double>> m_Meshes;
		int m_NextID;
	public:

		DebugObject();

		void Update(Time time) override;
		void Release() override;

		/// <summary>
		/// Registers mesh and keeps track of it
		/// </summary>
		/// <param name="lifetime">Time in seconds before it is deleted. Use -1 to denote a shape that lasts indefinetely</param>
		void CreatePolygon(GraphicsPipeline::IMesh* pMesh, double lifeTime);
		void CreateRectangle(Vector2 bottomLeft, Vector2 topRight, double lifeTime, int shader);

		bool DeleteShape(unsigned long identifier);
	};
}