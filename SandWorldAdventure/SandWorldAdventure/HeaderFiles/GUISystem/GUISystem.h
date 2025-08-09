#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "HeaderFiles/Math.h"

#define GUISYSTEM_H
namespace SandboxEngine::GUISystem
{
#ifndef GUIELEMENT_H
#define GUIELEMENT_H
	class GUIElement;
#endif
#ifndef GUIHIERARCHY_H
#define GUIHIERARCHY_H
	class GUIHierarchy;
#endif

	// TODO: maybe use an acceleration structure if there gets to be a lot of elements
	// Dependent on the graphics pipeline 2d to be initialized
	class GUISystem
	{
	public:
		GUIHierarchy *p_Hierarchy;
		GraphicsPipeline::GraphicsPipeline2D *p_Pipeline;
		
		GUISystem();
		bool Initialize(GraphicsPipeline::GraphicsPipeline2D *pPipeline);

		/// <summary>
		/// Registers the mesh with the graphics pipeline 2D on the gui render layer
		/// </summary>
		/// <param name="prMesh"></param>
		/// <param name="id">used to identify the mesh in a map, and from -infinity to infinity as the order to iterate when drawing from background to foreground respectively</param>
		void RegisterMesh(GraphicsPipeline::IMesh* pMesh, int id = -1);

		// Does not register the mesh
		GraphicsPipeline::Mesh* CreateTextureMesh(Vector2 origin, Vector2 size, const char* fullTexturePath);
		/*void CreatePolygonMesh();
		void CreateTextMesh();*/

		// TODO: Uses slow algorithim for gui collision detection
		// TODO: Could create a function for mouse inputs that goes through the GUISystem so that mouse clicks don't register over gui
		// used to check a key state, but will always return false if: the element is active, and the mouse is over an active gui element with the member ShouldAffectKeyState == true
		bool GetKeyState(int glfwKey);

		void Release();
	};
}