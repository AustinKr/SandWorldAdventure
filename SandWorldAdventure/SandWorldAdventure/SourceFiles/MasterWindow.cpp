//#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/Math.h"

#include <iostream>
#include <string>

//#include "KeyboardInterfaceAPI/Headers/KeyboardInterface/KeyboardState.h"


namespace SandboxEngine
{
	std::unordered_map<int, int> MasterWindow::m_KeyStates;

	const char* const MasterWindow::p_WindowTitle = "Sand World Adventure GL";
	GLFWwindow* MasterWindow::p_glfwWindow;

	SandboxEngine::GraphicsPipeline::GraphicsPipeline2D MasterWindow::Pipeline;
	SandboxEngine::GUISystem::GUISystem MasterWindow::GraphicalUserInterfaceSystem;

	Event::EventHandler<> MasterWindow::KeyStrokeEventHandler;
	Event::EventHandler<> MasterWindow::MouseButtonEventHandler;
	Event::EventHandler<> MasterWindow::ScreenResizeEventHandler;

	int MasterWindow::InitializeGLFW()
	{
		// - Set hints -
		glfwInitHint(GLFW_PLATFORM, GLFW_ANY_PLATFORM);

		// Version of GLFW is 3.4 and we want to use the matching client API version of the window's context. This will throw an error if the client does not have the correct version of opengl.
		glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 4);

		// TODO: What is this?
		glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		return glfwInit();
	}
	void MasterWindow::CreateMainWindow()
	{
		p_glfwWindow = glfwCreateWindow(800, 600, p_WindowTitle, NULL, NULL);
		if (!p_glfwWindow)
		{
			// Window or OpenGL context creation failed
			glfwTerminate();
			p_glfwWindow = nullptr;
			return;
		}
		glfwMakeContextCurrent(p_glfwWindow);  // Makes the context of the window current for the calling thread
		glfwSwapInterval(1); // Set the time between buffer swaps

		// Update the viewport with the size of the frame buffer
		int width = 0, height = 0;
		glfwGetFramebufferSize(p_glfwWindow, &width, &height);
		glViewport(0, 0, width, height);

		glClearColor(0.3f, 0.3f, 0.8f, 1.0f); // Set clear color

		return;
	}
	void MasterWindow::SetGLFWEventCallbacks()
	{
		// Set event callbacks
		glfwSetFramebufferSizeCallback(p_glfwWindow, FrameBufferSize_Callback); // Resize event
		glfwSetKeyCallback(p_glfwWindow, Key_Callback); // Key input event
		glfwSetCharCallback(p_glfwWindow, Character_Callback); // Character input event
		glfwSetMouseButtonCallback(p_glfwWindow, MouseButton_Callback);
	}

	// Callbacks
	void MasterWindow::FrameBufferSize_Callback(GLFWwindow* pWindow, int width, int height)
	{
		glViewport(0, 0, width, height); // Update the viewport to the same size as the buffer so the window coordinates are correctly computed
		Pipeline.ActiveCamera.ScreenSize = SandboxEngine::Vector2Int(width, height);
		GraphicalUserInterfaceSystem.p_Hierarchy->UpdateAllTransforms();

		// Invoke events
		int *pArgs = new int[2] {width, height};
		ScreenResizeEventHandler.InvokeEvents(pArgs);
		delete[](pArgs);
	}
	void MasterWindow::Key_Callback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
	{
		if (action != GLFW_PRESS && action != GLFW_RELEASE)
			return;
		
		if (m_KeyStates.contains(key))
			m_KeyStates.at(key) = action;
		else
			m_KeyStates.insert(std::make_pair(key, action));

		KeyStrokeEventHandler.InvokeEvents(nullptr);
	}
	void MasterWindow::Character_Callback(GLFWwindow* pWindow, unsigned int codepoint)
	{
		std::cout << (char)codepoint;
		std::cout << '\n';
	}
	void MasterWindow::MouseButton_Callback(GLFWwindow* pWindow, int button, int action, int mods)
	{
		if (action != GLFW_PRESS && action != GLFW_RELEASE)
			return;

		if (m_KeyStates.contains(button))
			m_KeyStates.at(button) = action;
		else
			m_KeyStates.insert(std::make_pair(button, action));

		MouseButtonEventHandler.InvokeEvents(nullptr);
	}

	void MasterWindow::InitializeWindow()
	{
		// Initialize GLFW
		if (!InitializeGLFW())
			exit(EXIT_FAILURE);
		std::cout << glfwGetVersionString();
		std::cout << "\n";

		// Create window
		CreateMainWindow();
		if (p_glfwWindow == nullptr)
			exit(EXIT_FAILURE);
		
		// Initialize glew
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}

		// Register events
		SetGLFWEventCallbacks();

		// Initialize other systems
		Pipeline.Initialize();
		GraphicalUserInterfaceSystem.Initialize(&Pipeline);
	}

	void MasterWindow::Release()
	{
		// Release event handlers
		MasterWindow::KeyStrokeEventHandler.Release();
		MasterWindow::MouseButtonEventHandler.Release();
		MasterWindow::ScreenResizeEventHandler.Release();

		// Release other
		GraphicalUserInterfaceSystem.Release();
		Pipeline.Release();
		glfwDestroyWindow(MasterWindow::p_glfwWindow);
	}

	int MasterWindow::GetKeyState(int glfwKey)
	{
		if (m_KeyStates.contains(glfwKey))
			return m_KeyStates.at(glfwKey);

		return GLFW_RELEASE;
	}
}