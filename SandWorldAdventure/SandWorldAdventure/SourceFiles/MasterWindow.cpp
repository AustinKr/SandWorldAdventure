//#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/GUISystem/Elements/GUIElement.h"
#include "HeaderFiles/GUISystem/Elements/GUISprite.h"
#include "HeaderFiles/RenderLayerNames.h"
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
	SandboxEngine::GUISystem::GUISystem MasterWindow::UserInterfaceSystem;

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
	void MasterWindow::SetEventCallbacks()
	{
		// Set event callbacks
		glfwSetFramebufferSizeCallback(p_glfwWindow, FrameBufferSize_Callback); // Resize event
		glfwSetKeyCallback(p_glfwWindow, Key_Callback); // Key input event
		glfwSetCharCallback(p_glfwWindow, Character_Callback); // Character input event
		glfwSetMouseButtonCallback(p_glfwWindow, MouseButton_Callback);
	}
	void MasterWindow::InitializeGraphics()
	{
		Pipeline.Initialize();
		int width, height;
		glfwGetWindowSize(p_glfwWindow, &width, &height);
		Pipeline.ActiveCamera.ScreenSize = SandboxEngine::Vector2Int(width, height);
		Pipeline.ActiveCamera.Scale = SandboxEngine::Vector2(150, 150);
		Pipeline.ActiveCamera.Origin = SandboxEngine::Vector2(0, 0);

		// Create layers
		Pipeline.InsertLayer(RENDERLAYERS_Tilemap0, { "Tilemap0" });
		Pipeline.InsertLayer(RENDERLAYERS_Objects, { "Objects" });
		Pipeline.InsertLayer(RENDERLAYERS_Characters, { "Characters" });
		Pipeline.InsertLayer(RENDERLAYERS_GUI, { "GUI" });
		Pipeline.InsertLayer(RENDERLAYERS_Debug, { "Debug" });

		// TODO: Game specific shaders(those that are not basic and required for every graphics pipeline) should really be registered here
	}
	void MasterWindow::InitializeGUI()
	{
		UserInterfaceSystem.Initialize(&Pipeline);
		
		GUISystem::GUISprite* pElement = new GUISystem::GUISprite(&UserInterfaceSystem, 0, std::string(GraphicsPipeline::GraphicsPipeline2D::PROJECT_DIRECTORY).append("Resources/GUI/Background.bmp").c_str());
		pElement->SetPosition(Vector2(25, 25), GUISystem::ALIGNMENT_LEFT | GUISystem::ALIGNMENT_BOTTOM);
		pElement->SetScale(Vector2(100, -100), GUISystem::ALIGNMENT_LEFT | GUISystem::ALIGNMENT_TOP);
		UserInterfaceSystem.RegisterElement(pElement); // Needed for resizing and other stuff
	}

	// Callbacks
	void MasterWindow::FrameBufferSize_Callback(GLFWwindow* pWindow, int width, int height)
	{
		glViewport(0, 0, width, height); // Update the viewport to the same size as the buffer so the window coordinates are correctly computed
		Pipeline.ActiveCamera.ScreenSize = SandboxEngine::Vector2Int(width, height);
		UserInterfaceSystem.OnScreenResize(Vector2Int(width, height));
	}
	void MasterWindow::Key_Callback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
	{
		if (action != GLFW_PRESS && action != GLFW_RELEASE)
			return;
		
		if (m_KeyStates.contains(key))
			m_KeyStates.at(key) = action;
		else
			m_KeyStates.insert(std::make_pair(key, action));
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
		SetEventCallbacks();

		// Initialize grahpics
		InitializeGraphics();
		// Initialize
		InitializeGUI();
	}

	void MasterWindow::Release()
	{
		UserInterfaceSystem.Release();
		Pipeline.Release();
		glfwDestroyWindow(MasterWindow::p_glfwWindow);
	}

	Vector2Int MasterWindow::GetScreenSize()
	{
		int width = 0, height = 0;
		glfwGetWindowSize(p_glfwWindow, &width, &height);
		return { width, height };
	}
	void MasterWindow::SetScreenSize(Vector2Int size)
	{
		glfwSetWindowSize(p_glfwWindow, size.X, size.Y);
	}

	int MasterWindow::GetKeyState(int glfwKey)
	{
		if (m_KeyStates.contains(glfwKey))
			return m_KeyStates.at(glfwKey);

		return GLFW_RELEASE;
	}
}