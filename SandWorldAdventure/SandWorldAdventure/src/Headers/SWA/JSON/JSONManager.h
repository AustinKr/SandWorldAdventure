#pragma once
#include <nlohmann/json.hpp>
#include <SWAEngine/FileManager.h>
#include <SWA/JSON/Shader.h>
#include <GP2D/Pipeline/GenericPipeline.h>
#include <GP2D/Pipeline/Texture/BitmapTexture.h>
#include <GP2D/Pipeline/Shader/GeometryShader.h>

#include <string>
#include <stdexcept>

namespace SWA::JSON
{
	// Manages reading/writing JSON files for the game
	struct JSONManager 
	{
	private:
		static std::string GetResourceFullPath(std::string localPath)
		{
			std::string fullPath = std::string(_SWA_RESOURCES_DIR) + localPath;
			if (!std::filesystem::exists(fullPath))
				fullPath = std::string(_SWA_DEFAULT_RESOURCES_DIR) + localPath;
			return fullPath;
		}

	public:
		// Loads the json file
		static nlohmann::json LoadFromFile(const char* filePath)
		{
			// Read file
			char* code; size_t size;
			if (SWAEngine::FileManager::FILE_MANAGER_FAILED & 
				SWAEngine::FileManager::ReadFile(std::string(_SWA_RESOURCES_DIR).append(filePath).c_str(), &code, &size))
				throw std::runtime_error("Failed to read file!");

			nlohmann::json file = nlohmann::json::parse(code);
			return file;
		}

		static void LoadTextures(const char* filePath)
		{
			using GP2D::Pipeline::GenericPipeline;
			using GP2D::Pipeline::Texture::BitmapTexture;

			// Register textures
			for (nlohmann::json& element : LoadFromFile(filePath))
			{
				if (!element.is_object())
					continue;
				// Convert
				Resource tex = element;

				// Try get bitmap
				BitmapTexture bitmap = BitmapTexture::LoadBMPTextureFromFile(GetResourceFullPath(tex.path).c_str());
				if (bitmap.ColorsSize < 2)
				{
					fprintf(stdout, (std::string("failed to locate texture: ") + tex.path + "\n").c_str());
					continue;
				}

				// Register texture
				GenericPipeline::s_Textures.RegisterTexture(tex.alias.c_str(), std::move(bitmap));
			}
		}
		static void LoadShaders(const char* filePath)
		{
			using GP2D::Pipeline::GenericPipeline;
			using GP2D::Pipeline::Texture::BitmapTexture;
			using GP2D::Pipeline::Shader::BaseShaderType;
			using GP2D::Pipeline::Shader::GeometryShader;

			// Register textures
			for (nlohmann::json& element : LoadFromFile(filePath))
			{
				if (!element.is_object())
					continue;
				// Convert
				Shader shader = element;

				// Register
				BaseShaderType* pShader;
				switch (shader.type)
				{
					case Shader::SHADER_TYPE_GEOMETRY:
						pShader = new GeometryShader(shader.resource.alias, GetResourceFullPath(shader.resource.path).c_str());
						break;
					default:
						pShader = nullptr;
				}
				GenericPipeline::s_Shaders.RegisterShader(pShader);
			}
		}
	};
}