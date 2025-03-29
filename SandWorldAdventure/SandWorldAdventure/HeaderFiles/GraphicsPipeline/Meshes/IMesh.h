#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"

namespace SandboxEngine::GraphicsPipeline
{
#ifndef GRAPHICS_PIPELINE2D_H
#define GRAPHICS_PIPELINE2D_H
	class GraphicsPipeline2D;//: IGraphicsPipeline
#endif

	class IMesh
	{
	public:
		virtual void Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray) = 0;
		// Calls delete(this) and releases any buffer or other data
		virtual void Release() = 0;
	};
}