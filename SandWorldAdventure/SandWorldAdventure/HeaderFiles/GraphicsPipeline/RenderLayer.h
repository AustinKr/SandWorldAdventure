#pragma once
#include "Mesh.h"
#include <vector>
#include <string>

namespace SandboxEngine::GraphicsPipeline
{
	class RenderLayer
	{
	private:
		// Contains the addresses of each mesh
		std::vector<Mesh*> m_Meshes;
	public:
		std::string Name;

		inline RenderLayer() : Name({}), m_Meshes({}) {}
		inline RenderLayer(std::string name) : Name(name), m_Meshes({}) {}

		inline int MeshesCount()
		{
			return m_Meshes.size();
		}

		inline Mesh* MeshAt(int index)
		{
			return m_Meshes.at(index);
		}

		inline void RegisterMesh(int i, Mesh* pMesh)
		{
			if (i < 0 || i >= m_Meshes.size())
				return;

			m_Meshes.insert(m_Meshes.begin() + i, pMesh);
		}
		inline void RegisterMesh(Mesh* pMesh)
		{
			m_Meshes.push_back(pMesh);
		}
		inline void UnregisterMesh(int i)
		{
			if (i < 0 || i >= m_Meshes.size())
				return;

			delete(m_Meshes[i]);
			m_Meshes.erase(m_Meshes.begin() + i);
		}
		inline void UnregisterMesh(Mesh* pMesh)
		{
			std::vector<Mesh*>::iterator iter;
			for (int i = 0; i < m_Meshes.size(); i++)
			{
				iter = m_Meshes.begin() + i;
				if (*iter != pMesh)
					continue;

				delete(pMesh);
				m_Meshes.erase(iter);
			}
		}
	};
}