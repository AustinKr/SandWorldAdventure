#include "HeaderFiles/Game/GameObjects/DebugObject.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/RenderLayerNames.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"

namespace SandboxEngine::Game::GameObject
{
	DebugObject::DebugObject() : m_Meshes{}
	{
	}

	void DebugObject::Update(Time time)
	{
		std::pair<unsigned long, double>* pMeshPair;
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			pMeshPair = &m_Meshes[i];

			if (pMeshPair->second == -1)
				continue;

			pMeshPair->second -= time.FrameDeltaTime;
			if (pMeshPair->second > 0)
				continue;
			// Unregister

			MasterWindow::Pipeline.GetLayer(RENDERLAYERS_Debug).UnregisterMesh(pMeshPair->first); // Unregister
			m_Meshes.erase(m_Meshes.begin() + i);
			i--;
		}
	}

	void DebugObject::Release()
	{
		// Just let the graphics pipeline release meshes

		m_Meshes.clear();
	}

	void DebugObject::CreatePolygon(GraphicsPipeline::IMesh* pMesh, double lifeTime)
	{
		std::optional<unsigned long> uid = MasterWindow::Pipeline.GetLayer(RENDERLAYERS_Debug).RegisterMesh(pMesh); // Register the mesh
		if(uid.has_value())
			m_Meshes.push_back(std::make_pair(uid.value(), lifeTime));
	}
	void DebugObject::CreateRectangle(Vector2 bottomLeft, Vector2 topRight, double lifeTime, int shader)
	{
		GraphicsPipeline::Mesh* pMesh = new GraphicsPipeline::Mesh();
		pMesh->Vertices.assign(
			{
				{ bottomLeft, { 0, 0, 0 } },
				{ { (float)bottomLeft.X, (float)topRight.Y }, { 0, 1, 0} },
				{ topRight, { 1, 1 } },
				{ { (float)topRight.X, (float)bottomLeft.Y }, { 1, 0, 0} }
			});

		pMesh->Triangles.assign(
			{
				0, 1, 2,
				2, 3, 0
			});
		pMesh->Shaders.assign({ 0, 2, shader });

		CreatePolygon(pMesh, lifeTime);
	}
	
	bool DebugObject::DeleteShape(unsigned long identifier)
	{
		for (auto iter = m_Meshes.begin(); iter != m_Meshes.end(); iter++)
		{
			if (iter->first != identifier)
				continue;

			MasterWindow::Pipeline.GetLayer(RENDERLAYERS_Debug).UnregisterMesh(iter->first); // Unregister
			m_Meshes.erase(iter);
			return true; // Found and deleted!
		}

		return false; // Failed to find
	}
}