#pragma once
#include "Mesh.h"
#include <vector>
#include <string>

namespace SandboxEngine::GraphicsPipeline
{
	class RenderLayer
	{
	public:
		std::string Name;
		// Contains the addresses of each mesh
		std::vector<Mesh*> Meshes;

		inline RenderLayer() : Name({}), Meshes({}) {}
		inline RenderLayer(std::string name) : Name(name), Meshes({}) {}
	};
}