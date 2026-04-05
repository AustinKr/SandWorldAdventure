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

#include <thread>
#include <chrono>

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

// Run on thread separate from processing events
void InitializeGame()
{
	// Make context current
	glfwMakeContextCurrent(GP2D::Pipeline::Window::Window::sp_Window);
	
	glfwSwapInterval(1);

	// Create resources prior to the pipeline compiling any shaders
	SWA::Game::CreateResources();
	// Initialize pipeline
	GP2D::Pipeline::GenericPipeline::s_Instance.Initialize();
	// Create gui
	GP2D::GUI::Hierarchy::sp_ActiveInstance = new GP2D::GUI::Hierarchy();
	// Initalize game logic
	SWA::Game::Initialize();

	// A fixed duration with a length of 1/FPS seconds
	const double fixedDeltaTime = 1.0 / (double)SWA::Game::FPS;
	const auto fixedDuration = std::chrono::seconds((long)fixedDeltaTime);

	// Used to track time
	SWAEngine::Time frameTime = {};
	auto timePoint = std::chrono::steady_clock::now();

	while (!glfwWindowShouldClose(GP2D::Pipeline::Window::Window::sp_Window))
	{
		// Get time
		frameTime = { glfwGetTime(),
					glfwGetTime() - frameTime.CurrentTime,
					fixedDeltaTime};

		// Update
		SWA::Game::Update(frameTime);

		// Render
		GP2D::Pipeline::GenericPipeline::s_Instance.RenderScene();
		glfwSwapBuffers(GP2D::Pipeline::Window::Window::sp_Window);

		// Wait to keep consistent update cycles and avoid eating up CPU
		timePoint += fixedDuration;
		std::this_thread::sleep_until(timePoint + std::chrono::seconds((long)(fixedDeltaTime - .001)));
	}
	// TODO: Display FPS somewhere
}

int main(void)
{
	// Create window
	CreateGP2DWindow();

	// Create a thread separate from processing events
	glfwMakeContextCurrent(NULL);
	std::thread gameThread(InitializeGame);
	
	// Process events
	GP2D::Pipeline::Window::Window::ProcessEvents();

	// Wait to join game thread with main
	if (gameThread.joinable())
		gameThread.join();
	// Release and close app
	Release();
}