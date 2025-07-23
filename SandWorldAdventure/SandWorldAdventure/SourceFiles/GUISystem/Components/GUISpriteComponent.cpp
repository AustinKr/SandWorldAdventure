#include "HeaderFiles/GUISystem/Components/GUISpriteComponent.h"
#include "HeaderFiles/GUISystem/GUISystemFramework.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GUISystem::Components
{
	void GUISpriteComponent::Initialize(GUIElement* pElement)
	{
		// Mesh
		mp_Mesh = pElement->p_System->CreateTextureMesh({ 0,0 }, { 1,1 }, m_ImagePath);
		mp_Mesh->color = float4(1, 1, 1, 1);
		pElement->p_System->RegisterMesh(mp_Mesh, m_MeshID);

		// Subscribe event
		pElement->TransformEventHandler.SubscribeEvent(new TransformEventDelegate(this));
	}
	void GUISpriteComponent::Release()
	{
		delete(this);
	}

	GUISpriteComponent::GUISpriteComponent(int meshID, const char* imagePath) : m_MeshID(meshID), m_ImagePath(imagePath)
	{/*nothing*/}


	// - Event -
	GUISpriteComponent::TransformEventDelegate::TransformEventDelegate(GUISpriteComponent* pSprite) : mp_Sprite(pSprite)
	{/*nothing*/ }
	void GUISpriteComponent::TransformEventDelegate::Invoke(void* pElement)
	{
		GUITransform transform = reinterpret_cast<GUIElement*>(pElement)->GetTransform();

		mp_Sprite->mp_Mesh->Origin = transform.GlobalPosition;
		mp_Sprite->mp_Mesh->Scale = transform.GlobalScale;
	}
}