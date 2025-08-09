#include "HeaderFiles/GUISystem/GUISystemFramework.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/RenderLayerNames.h"


namespace SandboxEngine::GUISystem
{
	const GUIHierarchy::UID GUIHierarchy::NULL_UID = 0;

	GUISystem::GUISystem() : p_Hierarchy(nullptr), p_Pipeline(nullptr)
	{
		/*nothing*/
	}
	bool GUISystem::Initialize(GraphicsPipeline::GraphicsPipeline2D* pPipeline)
	{
		p_Hierarchy = new GUIHierarchy(this);
		p_Pipeline = pPipeline;
		return pPipeline != nullptr;
	}

	//void GUISystem::OnScreenResize(Vector2Int newSize)
	//{
	//	// TODO: I don't think this is needed because there will be gui renderer which will figure out mesh data for elements when needed
	//	/*for(auto iter = p_Hierarchy->GetBegin(); iter != p_Hierarchy->GetEnd(); iter++)
	//	{
	//		iter->second->OnScreenResize(newSize);
	//	}*/
	//}

	void GUISystem::RegisterMesh(GraphicsPipeline::IMesh* pMesh, int id)
	{	
		// This will automatically be deleted after this point so we don't have to worry about memory leaks
		if (id >= 0)
			p_Pipeline->GetLayer(RENDERLAYERS_GUI).RegisterMesh(pMesh, id);
		else
			p_Pipeline->GetLayer(RENDERLAYERS_GUI).RegisterMesh(pMesh);
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

	bool GUISystem::GetKeyState(int glfwKey)
	{
		if (!MasterWindow::GetKeyState(glfwKey))
			return false;

		Vector2 cursorPosition;
		glfwGetCursorPos(MasterWindow::p_glfwWindow, &cursorPosition.X, &cursorPosition.Y);
		
		for (auto iter = p_Hierarchy->GetBegin(); iter != p_Hierarchy->GetEnd(); iter++)
		{
			// Collision detection
			GUITransform transform = iter->second->GetTransform();
			if (iter->second->GetActiveState() && iter->second->ShouldAffectKeyState &&
				cursorPosition.X > transform.GlobalPosition.X && cursorPosition.Y > transform.GlobalPosition.Y &&
				cursorPosition.X < transform.GlobalPosition.X + transform.GlobalScale.X && cursorPosition.Y < transform.GlobalPosition.Y + transform.GlobalScale.X)
				// cursor is overlapping a gui element!
				return false;
		}
		return true;
	}

	void GUISystem::Release()
	{
		p_Hierarchy->Release();
		p_Hierarchy = nullptr;
	}
}