#pragma once
#include "Mesh.h"
#include <vector>
#include <string>

namespace GraphicsPipeline
{
	class RenderLayer
	{
	public:
		std::string Name;
		std::vector<Mesh> Meshes;

		inline RenderLayer() : Name({}), Meshes({}) {}
		inline RenderLayer(std::string name) : Name(name), Meshes({}) {}
	};
}