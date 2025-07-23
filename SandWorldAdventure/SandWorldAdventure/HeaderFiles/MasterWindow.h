#pragma once
#include <string>

#include <GL/glew.h>
// If none of the inclusion macros are defined, the standard OpenGL GL / gl.h header(OpenGL / gl.h on macOS) is included when glfw3.h is
#include <GLFW/glfw3.h>

#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"
#include "HeaderFiles/GUISystem/GUISystemFramework.h"
#include "HeaderFiles/Event/EventHandler.h"

namespace SandboxEngine
{
	static class MasterWindow
	{
	private:
		static std::unordered_map<int, int> m_KeyStates;

		// Major internal functions

		static int InitializeGLFW();
		static void CreateMainWindow();
		static void SetGLFWEventCallbacks();

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

		// Has no arguments. Use GetKeyState(int) to detect keys
		static Event::EventHandler<> KeyStrokeEventHandler;
		// Has no arguments. Use GeyKeyState(int) to detect mouse buttons
		static Event::EventHandler<> MouseButtonEventHandler;
		// Has int[2] with width and height as arguments
		static Event::EventHandler<> ScreenResizeEventHandler;

		// Initializes glfw and glew and creates a window.
		static void InitializeWindow();
		static void Release();

		/// <returns>GLFW_RELEASE or GLFW_PRESS and includes mouse buttons</returns>
		static int GetKeyState(int glfwKey);
	};
}