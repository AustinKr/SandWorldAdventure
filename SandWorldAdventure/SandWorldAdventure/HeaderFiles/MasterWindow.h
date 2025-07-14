#pragma once
#include <string>

#include <GL/glew.h>
// If none of the inclusion macros are defined, the standard OpenGL GL / gl.h header(OpenGL / gl.h on macOS) is included when glfw3.h is
#include <GLFW/glfw3.h>

#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"
#include "HeaderFiles/GUISystem/GUISystem.h"

namespace SandboxEngine
{
	static class MasterWindow
	{
	private:
		static std::unordered_map<int, int> m_KeyStates;

		// Major internal functions

		static int InitializeGLFW();
		static void CreateMainWindow();
		static void SetEventCallbacks();
		static void InitializeGraphics();
		static void InitializeGUI();

		// Callbacks

		static void FrameBufferSize_Callback(GLFWwindow* pWindow, int width, int height);
		static void Key_Callback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
		static void Character_Callback(GLFWwindow* pWindow, unsigned int codepoint);
		static void MouseButton_Callback(GLFWwindow* pWindow, int button, int action, int mods);
	public:
		static const char* const p_WindowTitle;
		static GLFWwindow* p_glfwWindow;

		// The graphics pipeline
		static SandboxEngine::GraphicsPipeline::GraphicsPipeline2D Pipeline;
		// The graphical user interface system
		static SandboxEngine::GUISystem::GUISystem UserInterfaceSystem;

		// Initializes glfw and glew and creates a window.
		static void InitializeWindow();
		static void Release();

		static Vector2Int GetScreenSize();
		static void SetScreenSize(Vector2Int size);

		/// <returns>GLFW_RELEASE or GLFW_PRESS and includes mouse buttons</returns>
		static int GetKeyState(int glfwKey);
	};
}