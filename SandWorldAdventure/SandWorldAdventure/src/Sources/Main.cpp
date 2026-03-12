/*
NOTE: can only run on Release

*/

#include "SWA/Game.h"
#include "SWAEngine/Time.h"

#include "GP2D/Pipeline/Window/Window.h"
#include "GP2D/Pipeline/GenericPipeline.h"
#include "GP2D/GUI/Hierarchy.h"

#define GLFW_INCLUDE_NONE
#include <gl/glew.h>
#include <GLFW/glfw3.h>

void CreateGP2DWindow()
{
	GP2D::Pipeline::Window::Window::s_Properties.DefaultWidth = 526;
	GP2D::Pipeline::Window::Window::s_Properties.DefaultHeight = 526;
	GP2D::Pipeline::Window::Window::s_Properties.ShouldStartFullScreen = false;
	GP2D::Pipeline::Window::Window::s_Properties.Title = "Sand World Adventure";
	GP2D::Pipeline::Window::Window::s_Properties.BackgroundColor = { .5,.5,1,1 };
	
	if (GP2D::Pipeline::Window::Window::Initialize() == nullptr)
	{
		fprintf(stderr, "Fatal error: failed to initialize window!\n");
		throw std::exception();
	}
}

void Release()
{
	SWA::Game::Release();

	GP2D::GUI::Hierarchy::sp_ActiveInstance->Release();
	GP2D::Pipeline::GenericPipeline::s_Instance.Release(); // Releases all registered data
	GP2D::Pipeline::Window::Window::Destroy();
	GP2D::Pipeline::Window::Window::TerminateGLFW();
}

int main(void)
{
	// Create window
	CreateGP2DWindow();
	// Create resources prior to the pipeline compiling any shaders
	SWA::Game::CreateResources();
	// Initialize pipeline
	GP2D::Pipeline::GenericPipeline::s_Instance.Initialize();
	// Create gui
	GP2D::GUI::Hierarchy::sp_ActiveInstance = new GP2D::GUI::Hierarchy();
	// Initalize game
	SWA::Game::Initialize();

	glfwSwapInterval(1);
	SWAEngine::Time time = {0,0,0};

	// Game loop
	while (!glfwWindowShouldClose(GP2D::Pipeline::Window::Window::sp_Window))
	{
		time.CurrentTime = glfwGetTime();
		time.FrameDeltaTime = time.CurrentTime - time.LastTime;
		time.LastTime = time.CurrentTime;

		glClear(GL_COLOR_BUFFER_BIT);
		
		SWA::Game::Update(time);
		GP2D::Pipeline::GenericPipeline::s_Instance.RenderScene();
		GP2D::Pipeline::Window::Window::ProcessEvents();
	}

	// Release and close app
	Release();
}