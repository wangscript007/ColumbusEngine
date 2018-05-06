#pragma once

#include <Graphics/Shader.h>
#include <Core/Types.h>
#include <map>

namespace Columbus
{

	class ShaderStageOpenGL : public ShaderStage
	{
	private:
		uint32 ID = 0;
	public:
		ShaderStageOpenGL();

		bool IsValid() const override;
		bool Load(std::string InPath, ShaderType InType) override;
		bool Compile() override;

		//Get OpenGL-sepcifed identifier
		uint32 GetID() const;

		~ShaderStageOpenGL() override;
	};

	class ShaderProgramOpenGL : public ShaderProgram
	{
	private:
		std::map<std::string, uint32> UniformLocations;
		uint32 ID = 0;
	public:
		ShaderProgramOpenGL();

		void Bind() const override;
		void Unbind() const override;

		void AddStage(ShaderStage* Stage) override;
		bool Compile() override;

		void AddUniform(std::string Name) override;
		void SetUniform1i(std::string Name, int Value) const override;
		void SetUniform1f(std::string Name, float Value) const override;
		void SetUniform2f(std::string Name, Vector2 Value) const override;
		void SetUniform3f(std::string Name, Vector3 Value) const override;
		void SetUniform4f(std::string Name, Vector4 Value) const override;
		void SetUniformMatrix(std::string Name, const float* Value) const override;
		void SetUniformArrayf(std::string Name, const float* Array, uint32 Size) const override;

		~ShaderProgramOpenGL() override;
	};

}



