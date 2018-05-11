#include <Graphics/OpenGL/ShaderOpenGL.h>
#include <RenderAPIOpenGL/OpenGL.h>
#include <fstream>
#include <algorithm>

namespace Columbus
{
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//STANDART SKYBOX VERTEX SHADER
	const std::string gSkyVertexShader =
		"#version 130\n"
		"in vec3 aPos;\n"
		"in vec2 aUV;\n"
		"in vec3 aNorm;\n"
		"out vec3 texCoord;\n"
		"uniform mat4 uView;\n"
		"uniform mat4 uProjection;\n"
		"void main()\n"
		"{\n"
		"gl_Position = uProjection * uView * vec4(aPos, 1.0);\n"
		"texCoord = aPos;\n"
		"}\n";
	//////////////////////////////////////////////////////////////////////////////
	//STANDART SKYBOX FRAGMENT SHADER
	const std::string gSkyFragmentShader =
		"#version 130\n"
		"out vec4 FragColor;\n"
		"in vec3 texCoord;\n"
		"uniform samplerCube uSkybox;\n"
		"void main()\n"
		"{\n"
		"FragColor = textureCube(uSkybox, texCoord);\n"
		"}\n";
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	/*
	*
	* Shader loading functions
	*
	*/
	bool ShaderLoadFromFile(std::string InFile, std::string& OutSource, ShaderBuilder& Builder, ShaderType Type)
	{
		std::ifstream File;
		File.open(InFile.c_str());

		if (!File.is_open())
		{
			Log::error("Shader not loaded: " + InFile);
			return false;
		}

		std::string TmpFile = std::string((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());

		if (!Builder.Build(TmpFile, Type))
		{
			Log::error("Shader not built: " + InFile);
			return false;
		}

		if (Builder.ShaderSource.empty())
		{
			Log::error("Shader loading incorrect: " + InFile);
			return false;
		}

		OutSource = Builder.ShaderSource;

		return true;
	}

	bool ShaderLoad(std::string InPath, std::string& OutSource, ShaderBuilder& Builder, ShaderType Type)
	{
		switch (Type)
		{
			case ShaderType::Vertex:
			{
				if (InPath == "STANDART_SKY_VERTEX")
				{
					OutSource = gSkyVertexShader;
					return true;
				}
				else if (ShaderLoadFromFile(InPath, OutSource, Builder, Type))
				{
					return true;
				}

				break;
			}

			case ShaderType::Fragment:
			{
				if (InPath == "STANDART_SKY_FRAGMENT")
				{
					OutSource = gSkyFragmentShader;
					return true;
				}
				else if (ShaderLoadFromFile(InPath, OutSource, Builder, Type))
				{
					return true;
				}

				break;
			}
		}

		return false;
	}
	/*
	*
	* End of shader loading functions
	*
	*/

	/*
	*
	* Shader compilation functions
	*
	*/
	bool ShaderGetError(std::string ShaderPath, int32 ShaderID)
	{
		int32 Status = GL_TRUE;
		int32 Length = 0;
		char* Error = nullptr;

		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Status);

		if (Status == GL_FALSE)
		{
			glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &Length);
			Error = new char[Length];
			glGetShaderInfoLog(ShaderID, Length, &Length, Error);
			Log::error(ShaderPath + ": " + Error);

			delete[] Error;
			return true;
		}

