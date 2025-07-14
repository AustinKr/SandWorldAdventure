#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "HeaderFiles/Math.h"
#include <unordered_map>

#define GUISYSTEM_H
namespace SandboxEngine::GUISystem
{
#ifndef GUIELEMENT_H
#define GUIELEMENT_H
	class GUIElement;
#endif

	// Dependent on the graphics pipeline 2d to be initialized
	class GUISystem
	{
	public:
		typedef unsigned long int UID;

		GraphicsPipeline::GraphicsPipeline2D* p_Pipeline;
		
		GUISystem();
		bool Initialize(GraphicsPipeline::GraphicsPipeline2D* pPipeline);
		// Updates gui elements
		void OnScreenResize(Vector2Int newSize);
		
		void RegisterElement(GUIElement* pElement);
		void UnregisterElement(UID id);
		GUIElement* GetElement(UID id);

		/// <summary>
		/// Registers the mesh with the graphics pipeline 2D on the gui render layer
		/// </summary>
		/// <param name="prMesh"></param>
		/// <param name="id">used to identify the mesh in a map, and from -infinity to infinity as the order to iterate when drawing from background to foreground respectively</param>
		void RegisterMesh(GraphicsPipeline::IMesh* pMesh, int id);
		// Does not register the mesh
		GraphicsPipeline::Mesh* CreateTextureMesh(Vector2 origin, Vector2 size, const char* fullTexturePath);
		/*void CreatePolygonMesh();
		void CreateTextMesh();*/

		void Release();

	private:
		UID m_UIDNext;
		std::unordered_map<UID, GUIElement*> m_ElementsRegistry;
	};
}