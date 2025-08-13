#pragma once
#include "IGUIComponent.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"

namespace SandboxEngine::GUISystem::Components
{
	struct GUISpriteComponent : public IGUIComponent
	{
	public:
		virtual ComponentTags GetTag() override;

		// Creates mesh
		virtual void Initialize(GUIElement* pElement) override;
		virtual void Release() override;

		GraphicsPipeline::Mesh* GetMesh();

		// Constructor gets information to create a mesh
		GUISpriteComponent(int meshID, const char* imagePath);
	private:
		int m_MeshID;
		const char* m_ImagePath;
		GraphicsPipeline::Mesh* mp_Mesh;

		// Updates mesh when transformations occur
		static void OnTransform(GUIElement* pElement, void*);
	};
}