		return false;
	}

	bool ShaderCompile(std::string ShaderPath, std::string ShaderSource, int32 ShaderID)
	{
		auto Source = ShaderSource.c_str();
		glShaderSource(ShaderID, 1, &Source, NULL);
		glCompileShader(ShaderID);

		if (ShaderGetError(ShaderPath, ShaderID))
		{
			return false;
		}

		return true;
	}

	void ShaderCleanup(int32 VertexID, int32 FragmentID)
	{
		glDeleteShader(VertexID);
		glDeleteShader(FragmentID);
	}
	/*
	*
	* End of shader compilation functions
	*
	*/
	
	/*
	*
	* ShaderStageOpenGL implementation
	*
	*/
	ShaderStageOpenGL::ShaderStageOpenGL()
	{
		
	}

	bool ShaderStageOpenGL::IsValid() const
	{
		return ID != 0 && glIsShader(ID);
	}

	bool ShaderStageOpenGL::Load(std::string InPath, ShaderType InType)
	{
		uint32 tType;
		
		switch (InType)
		{
		case ShaderType::Vertex:
			tType = GL_VERTEX_SHADER;
			break;
		case ShaderType::Fragment:
			tType = GL_FRAGMENT_SHADER;
			break;
		default:
			return false;
			break;
		}
		
		ID = glCreateShader(tType);

		if (ID == 0) return false;

		if (!ShaderLoad(InPath, ShaderSource, Builder, InType))
		{
			return false;
		}

		ShaderPath = InPath;
		Type = InType;
		Loaded = true;
		Compiled = false;
		Log::success("Shader loaded: " + InPath);

		return true;
	}

	bool ShaderStageOpenGL::Compile()
	{
		if (!Loaded)
		{
			Log::error("Couldn't compile shader: Shader wasn't loaded");
			return false;
		}

		if (!IsValid())
		{
			Log::error("Couldn't compile shader: Shader is invalid");
			return false;
		}

		if (!ShaderCompile(ShaderPath, ShaderSource, ID)) { glDeleteShader(ID); return false; }

		Compiled = true;
		Log::success("Shader compiled: " + ShaderPath);

		return true;
	}

	uint32 ShaderStageOpenGL::GetID() const
	{
		return ID;
	}

	ShaderStageOpenGL::~ShaderStageOpenGL()
	{
		if (ID != 0)
		{
			glDeleteShader(ID);
		}
	}
	/*
	*
	* ShaderProgramOpenGL implementation
	*
	*/
	ShaderProgramOpenGL::ShaderProgramOpenGL()
	{
		ID = glCreateProgram();
	}

	void ShaderProgramOpenGL::Bind() const
	{
		glUseProgram(ID);
	}

	void ShaderProgramOpenGL::Unbind() const
	{
		glUseProgram(0);
	}

	void ShaderProgramOpenGL::AddStage(ShaderStage* Stage)
	{
		if (Stage != nullptr)
		{
			Stages.push_back(Stage);
		}
	}

	bool ShaderProgramOpenGL::Compile()
	{
		if (std::find_if(Stages.begin(), Stages.end(), [](ShaderStage* InStage)->bool { return InStage->GetType() == ShaderType::Vertex; }) == Stages.end() &&
			std::find_if(Stages.begin(), Stages.end(), [](ShaderStage* InStage)->bool { return InStage->GetType() == ShaderType::Fragment; }) == Stages.end())
		{
			Log::error("Coldn't compile Shader Program: Needs vertex and fragment shader");
			return false;
		}

		for (auto Stage : Stages)
		{
			if (!Stage->IsCompiled())
			{
				if (!Stage->Compile())
				{
					Log::error("Couldn't compile Shader Program: One or more of the shader not compiled");
					return false;
				}
			}

			if (!Stage->IsValid())
			{
				Log::error("Couldn't compile Shader Program: One or more of the shaders is invalid");
				return false;
			}

			glAttachShader(ID, static_cast<ShaderStageOpenGL*>(Stage)->GetID());
		}

		for (auto Attrib : Attributes)
		{
			glBindAttribLocation(ID, Attrib.Value, Attrib.Name.c_str());
		}

		glLinkProgram(ID);

		Compiled = true;
		Log::success("Shader program compiled");

		return true;
	}

	void ShaderProgramOpenGL::AddUniform(std::string Name)
	{
		UniformLocations[Name] = glGetUniformLocation(ID, Name.c_str());
	}

	void ShaderProgramOpenGL::SetUniform1i(std::string Name, int Value) const
	{
		if (ID != 0 && Compiled)
		{
			auto Location = UniformLocations.find(Name);
			if (Location != UniformLocations.end())
			{
				glUniform1i(UniformLocations.at(Name), Value);
			}
		}
	}

	void ShaderProgramOpenGL::SetUniform1f(std::string Name, float Value) const
	{
		if (ID != 0 && Compiled)
		{
			auto Location = UniformLocations.find(Name);
			if (Location != UniformLocations.end())
			{
				glUniform1f(UniformLocations.at(Name), Value);
			}
		}
	}

	void ShaderProgramOpenGL::SetUniform2f(std::string Name, Vector2 Value) const
	{
		if (ID != 0 && Compiled)
		{
			auto Location = UniformLocations.find(Name);
			if (Location != UniformLocations.end())
			{
				glUniform2f(UniformLocations.at(Name), Value.X, Value.Y);
			}
		}
	}

	void ShaderProgramOpenGL::SetUniform3f(std::string Name, Vector3 Value) const
	{
		if (ID != 0 && Compiled)
		{
			auto Location = UniformLocations.find(Name);

			if (Location != UniformLocations.end())
			{
				glUniform3f(UniformLocations.at(Name), Value.X, Value.Y, Value.Z);
			}
		}
	}

	void ShaderProgramOpenGL::SetUniform4f(std::string Name, Vector4 Value) const
	{
		if (ID != 0 && Compiled)
		{
			auto Location = UniformLocations.find(Name);
			if (Location != UniformLocations.end())
			{
				glUniform4f(UniformLocations.at(Name), Value.x, Value.y, Value.z, Value.w);
			}
		}
	}

	void ShaderProgramOpenGL::SetUniformMatrix(std::string Name, const float* Value) const
	{
		if (ID != 0 && Compiled)
		{
			auto Location = UniformLocations.find(Name);
			if (Location != UniformLocations.end())
			{
				glUniformMatrix4fv(UniformLocations.at(Name), 1, GL_FALSE, Value);
			}
		}
	}

	void ShaderProgramOpenGL::SetUniformArrayf(std::string Name, const float* Array, uint32 Size) const
	{
		if (ID != 0 && Compiled)
		{
			auto Location = UniformLocations.find(Name);
			if (Location != UniformLocations.end())
			{
				glUniform1fv(UniformLocations.at(Name), Size, Array);
			}
		}
	}

	ShaderProgramOpenGL::~ShaderProgramOpenGL()
	{
		glDeleteProgram(ID);
	}

}








