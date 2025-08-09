#include "HeaderFiles/GUISystem/GUISystemFramework.h"
#include "HeaderFiles/GUISystem/Components/GUISpriteComponent.h"

#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/RenderLayerNames.h"

namespace SandboxEngine::GUISystem
{
	const int GUIElement::MAX_PARENT_ELEMENTS = 99;

	Vector2 GUIElement::ApplyAligmentFlags(Vector2 coord, Vector2 bounds, int alignmentFlags)
	{
		if (alignmentFlags & ElementAlignmentFlag::ALIGNMENT_RIGHT)
			coord.X = bounds.X + coord.X;
		else if (alignmentFlags & ElementAlignmentFlag::ALIGNMENT_CENTER_HORIZONTAL)
			coord.X = bounds.X / 2.0 + coord.X;

		if (alignmentFlags & ElementAlignmentFlag::ALIGNMENT_TOP)
			coord.Y = bounds.Y + coord.Y;
		else if (alignmentFlags & ElementAlignmentFlag::ALIGNMENT_CENTER_VERTICAL)
			coord.Y = bounds.Y / 2.0 + coord.Y;

		return coord;
	}

	GUIElement::GUIElement(GUISystem* pSystem, bool shouldAffectKeyState) : ShouldAffectKeyState(shouldAffectKeyState), m_IsActive(true), TransformEventHandler{}, p_System(pSystem), m_Transform{}, m_Children{}, m_ParentID{}, Identifier(pSystem->p_Hierarchy->RegisterElement(this))
	{
		pSystem->p_Hierarchy->UnparentElement(GUIHierarchy::NULL_UID, Identifier);
	}
	GUIElement::GUIElement(GUISystem* pSystem, UID parentID, bool shouldAffectKeyState) : ShouldAffectKeyState(shouldAffectKeyState), m_IsActive(true), TransformEventHandler{}, p_System(pSystem), m_Transform{}, m_Children{}, m_ParentID{parentID}, Identifier(pSystem->p_Hierarchy->RegisterElement(this))
	{
		pSystem->p_Hierarchy->ParentElement(parentID, Identifier);
	}

	std::vector<GUIElement::UID>::iterator GUIElement::GetChildrenBegin()
	{
		return m_Children.begin();
	}
	std::vector<GUIElement::UID>::iterator GUIElement::GetChildrenEnd()
	{
		return m_Children.end();
	}
	
	void GUIElement::UnregisterComponent(Components::IGUIComponent* pComponent)
	{
		for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
		{
			// Release component
			iter->second->Release();
			m_Components.erase(iter);
			return;
		}
	}

	void GUIElement::SetTransform(GUICoordinate positionCoordinate, GUICoordinate scaleCoordinate)
	{
		// Get aligned coordinates
		Vector2Int screenSize = MasterWindow::Pipeline.ActiveCamera.ScreenSize;
		Vector2 alignedPosition = ApplyAligmentFlags(positionCoordinate.Offset, positionCoordinate.IsLocalSpace? Vector2(1, 1) : (Vector2)screenSize, positionCoordinate.AlignmentFlags);
		Vector2 alignedScale = ApplyAligmentFlags(scaleCoordinate.Offset, scaleCoordinate.IsLocalSpace ? Vector2(1, 1) : (Vector2)screenSize, scaleCoordinate.AlignmentFlags);
		
		// Assign to corresponding coordinates
		if (positionCoordinate.IsLocalSpace) // assign local, then convert and assign to global
		{
			m_Transform.LocalPosition = alignedPosition;
			m_Transform.GlobalPosition = ConvertLocalToGlobal(m_Transform.LocalPosition);
		}
		else
		{
			m_Transform.GlobalPosition = alignedPosition;
			m_Transform.LocalPosition = ConvertGlobalToLocal(m_Transform.GlobalPosition);
		}
		if (scaleCoordinate.IsLocalSpace) // assign global, then convert and assign to local
		{
			m_Transform.LocalScale = alignedScale;
			m_Transform.GlobalScale = ConvertLocalToGlobal(m_Transform.LocalScale, false);
		}
		else
		{
			m_Transform.GlobalScale = alignedScale;
			m_Transform.LocalScale = ConvertGlobalToLocal(m_Transform.GlobalScale, false);
		}

		// Iterate
		std::pair<Vector2, Vector2> conversionPair;
		std::vector<UID> elements = (std::vector<UID>&)m_Children;
		GUIElement* pCurrentElement = nullptr;
		for (int i = 0; i < elements.size(); i++)
		{
			// Get element
			pCurrentElement = p_System->p_Hierarchy->GetElement(elements[i]);
			
			// Update transform information
			conversionPair = pCurrentElement->ConvertLocalToGlobal(pCurrentElement->m_Transform.LocalPosition, pCurrentElement->m_Transform.LocalScale);
			pCurrentElement->m_Transform.GlobalPosition = conversionPair.first;
			pCurrentElement->m_Transform.GlobalScale = conversionPair.second;
			pCurrentElement->TransformEventHandler.InvokeEvents(pCurrentElement);

			// Append children of current element
			elements.insert(elements.end(), pCurrentElement->m_Children.begin(), pCurrentElement->m_Children.end());
		}

		// Fire events
		TransformEventHandler.InvokeEvents(this);
	}
	GUITransform GUIElement::GetTransform()
	{
		return this->m_Transform;
	}
	void GUIElement::UpdateTransform()
	{
		SetTransform({ m_Transform.LocalPosition, true }, { m_Transform.LocalScale, true });
	}

