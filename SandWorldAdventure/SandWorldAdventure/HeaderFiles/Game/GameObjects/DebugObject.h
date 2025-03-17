#pragma once
#include "IGameObject.h"
#include "HeaderFiles/GraphicsPipeline/Mesh.h"

namespace SandboxEngine::Game::GameObject
{
	class DebugObject : public IGameObject
	{
	private:
		int _i = 0;
		GraphicsPipeline::Mesh* mp_Mesh;
	public:

		inline DebugObject() : _i(0), mp_Mesh(nullptr)
		{
			/*nothing*/
		}
		inline DebugObject(GraphicsPipeline::Mesh* pMesh) : _i(0), mp_Mesh(pMesh)
		{
			/*nothing*/
		}

		void Update(Time time) override;
		void Release() override;
	};
}