#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include "IShaderType.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/BaseShaderInformation.h" 
#include <string>

namespace SandboxEngine::GraphicsPipeline::ShaderTypes
{
	// The default shader used for most objects that provides fragment and vertex shader capabilities
	struct ShaderType : IShaderType
	{
	private:
		GLuint mp_VertexShader;
		GLuint mp_FragmentShader;

		std::string m_Name;
		std::string m_VertexShaderCode;
		std::string m_FragmentShaderCode;

		bool ParseCode(std::string code, std::string *pVertCode, std::string *pFragCode);
	public:
		Shaders::BaseShaderInformation* const p_ShaderInformation;

		/// <summary>
		/// The filepath relative to this project. e.g. "Shaders/DefaultShader.shader"
		/// </summary>
		/// <param name="filepath"></param>
		ShaderType(std::string name, std::string filepath, Shaders::BaseShaderInformation* pShaderInformation);

		virtual int Compile(IGraphicsPipeline* pPipeline) override;
		
		virtual void Release() override;

		virtual GLuint GetProgram() override;
		virtual std::string GetName() override;
	};
}