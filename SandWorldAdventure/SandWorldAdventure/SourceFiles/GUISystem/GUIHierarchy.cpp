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
		m_RootElements.clear();
	}

	std::unordered_map<GUIHierarchy::UID, GUIElement*>::iterator GUIHierarchy::GetBegin()
	{
		return m_ElementsRegistry.begin();
	}
	std::unordered_map<GUIHierarchy::UID, GUIElement*>::iterator GUIHierarchy::GetEnd()
	{
		return m_ElementsRegistry.end();
	}

	GUIHierarchy::UID GUIHierarchy::RegisterElement(GUIElement* pElement)
	{
		pElement->p_System = p_System;
		m_ElementsRegistry.insert(std::make_pair(m_UIDNext, pElement));
		return m_UIDNext++;
	}
	void GUIHierarchy::UnregisterElement(UID id)
	{
		m_ElementsRegistry.erase(id);
		m_RootElements.erase(id);
	}
	GUIElement* GUIHierarchy::GetElement(UID id)
	{
		return m_ElementsRegistry[id];
	}

	void GUIHierarchy::UnparentElement(UID parentID, UID childID)
	{
		GUIElement* pChild = GetElement(childID);
		GUIElement* pParent = parentID == NULL_UID ? nullptr : GetElement(parentID);
		
		pChild->m_ParentID = NULL_UID;
		m_RootElements.insert(std::make_pair(childID, pChild));
		if(pParent != nullptr)
			for (std::vector<UID>::iterator iter = pParent->m_Children.begin(); iter != pParent->m_Children.end(); iter++)
			{
				if (*iter != childID)
					continue;
				pParent->m_Children.erase(iter);
				break;
			}

		// Update transform
		pChild->UpdateTransform();
	}
	void GUIHierarchy::ParentElement(UID parentID, UID childID)
	{
		GUIElement* pChild = GetElement(childID);
		GUIElement* pParent = GetElement(parentID);

		pChild->m_ParentID = parentID;
		if (m_RootElements.contains(childID))
			m_RootElements.erase(childID);
		pParent->m_Children.push_back(childID);

		// Update transform
		pChild->UpdateTransform();
	}

	void GUIHierarchy::EraseElement(UID elementID)
	{
		GUIElement* pElement = GetElement(elementID);
		GUIElement* pParent = pElement->m_ParentID == NULL_UID ? nullptr : GetElement(pElement->m_ParentID);

		if (pParent != nullptr)
		{
			for (std::vector<UID>::iterator iter = pParent->m_Children.begin(); iter != pParent->m_Children.end(); iter++)
			{
				if (*iter != elementID)
					continue;
				pParent->m_Children.erase(iter);
				break;
			}
		}
		else if (m_RootElements.contains(elementID))
				m_RootElements.erase(elementID);

		pElement->Release();
		m_ElementsRegistry.erase(elementID);
	}
	void GUIHierarchy::EraseChildren(UID elementID)
	{
		// The root element
		GUIElement* pElement = GetElement(elementID);

		// Iterate
		GUIElement* pChild;
		std::vector<UID> elements = (std::vector<UID>&)pElement->m_Children;
		for (int i = 0; i < elements.size(); i++)
		{
			pChild = GetElement(elementID);
			// Add its children
			if (pChild->m_Children.size() > 0)
				elements.insert(elements.end(), pChild->m_Children.begin(), pChild->m_Children.end());

			// Release the element
			pElement->Release();
			m_ElementsRegistry.erase(elementID);

		}
		pElement->m_Children.clear();
	}

	void GUIHierarchy::UpdateAllTransforms()
	{
		for (auto iter : m_RootElements)
		{
			iter.second->UpdateTransform();
		}
	}
}