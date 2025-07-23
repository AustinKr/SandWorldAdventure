#include "HeaderFiles/GUISystem/GUISystemFramework.h"

namespace SandboxEngine::GUISystem
{
	GUIHierarchy::GUIHierarchy(GUISystem* pSystem) : p_System(pSystem), m_UIDNext(1), m_ElementsRegistry{}
	{
		// Error check

		if (pSystem != nullptr)
			return;
		fprintf(stderr, "pSystem was nullptr!");
#ifdef _DEBUG
		throw std::exception();
#endif
	}
	void GUIHierarchy::Release()
	{
		for (auto pair : m_ElementsRegistry)
		{
			pair.second->Release();
		}
		m_ElementsRegistry.clear();
	}

	std::unordered_map<GUIHierarchy::UID, GUIElement*>::iterator GUIHierarchy::GetBegin()
	{
		return m_ElementsRegistry.begin();
	}
	std::unordered_map<GUIHierarchy::UID, GUIElement*>::iterator GUIHierarchy::GetEnd()
	{
		return m_ElementsRegistry.end();
	}

	void GUIHierarchy::RegisterElement(GUIElement* pElement)
	{
		pElement->p_System = p_System;
		m_ElementsRegistry.insert(std::make_pair(m_UIDNext++, pElement));
	}
	void GUIHierarchy::UnregisterElement(UID id)
	{
		m_ElementsRegistry.erase(id);
	}
	GUIElement* GUIHierarchy::GetElement(UID id)
	{
		return m_ElementsRegistry[id];
	}

}