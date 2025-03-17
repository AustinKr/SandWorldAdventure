#pragma once
#include "Math.h"
#include <vector>

namespace SandboxEngine::GraphicsPipeline
{
	class Mesh
	{
	public:
		// This gets added to the vertex positions when rendering
		float2 Origin;
		// This is multiplied with the vertex positions when rendering
		float2 Scale;

		// The vertices in the mesh
		std::vector<Vertex> Vertices;
		// Note: all shaders here are expected to inherit  Shader
		// Every 3 are the indices of each vertex(in Vertices) of a triangle
		std::vector<int> Triangles;

		// 1st: Head of whole triangle;
		// 2nd: count; 
		// 3rd: shader
		std::vector<int> Shaders;

		Mesh() : Origin({}), Scale({1,1}), Vertices({}), Triangles({})
		{}
	};
}