#pragma once
#include "IMesh.h"
#include "HeaderFiles/GraphicsPipeline/Math.h"
#include <vector>

namespace SandboxEngine::GraphicsPipeline
{
	class Mesh : public IMesh
	{
	public:
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

		Mesh();
		
		virtual void Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray) override;
		virtual void Release() override;
	};
}