// Game
#include "SWA/Game.h"
#include "SWAEngine/Scene.h"
#include "SWAEngine/SceneManager.h"
#include "SWA/RenderLayerNames.h"
#include "SWA/SpriteShaderProperties.h"

#include <SWA/JSON/JSONManager.h>

#include "SWAEngine/Tilemap/TileBehavior/Types.h"

// Graphics
#include "GP2D/Pipeline/GenericPipeline.h"
#include "GP2D/Pipeline/Texture/BitmapTexture.h"

#include "GP2D/Pipeline/Shader/GeometryShader.h"
#include "GP2D/Pipeline/Mesh/Mesh.h"

#include <gl/glew.h>


using namespace GP2D;
using namespace GP2D::Pipeline;
// TODO: Go through all includes and use <> instead of "" where appropriate
namespace SWA
{
	const unsigned int Game::FPS = 60;

	Player::Player* Game::p_MainPlayer = nullptr;
	SWAEngine::Tilemap::Tilemap* Game::p_Tilemap = nullptr;
	SWAEngine::Tilemap::TilemapMesh* Game::p_TilemapMesh = nullptr;

	void Game::CreateRenderLayers()
	{
		// Create render layers
		GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Tilemap0, { "Tilemap0" });
		GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Objects, { "Objects" });
		GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Characters, { "Characters" });
		GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_GUI, { "GUI" });
		GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Debug, { "Debug" });
	}

	void Game::InitializeGame()
	{
		SWAEngine::SceneManager::CreateScene("ThisIsMyScene");
		SWAEngine::SceneManager::CreateScene("ThisIsAnotherScene");

		// TEst mesh
		auto pNewMesh = new Mesh::Mesh(true, SpriteShaderProperties::CreateProperties("lava_24"), true);
		pNewMesh->Origin = { -0.1f, -.5f };
		pNewMesh->Scale = { 0.25f, 0.25f };
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
			{0, 6, "DefaultSpriteShader"}
		};
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Objects).RegisterMesh(pNewMesh);

		// Create tilemap
		p_Tilemap = new SWAEngine::Tilemap::Tilemap("MainTilemap", { 0, 0 }, {0.01f,0.01f});
		p_TilemapMesh = new SWAEngine::Tilemap::TilemapMesh(p_Tilemap, "TilemapShader");
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Tilemap0).RegisterMesh(p_TilemapMesh);

		p_Tilemap->SetTile({ 2, 2 }, { SWAEngine::Tilemap::TileBehavior::SOLID, 0xff0000ff, true });
		p_Tilemap->SetTile({ 0, 0 }, { SWAEngine::Tilemap::TileBehavior::SAND, 0xff0000ff, true });
		p_Tilemap->SetTile({ 1, 2 }, { SWAEngine::Tilemap::TileBehavior::SOLID, 0xff0000ff, true });
		p_Tilemap->SetTile({ 0, 1 }, { SWAEngine::Tilemap::TileBehavior::SAND, 0xff0000ff, true });
		p_Tilemap->SetTile({ 3, 3 }, { SWAEngine::Tilemap::TileBehavior::SOLID, 0xffFFffFF, true });

		// Create the player
		p_MainPlayer = new Player::Player();
		p_MainPlayer->SetPosition({ 1, 2 });
	}


	void Game::CreateResources()
	{
		// Register textures
		JSON::JSONManager::LoadTextures("textures.json");


		// Register shaders
		JSON::JSONManager::LoadShaders("shaders.json");

		// Update callback
		GenericPipeline::s_Shaders.TryGetShader<Shader::GeometryShader>("DefaultSpriteShader")->UpdatePropertiesCallback =
			[](Shader::BaseShaderType* pShader, void* pProperties)
		{
			if (pProperties == nullptr)
			{
				pShader->PropertyManager.TrySetTexture("default_sprite", "Tex");
				return;
			}

			SpriteShaderProperties properties = *static_cast<SpriteShaderProperties*>(pProperties);

			// Set outline related
			glUniform2f(glGetUniformLocation(pShader->GetProgram(), "Outline_Thickness"), properties.OutlineThickness.X, properties.OutlineThickness.Y);
			glUniform4f(glGetUniformLocation(pShader->GetProgram(), "Outline_Color"), properties.OutlineColor.X, properties.OutlineColor.Y, properties.OutlineColor.Z, properties.OutlineColor.W);
			glUniform1f(glGetUniformLocation(pShader->GetProgram(), "Outline_Clip"), properties.OutlineClip);

			// Texture related
			const char* name = properties.TextureName != nullptr && GenericPipeline::s_Textures.ContainsTexture(properties.TextureName) ? (const char*)properties.TextureName : "default_sprite";
			pShader->PropertyManager.TrySetTexture(name, "Tex");
			glUniform4f(glGetUniformLocation(pShader->GetProgram(), "Tex_Color"), properties.TextureColor.X, properties.TextureColor.Y, properties.TextureColor.Z, properties.TextureColor.W);

			Math::Int2 resolution = GenericPipeline::s_Textures.GetTexture(name).second.Size;
			glUniform4f(glGetUniformLocation(pShader->GetProgram(), "Tex_Resolution"), resolution.X, resolution.Y, 0, 0);
		};
	}
	void Game::Initialize()
	{
		// Set up graphics
		CreateRenderLayers();

		// Set up game
		InitializeGame();
	}
	void Game::Update(SWAEngine::Time time)
	{
		auto camera = GP2D::Pipeline::GenericPipeline::s_ActiveCamera;
		glViewport(0, 0, camera.GetScreenSize().X, camera.GetScreenSize().Y);
		glClear(GL_COLOR_BUFFER_BIT);

		SWAEngine::SceneManager::GetScene().UpdateObjects(time);
	}
	void Game::Release()
	{
		SWAEngine::SceneManager::ReleaseScene();
	}
}