#pragma once

namespace GraphicsPipeline
{
	// Provides the simple layout for very basic functions needed by the application to use shaders
	class IGraphicsPipeline
	{
	private:
		// Create and compile shader code at runtime.
		/// <returns> The error code. 0 means it was successful </returns>
		virtual int CompileShaders() = 0;
		// Create a program and link it to the shaders. Also allow the passage of any needed arrays or buffers
		virtual int CreatePrograms() = 0;

	public:
		static const int GP_SUCCESS = 0;
		
		// Generates buffers and calls all necessary functions
		virtual void Initialize() = 0;

		// Called every cycle to draw to the buffer with gl api
		virtual void RenderScene() = 0;

		// Clean up shaders
		virtual void Release() = 0;

		// TODO: Functions: PassInOrUpdateSceneTriangleOrOtherData() -> this would occur each game cycle at runtime and will handle dynamically sending data through buffers
	};
}