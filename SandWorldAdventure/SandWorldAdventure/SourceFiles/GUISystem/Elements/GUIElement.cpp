#include "HeaderFiles/GUISystem/Elements/GUIElement.h"
#include "HeaderFiles/GUISystem/GUISystem.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/RenderLayerNames.h"

namespace SandboxEngine::GUISystem
{
	GUIElement::GUIElement(GUISystem* pSystem) : p_System(pSystem), Parent(0), Children{}
	{

	}
	GUIElement::GUIElement(GUISystem* pSystem, UID parent) : p_System(pSystem), Parent(parent), Children{}
	{

	}
	void GUIElement::EraseChild(UID id)
	{
		for (int i = 0; i < Children.size(); i++)
		{
			if (Children[i] != id)
				continue;
			p_System->GetElement(id)->Release();
			Children.erase(Children.begin() + i);
		}
	}

	void GUIElement::Release()
	{
		delete(this);
	}

	void GUIElement::OnScreenResize(Vector2Int newSize)
	{
		SetPosition(m_OriginalPosition.first, m_OriginalPosition.second);
		SetScale(m_OriginalScale.first, m_OriginalScale.second);
	}

	void GUIElement::SetPosition(Vector2Int position, int aligmentFlags)
	{
		Vector2Int screenSize = MasterWindow::GetScreenSize();
		Vector2Int pos = position;

		if (aligmentFlags & ElementAlignmentFlag::ALIGNMENT_RIGHT)
			pos.X = screenSize.X + pos.X;
		if (aligmentFlags & ElementAlignmentFlag::ALIGNMENT_TOP)
			pos.Y = screenSize.Y + pos.Y;

		if (aligmentFlags & ElementAlignmentFlag::ALIGNMENT_CENTER_VERTICAL)
			pos.Y = screenSize.Y / 2 + pos.Y;
		if (aligmentFlags & ElementAlignmentFlag::ALIGNMENT_CENTER_HORIZONTAL)
			pos.X = screenSize.X / 2 + pos.X;

		m_OriginalPosition = std::make_pair(position, aligmentFlags);
		SetElementPosition(pos);
	}
	void GUIElement::SetScale(Vector2Int scale, int aligmentFlags)
	{
		Vector2Int screenSize = MasterWindow::GetScreenSize();
		Vector2Int s = scale;

		if (aligmentFlags & ElementAlignmentFlag::ALIGNMENT_RIGHT)
			s.X = screenSize.X + s.X;
		if (aligmentFlags & ElementAlignmentFlag::ALIGNMENT_TOP)
			s.Y = screenSize.Y + s.Y;

		if (aligmentFlags & ElementAlignmentFlag::ALIGNMENT_CENTER_VERTICAL)
			s.Y = screenSize.Y / 2 + s.Y;
		if (aligmentFlags & ElementAlignmentFlag::ALIGNMENT_CENTER_HORIZONTAL)
			s.X = screenSize.X / 2 + s.X;

		m_OriginalScale = std::make_pair(scale, aligmentFlags);
		SetElementScale(s);
	}
}