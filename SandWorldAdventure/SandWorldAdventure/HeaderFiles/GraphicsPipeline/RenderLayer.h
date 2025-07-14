#pragma once
#include "Meshes/IMesh.h"
#include <string>
#include <map>

namespace SandboxEngine::GraphicsPipeline
{
	class RenderLayer
	{
	public: typedef int UID;

	private:
		// Contains the addresses of each mesh
		std::map<UID, IMesh*> m_Meshes;
		
	public:
		std::string Name;

		inline RenderLayer() : Name({}), m_Meshes({}) {}
		inline RenderLayer(std::string name) : Name(name), m_Meshes({}) {}

		inline int MeshesCount()
		{
			return m_Meshes.size();
		}

		inline IMesh* MeshAt(UID identifier)
		{
			if (!m_Meshes.contains(identifier))
				return nullptr;
			return m_Meshes.at(identifier);
		}
		inline std::map<UID, IMesh*>::iterator begin() { return m_Meshes.begin(); }
		inline std::map<UID, IMesh*>::iterator end() { return m_Meshes.end(); }

		inline bool RegisterMesh(IMesh* pMesh, UID id)
		{
			return m_Meshes.insert(std::make_pair(id, pMesh)).second;
		}
		inline bool UnregisterMesh(UID identifier)
		{
			if (!m_Meshes.contains(identifier))
				return false;

			m_Meshes[identifier]->Release();
			m_Meshes.erase(identifier);
			return true;
		}
		inline bool UnregisterMesh(IMesh* pMesh)
		{
			for (auto& iter : m_Meshes)
			{
				if (iter.second != pMesh)
					continue;

				iter.second->Release();
				m_Meshes.erase(iter.first);
				return true;
			}
			return false;
		}
	};
}