	void GUIElement::SetActiveState(bool isActive)
	{
		if (isActive == m_IsActive) 
			return;
		m_IsActive = isActive;

		std::vector<UID> elements = (std::vector<UID>&)m_Children;
		GUIElement* pCurrentElement = nullptr;
		for (int i = 0; i < elements.size(); i++)
		{
			// Get element
			pCurrentElement = p_System->p_Hierarchy->GetElement(elements[i]);
			// Set
			pCurrentElement->m_IsActive = isActive;
			Components::GUISpriteComponent* pSpriteComp = pCurrentElement->GetComponent<Components::GUISpriteComponent>(ComponentTags::COMPONENT_TAG_SPRITE);
			if(pSpriteComp != nullptr)
				pSpriteComp->GetMesh()->IsActive = isActive;

			// Append children of current element
			elements.insert(elements.end(), pCurrentElement->m_Children.begin(), pCurrentElement->m_Children.end());
		}
	}
	bool GUIElement::GetActiveState()
	{
		return m_IsActive;
	}

	Vector2 GUIElement::ConvertGlobalToLocal(Vector2Int globalCoord, bool applyOrigin)
	{
		return applyOrigin ? ConvertGlobalToLocal(globalCoord, Vector2Int()).first : ConvertGlobalToLocal(Vector2Int(), globalCoord).second;
	}
	std::pair<Vector2, Vector2> GUIElement::ConvertGlobalToLocal(Vector2Int globalPosition, Vector2Int globalScale)
	{
		// Get either screen or parent for global(screen) coordinates to base transformations on
		Vector2Int rootScale = MasterWindow::Pipeline.ActiveCamera.ScreenSize;
		Vector2Int rootOrigin = {};
		if (m_ParentID != GUIHierarchy::NULL_UID)
		{
			GUIElement* pRootElement = p_System->p_Hierarchy->GetElement(m_ParentID);
			rootScale = pRootElement->m_Transform.GlobalScale;
			rootOrigin = pRootElement->m_Transform.GlobalPosition;
		}

		return std::make_pair((Vector2)(globalPosition - rootOrigin) / (Vector2)rootScale, (Vector2)globalScale / (Vector2)rootScale);
	}
	Vector2Int GUIElement::ConvertLocalToGlobal(Vector2 localCoord, bool applyOrigin)
	{
		return applyOrigin ? ConvertLocalToGlobal(localCoord, Vector2()).first : ConvertLocalToGlobal(Vector2(), localCoord).second;
	}
	std::pair<Vector2Int, Vector2Int> GUIElement::ConvertLocalToGlobal(Vector2 localPosition, Vector2 localScale)
	{
		Vector2Int rootScale = MasterWindow::Pipeline.ActiveCamera.ScreenSize;
		Vector2Int rootOrigin = {};
		if (m_ParentID != GUIHierarchy::NULL_UID)
		{
			GUIElement* pRootElement = p_System->p_Hierarchy->GetElement(m_ParentID);
			rootScale = pRootElement->m_Transform.GlobalScale;
			rootOrigin = pRootElement->m_Transform.GlobalPosition;
		}

		return std::make_pair(localPosition * rootScale + rootOrigin, localScale * rootScale);
	}

	void GUIElement::Release()
	{
		// Release componenets
		for (auto pComp : m_Components)
		{
			delete(pComp.second);
		}
		m_Components.clear();
		// Event handler
		TransformEventHandler.Release();
		// This
		delete(this);
	}
}