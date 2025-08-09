#pragma once
#include "IMesh.h"
#include "HeaderFiles/Math.h"
#include "HeaderFiles/GraphicsPipeline/Texture.h"
#include <vector>

#include "HeaderFiles/Event/EventHandler.h"

namespace SandboxEngine::GraphicsPipeline
{
	class Mesh : public IMesh
	{
	private:
		void TransformVertexCoordinates(GraphicsPipeline::GraphicsPipeline2D* pPipeline, Vertex buffer[3]);

	public:
		// TODO: PreRender May not be needed at all
		// (Takes no argumenets) Called before the mesh is drawn
		Event::EventHandler<> PreRenderEventHandler;

		// Whether to draw the mesh
		bool IsActive;

		// This gets added to the vertex positions when rendering
		float2 Origin;
		// This is multiplied with the vertex positions when rendering
		float2 Scale;

		// The vertices in the mesh
		std::vector<Vertex> Vertices;
		// Every 3 are the indices of each vertex(in Vertices) of a triangle
		std::vector<int> Triangles;
		// Note: all shaders here are expected to inherit  Shader
		// 1st: Head of triangle;
		// 2nd: count; 
		// 3rd: shader
		std::vector<int> Shaders;

		// If true, transforms vertex coordinates from world space; if false, transforms the vertex coordinate from screen space
		bool UseWorldCoordinates;
		// The texture(optional)
		Texture Texture;
		// Color
		float4 color;

		Mesh(bool useWorldCoordinates = true);
		
		virtual void Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray) override;
		virtual void Release() override;
	};
}