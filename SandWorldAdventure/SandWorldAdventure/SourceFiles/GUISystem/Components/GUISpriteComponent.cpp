#include "HeaderFiles/GUISystem/Components/GUISpriteComponent.h"
#include "HeaderFiles/GUISystem/GUISystemFramework.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GUISystem::Components
{
	ComponentTags GUISpriteComponent::GetTag()
	{
		return ComponentTags::COMPONENT_TAG_SPRITE;
	}

	void GUISpriteComponent::Initialize(GUIElement* pElement)
	{
		// Mesh
		mp_Mesh = pElement->p_System->CreateTextureMesh({ 0,0 }, { 1,1 }, m_ImagePath);
		mp_Mesh->color = float4(1, 1, 1, 1);
		pElement->p_System->RegisterMesh(mp_Mesh, m_MeshID);

		// Subscribe event
		pElement->TransformEventHandler.SubscribeEvent(Event::BaseEventDelegate<GUIElement*, void*>(OnTransform, nullptr));
	}
	void GUISpriteComponent::Release()
	{
		delete(this);
	}

	GUISpriteComponent::GUISpriteComponent(int meshID, const char* imagePath) : m_MeshID(meshID), m_ImagePath(imagePath)
	{/*nothing*/}

	GraphicsPipeline::Mesh* GUISpriteComponent::GetMesh()
	{
		return mp_Mesh;
	}

	void GUISpriteComponent::OnTransform(GUIElement* pElement, void*)
	{
		GUITransform transform = pElement->GetTransform();
		GUISpriteComponent* sprite = pElement->GetComponent<GUISpriteComponent>(ComponentTags::COMPONENT_TAG_SPRITE);

		sprite->mp_Mesh->Origin = transform.GlobalPosition;
		sprite->mp_Mesh->Scale = transform.GlobalScale;
	}
}