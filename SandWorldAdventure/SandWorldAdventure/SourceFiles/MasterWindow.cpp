//#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/MasterWindow.h"
#include <iostream>

//#include "KeyboardInterfaceAPI/Headers/KeyboardInterface/KeyboardState.h"

MasterWindow::MasterWindow() : p_glfwWindow(NULL)
{
	///*nothing*/
	//TODO: This may not run because it is in a constructor

	// - Set hints -
	glfwInitHint(GLFW_PLATFORM, GLFW_ANY_PLATFORM);

	// Version of GLFW is 3.4 and we want to use the matching client API version of the window's context. This will throw an error if the client does not have the correct version of opengl.
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	// TODO: What is this?
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!glfwInit())
		// Initialization failed
		exit(EXIT_FAILURE);

	std::cout << glfwGetVersionString();
	std::cout << "\n";

	// - Initialize -
	InitializeWindow();
	if (p_glfwWindow == nullptr)
		exit(EXIT_FAILURE);

	// Initialize glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	InitializeShaders();
}

void MasterWindow::InitializeWindow()
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

void MasterWindow::InitializeShaders()
{
//	// Create the pipline
//	g_Pipeline = GraphicsPipeline::GraphicsPipeline();
//
//	// Add shaders
//	GraphicsPipeline::Shaders::OpaqueShader* pOpaqueShader = new GraphicsPipeline::Shaders::OpaqueShader();
//	g_Pipeline.RegisterShader(pOpaqueShader);
//
//	// Compile
//	int shadersErrorCode = g_Pipeline.CompileShaders();
//	if (shadersErrorCode != GraphicsPipeline::IGraphicsPipeline::GP_SUCCESS)
//	{
//		fprintf(stderr, std::string("GraphicsPipline::CompileShaders failed!  Returned code: ").append(std::to_string(shadersErrorCode)).append("\n").c_str());
//#if _DEBUG
//		throw std::exception("Failed to compile shader!");
//#endif
//		//exit(EXIT_FAILURE);
//	}
//	shadersErrorCode = g_Pipeline.CreateProgram();
//	if (shadersErrorCode != GraphicsPipeline::IGraphicsPipeline::GP_SUCCESS)
//	{
//		fprintf(stderr, std::string("GraphicsPipline::CreateProgram() failed!  Returned code: ").append(std::to_string(shadersErrorCode)).append("\n").c_str());
//#if _DEBUG
//		throw std::exception("Failed to create program!");
//#endif
//		//exit(EXIT_FAILURE);
//	}
}