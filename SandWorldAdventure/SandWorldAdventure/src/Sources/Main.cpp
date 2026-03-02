#include "RenderLayerNames.h"
#include "ShaderNames.h"

#include "GP2D/Pipeline/Window/Window.h"
#include "GP2D/Pipeline/GenericPipeline.h"
#include "GP2D/GUI/Hierarchy.h"

#include "GP2D/Pipeline/Shader/GeometryShader.h"
#include "GP2D/Pipeline/Mesh/Mesh.h"

#define GLFW_INCLUDE_NONE
#include <gl/glew.h>
#include <GLFW/glfw3.h>

using namespace GP2D;
using namespace GP2D::Pipeline;

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

void AddTextures()
{
	// TODO: could define all textures in a .json file and load it
	GenericPipeline::s_Textures.RegisterTexture("default_sprite", Texture::LoadBMPTextureFromFile(_SWA_RESOURCES_DIR"Textures/default_sprite.bmp"));
	GenericPipeline::s_Textures.RegisterTexture("lava_24", Texture::LoadBMPTextureFromFile(_SWA_RESOURCES_DIR"Textures/GUI/Lava24.bmp"));
}

void AddShaders()
{
	auto pNewShader = new Shader::GeometryShader("DefaultSpriteShader", _SWA_RESOURCES_DIR"Shaders/TextureShader.shader");
	pNewShader->UpdatePropertiesCallback = [](Shader::BaseShaderType* pShader, void* texName)
	{
		pShader->PropertyManager.TrySetTexture(texName != nullptr ? (const char*)texName : "default_sprite", "Tex");
		
		pShader->PropertyManager.TrySetUniform("ShadeColor",
			[](int loc)
		{
			glUniform4f(loc, 1, 1, 1, 1);
		});
	};
	GenericPipeline::s_Shaders.RegisterShader(pNewShader); // SWA_TEXTURE_SHADER
}

void InitializeGraphics()
{
	GenericPipeline::s_Instance.Initialize();

	// Create render layers
	GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Tilemap0, { "Tilemap0" });
	GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Objects, { "Objects" });
	GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Characters, { "Characters" });
	GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_GUI, { "GUI" });
	GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Debug, {"Debug"});
}

void InitializeGame()
{
	GUI::Hierarchy::sp_ActiveInstance = new GUI::Hierarchy();

	// TEst mesh
	auto pNewMesh = new Mesh::Mesh(true, (void*)"lava_24");
	pNewMesh->Origin = { -0.1f, -.2f };
	pNewMesh->Scale = { 0.5f, 0.5f };
	pNewMesh->Vertices = {
		{{0, 0}, {0, 0}},
		{{1, 0}, {1, 0}},
		{{1, 1}, {1, 1}},
		{{0, 1}, {0, 1}} };
	pNewMesh->Triangles = {
		0, 1, 2,
		0, 2, 3
	};
	pNewMesh->Shaders = {
		0, 6, SWA_TEXTURE_SHADER
	};
	GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Objects).RegisterMesh(pNewMesh);
}

int main(void)
{
	CreateGP2DWindow();

	// Register textures
	AddTextures();
	// Register shaders
	AddShaders();

	// Set up graphics
	InitializeGraphics();

	// Set up game
	InitializeGame();

	// Game loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(Window::Window::sp_Window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		GenericPipeline::s_Instance.RenderScene();
		Window::Window::ProcessEvents();
	}

	// Release and close app
	GUI::Hierarchy::sp_ActiveInstance->Release();
	GenericPipeline::s_Instance.Release();
	Window::Window::Destroy();
	Window::Window::TerminateGLFW();
}