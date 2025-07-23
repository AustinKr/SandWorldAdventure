#pragma once
#include "IGUIComponent.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "HeaderFiles/Event/DelegateTypes/IEventDelegate.h"

namespace SandboxEngine::GUISystem::Components
{
	struct GUISpriteComponent : public IGUIComponent
	{
	public:
		// Creates mesh
		virtual void Initialize(GUIElement* pElement) override;
		virtual void Release() override;

		// Constructor gets information to create a mesh
		GUISpriteComponent(int meshID, const char* imagePath);
	private:
		int m_MeshID;
		const char* m_ImagePath;
		GraphicsPipeline::Mesh* mp_Mesh;

		// Updates mesh when transformations occur
		struct TransformEventDelegate : Event::IEventDelegate
		{
		private:
			GUISpriteComponent *mp_Sprite;

		public:
			TransformEventDelegate(GUISpriteComponent* pSprite);
			virtual void Invoke(void* pElement) override;
		};
	};
}