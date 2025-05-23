#pragma once
#include "Meshes/IMesh.h"
#include <string>
#include <unordered_map>
#include <optional> 

namespace SandboxEngine::GraphicsPipeline
{
	class RenderLayer
	{
	public: typedef unsigned long UID;

	private:
		// Contains the addresses of each mesh
		std::unordered_map<UID, IMesh*> m_Meshes; // TODO: Could further divide frontground and background with a map of int, iMesh*
		UID m_Next;
	public:
		std::string Name;

		inline RenderLayer() : Name({}), m_Meshes({}), m_Next{ 0 } {}
		inline RenderLayer(std::string name) : Name(name), m_Meshes({}), m_Next{ 0 } {}

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
		inline std::unordered_map<UID, IMesh*>::iterator begin() { return m_Meshes.begin(); }
		inline std::unordered_map<UID, IMesh*>::iterator end() { return m_Meshes.end(); }

		inline std::optional<UID> RegisterMesh(IMesh* pMesh)
		{
			auto pair = m_Meshes.insert(std::make_pair(m_Next++, pMesh));
			if (!pair.second)
				return {};

			return pair.first->first;
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