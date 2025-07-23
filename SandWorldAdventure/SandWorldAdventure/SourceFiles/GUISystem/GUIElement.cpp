#include "HeaderFiles/GUISystem/GUISystemFramework.h"

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

	GUIElement::GUIElement(GUISystem* pSystem) : TransformEventHandler{}, p_System(pSystem), m_Transform{}, Parent(0), Children{}
	{/*nothing*/}
	GUIElement::GUIElement(GUISystem* pSystem, UID parent) : TransformEventHandler{}, p_System(pSystem), m_Transform{}, Parent(parent), Children{}
	{/*nothing*/}
	

	void GUIElement::RegisterComponent(Components::IGUIComponent* pComponent)
	{
		m_Components.push_back(pComponent);
		pComponent->Initialize(this);
	}
	void  GUIElement::UnregisterComponent(Components::IGUIComponent* pComponent)
	{
		for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
		{
			// Release component
			(**iter).Release();
			m_Components.erase(iter);
			return;
		}
	}
	void GUIElement::SetTransform(Vector2 positionOffset, Vector2 scaleOffset, bool isLocalSpace, int alignmentFlags)
	{
		// Get aligned coordinates
		Vector2Int screenSize = MasterWindow::Pipeline.ActiveCamera.ScreenSize;
		Vector2 alignedPosition = ApplyAligmentFlags(positionOffset, isLocalSpace ? Vector2(1, 1) : (Vector2)screenSize, alignmentFlags);
		Vector2 alignedScale = ApplyAligmentFlags(scaleOffset, isLocalSpace ? Vector2(1, 1) : (Vector2)screenSize, alignmentFlags);
		
		// Assign to corresponding coordinates
		std::pair<Vector2, Vector2> conversionPair;
		if (isLocalSpace) // assign local, then convert and assign to global
		{
			// assign local
			m_Transform.LocalPosition = alignedPosition;
			m_Transform.LocalScale = alignedScale;
			// assign global
			conversionPair = ConvertLocalToGlobal(m_Transform.LocalPosition, m_Transform.LocalScale);
			m_Transform.GlobalPosition = conversionPair.first;
			m_Transform.GlobalScale = conversionPair.second;
		}
		else			  // assign global, then convert and assign to local
		{
			// assign global
			m_Transform.GlobalPosition = alignedPosition;
			m_Transform.GlobalScale = alignedScale;
			// assign local
			conversionPair = ConvertGlobalToLocal(m_Transform.GlobalPosition, m_Transform.GlobalScale);
			m_Transform.LocalPosition = conversionPair.first;
			m_Transform.LocalScale = conversionPair.second;
		}

		// Iterate
		std::vector<UID> elements = Children;
		GUIElement* pCurrentElement = nullptr;
		for (int i = 0; i < elements.size(); i++)
		{
			// Get element
			pCurrentElement = p_System->p_Hierarchy->GetElement(elements[i]);
			
			// Update transform information
			conversionPair = ConvertLocalToGlobal(pCurrentElement->m_Transform.LocalPosition, pCurrentElement->m_Transform.LocalScale);
			pCurrentElement->m_Transform.GlobalPosition = conversionPair.first;
			pCurrentElement->m_Transform.GlobalScale = conversionPair.second;

			// Append children of current element
			elements.insert(elements.end(), pCurrentElement->Children.begin(), pCurrentElement->Children.end());
		}

		// Fire events
		TransformEventHandler.InvokeEvents(this);
	}
	GUITransform GUIElement::GetTransform()
	{
		return this->m_Transform;
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
		if (Parent != GUIHierarchy::NULL_UID)
		{
			GUIElement* pRootElement = p_System->p_Hierarchy->GetElement(Parent);
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
		if (Parent != GUIHierarchy::NULL_UID)
		{
			GUIElement* pRootElement = p_System->p_Hierarchy->GetElement(Parent);
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
			delete(pComp);
		}
		m_Components.clear();
		// Event handler
		TransformEventHandler.Release();
		// This
		delete(this);
	}
}