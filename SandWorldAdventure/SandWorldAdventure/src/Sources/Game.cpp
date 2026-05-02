// Game
#include "SWA/Game.h"
#include "SWAEngine/Scene.h"
#include "SWAEngine/SceneManager.h"
#include "SWA/RenderLayerNames.h"
#include "SWA/SpriteShaderProperties.h"

#include <SWA/JSON/JSONManager.h>

#include <SWAEngine/Tilemap/TileBehavior/Types.h>
#include <SWAEngine/Component/Physics/BoxCollider.h>

// Graphics
#include "GP2D/Pipeline/GenericPipeline.h"
#include "GP2D/Pipeline/Texture/BitmapTexture.h"

#include "GP2D/Pipeline/Shader/GeometryShader.h"
#include "GP2D/Pipeline/Mesh/Mesh.h"

#include <gl/glew.h>


using namespace GP2D;
using namespace GP2D::Pipeline;

using namespace SWAEngine;
using namespace Component;
using namespace Physics;


// TODO: Go through all includes and use <> instead of "" where appropriate
namespace SWA
{
	const unsigned int Game::FPS = 60;

	Player::Player* Game::p_MainPlayer = nullptr;
	TilemapComponent* Game::p_Tilemap = nullptr;
	TilemapCollider* Game::p_TilemapCollider = nullptr;
	Tilemap::TilemapMesh* Game::p_TilemapMesh = nullptr;

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
		GenericPipeline::s_ActiveCamera.ScaleFactor = 1.5;

		SWAEngine::SceneManager::CreateScene("ThisIsMyScene");
		SWAEngine::SceneManager::CreateScene("ThisIsAnotherScene");

		// Create tilemap
		GameObject& tilemapObj = SceneManager::GetScene().CreateGameObject("Tilemap");
		p_TilemapCollider = tilemapObj.GetComponent<Component::Physics::TilemapCollider>();
		p_Tilemap = p_TilemapCollider->p_LinkedTilemap;
		p_Tilemap->p_LinkedTransform->SetPosition({0, 0});
		p_Tilemap->p_LinkedTransform->SetScale({.01f, .01f});
		// Create mesh
		p_TilemapMesh = new Tilemap::TilemapMesh(p_Tilemap, "TilemapShader");
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Tilemap0).RegisterMesh(p_TilemapMesh);

		// Set some tiles
		p_Tilemap->SetTile({ 2, 2 }, { Tilemap::TileBehavior::SOLID, 0xff0000ff, true });
		p_Tilemap->SetTile({ 0, 0 }, { Tilemap::TileBehavior::SAND, 0xff0000ff, true });
		p_Tilemap->SetTile({ 1, 2 }, { Tilemap::TileBehavior::SOLID, 0xff0000ff, true });
		p_Tilemap->SetTile({ 0, 1 }, { Tilemap::TileBehavior::SAND, 0xff0000ff, true });
		p_Tilemap->SetTile({ 3, 3 }, { Tilemap::TileBehavior::SOLID, 0xffFFffFF, true });

		// Create the player
		GameObject& playerObj = SWAEngine::SceneManager::GetScene().CreateGameObject("Player");
		p_MainPlayer = playerObj.GetComponent<Player::Player>();
		p_MainPlayer->p_LinkedTransform->SetPosition({ 3, 2 });


		// TODO: remove test collision object
		GameObject& testObj = SWAEngine::SceneManager::GetScene().CreateGameObject("MyTestObj");
		BoxCollider* testCollider = testObj.GetComponent<BoxCollider>();
		// Test mesh
		auto testMesh = new Mesh::Mesh(true, SpriteShaderProperties::CreateProperties("lava_24"), true);
		testMesh->Vertices = {
			{{0, 0}, {0, 0}},
			{{1, 0}, {1, 0}},
			{{1, 1}, {1, 1}},
			{{0, 1}, {0, 1}} };
		testMesh->Triangles = {
			0, 1, 2,
			0, 2, 3
		};
		testMesh->Shaders = {
			{0, 6, "DefaultSpriteShader"}
		};
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Objects).RegisterMesh(testMesh);
		// Make mesh follow object
		testCollider->p_LinkedTransform->OnSetPosition += [&](SWAEngine::Math::Vector2 newPosition) { testMesh->Origin = { (float)newPosition.X, (float)newPosition.Y }; };
		testCollider->p_LinkedTransform->OnSetScale += [&](SWAEngine::Math::Vector2 newScale) { testMesh->Scale = { (float)newScale.X, (float)newScale.Y }; };
		// Set coordinates
		testCollider->p_LinkedTransform->SetPosition({ 1, 1 });
		testCollider->p_LinkedTransform->SetScale({ .5, 1 });
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

			GP2D::Math::Int2 resolution = GenericPipeline::s_Textures.GetTexture(name).second.Size;
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
	void Game::Update(SWAEngine::Math::Time time)
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