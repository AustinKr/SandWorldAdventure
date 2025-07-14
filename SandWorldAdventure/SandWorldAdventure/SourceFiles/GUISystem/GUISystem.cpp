#include "HeaderFiles/GUISystem/GUISystem.h"
#include "HeaderFiles/GUISystem/Elements/GUIElement.h"
#include "HeaderFiles/RenderLayerNames.h"

namespace SandboxEngine::GUISystem
{
	GUISystem::GUISystem() : p_Pipeline(nullptr), m_UIDNext(0), m_ElementsRegistry{}
	{
		/*nothing*/
	}
	bool GUISystem::Initialize(GraphicsPipeline::GraphicsPipeline2D* pPipeline) 
	{
		p_Pipeline = pPipeline;
		return pPipeline != nullptr;
	}
	void GUISystem::OnScreenResize(Vector2Int newSize)
	{
		for (auto &rElement : m_ElementsRegistry)
		{
			rElement.second->OnScreenResize(newSize);
		}
	}

	void GUISystem::RegisterElement(GUIElement* pElement)
	{
		pElement->p_System = this;
		m_ElementsRegistry.insert(std::make_pair(m_UIDNext++, pElement));
	}
	void GUISystem::UnregisterElement(UID id)
	{
		m_ElementsRegistry.erase(id);
	}
	GUIElement* GUISystem::GetElement(UID id)
	{
		return m_ElementsRegistry[id];
	}

	void GUISystem::RegisterMesh(GraphicsPipeline::IMesh* pMesh, int id)
	{
		p_Pipeline->GetLayer(RENDERLAYERS_GUI).RegisterMesh(pMesh, id); // This will automatically be deleted after this point so we don't have to worry about memory leaks
	}
	GraphicsPipeline::Mesh* GUISystem::CreateTextureMesh(Vector2 origin, Vector2 size, const char* fullTexturePath)
	{
		GraphicsPipeline::Mesh* pMesh = new GraphicsPipeline::Mesh(false);
		pMesh->Vertices =
		{
			{origin, {0,0}},
			{origin + Vector2(size.X, 0), {1,0}},
			{origin + size, {1,1}},
			{origin + Vector2(0, size.Y), {0,1}}
		};
		pMesh->Triangles =
		{
			0, 1, 2,
			2, 3, 0
		};
		pMesh->Shaders =
		{
			0, 2, (int)GraphicsPipeline::GraphicsPipeline2D::GP2D_TEXTURE_SHADER
		};
		
		// Set the mesh to pass a texture along to the shader
		pMesh->Texture = GraphicsPipeline::Texture::LoadBMPTextureFromFile(fullTexturePath);
		return pMesh;
	}

	void GUISystem::Release()
	{
		for (auto pair : m_ElementsRegistry)
		{
			pair.second->Release();
		}
		m_ElementsRegistry.clear();
	}
}