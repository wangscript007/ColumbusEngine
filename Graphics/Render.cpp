#include <Graphics/Render.h>
#include <Graphics/Device.h>
#include <Graphics/RenderState.h>
#include <Scene/ComponentMeshRenderer.h>
#include <Scene/ComponentParticleSystem.h>
#include <Scene/Component.h>
#include <Math/Frustum.h>
#include <GL/glew.h>
#include <algorithm>

#include <Profiling/Profiling.h>

#include <Graphics/OpenGL/ShaderOpenGL.h>

namespace Columbus
{
	Texture* Blob;
	Texture* EditorIconSun;
	Texture* EditorIconLamp;
	Texture* EditorIconFlashlight;
	Texture* EditorIconAudio;
	Texture* EditorIconParticles;

	ShaderProgram* NoneShader;
	ShaderProgram* BloomBrightShader;
	ShaderProgram* GaussBlurShader;
	ShaderProgram* BloomShader;
	ShaderProgram* LensFlareShader;

	RenderState State;

	Renderer::Renderer()
	{
		Blob = gDevice->CreateTexture();
		EditorIconSun = gDevice->CreateTexture();
		EditorIconLamp = gDevice->CreateTexture();
		EditorIconAudio = gDevice->CreateTexture();
		EditorIconFlashlight = gDevice->CreateTexture();
		EditorIconParticles = gDevice->CreateTexture();

		Blob->Load("Data/Textures/blob.png");
		EditorIconSun->Load("Data/Icons/Sun.png");
		EditorIconLamp->Load("Data/Icons/Lamp.png");
		EditorIconFlashlight->Load("Data/Icons/Flashlight.png");
		EditorIconAudio->Load("Data/Icons/Audio.png");
		EditorIconParticles->Load("Data/Icons/Particles.png");

		BaseEffect.ColorTexturesEnablement[0] = true;
		BaseEffect.ColorTexturesEnablement[1] = true;
		BaseEffect.DepthTextureEnablement = true;
		BaseEffect.ColorTexturesMipmaps[0] = false;

		BaseEffect.ColorTexturesFormats[0] = TextureFormat::RGB16F;

		BloomBrightPass.ColorTexturesEnablement[0] = true;
		BloomBrightPass.ColorTexturesMipmaps[0] = false;

		BloomHorizontalBlurPass.ColorTexturesEnablement[0] = true;
		BloomVerticalBlurPass.ColorTexturesEnablement[0] = true;

		BloomHorizontalBlurPass.ColorTexturesMipmaps[0] = false;
		BloomVerticalBlurPass.ColorTexturesMipmaps[0] = false;

		BloomFinalPass.ColorTexturesEnablement[0] = true;

		FinalPass.ColorTexturesEnablement[0] = true;

		NoneShader = gDevice->CreateShaderProgram();
		NoneShader->Load("Data/Shaders/PostProcessing.glsl");
		NoneShader->Compile();

		BloomBrightShader = gDevice->CreateShaderProgram();
		BloomBrightShader->Load("Data/Shaders/Bright.glsl");
		BloomBrightShader->Compile();

		GaussBlurShader = gDevice->CreateShaderProgram();
		GaussBlurShader->Load("Data/Shaders/GaussBlur.glsl");
		GaussBlurShader->Compile();

		BloomShader = gDevice->CreateShaderProgram();
		BloomShader->Load("Data/Shaders/Bloom.glsl");
		BloomShader->Compile();

		LensFlareShader = gDevice->CreateShaderProgram();
		LensFlareShader->Load("Data/Shaders/LensFlare.glsl");
		LensFlareShader->Compile();
	}

	void Renderer::SetViewport(const iVector2& Origin, const iVector2& Size)
	{
		ViewportOrigin = Origin;
		ViewportSize = Size;
	}

	void Renderer::SetMainCamera(const Camera& InCamera)
	{
		MainCamera = InCamera;
		State.SetMainCamera(MainCamera);
	}

	void Renderer::SetSky(Skybox* InSky)
	{
		Sky = InSky;
	}

	void Renderer::SetScene(Scene* InScn)
	{
		Scn = InScn;
	}

	uint32 Renderer::GetPolygonsRendered() const
	{
		return PolygonsRendered;
	}

	uint32 Renderer::GetOpaqueObjectsRendered() const
	{
		return OpaqueObjectsRendered;
	}

	uint32 Renderer::GetTransparentObjectsRendered() const
	{
		return TransparentObjectsRendered;
	}

	void Renderer::SetRenderList(std::map<uint32, SmartPointer<GameObject>>* List)
	{
		RenderList = List;
	}

	void Renderer::SetLightsList(std::vector<Light*>* List)
	{
		LightsList = List;
	}

	void Renderer::CompileLists()
	{
		PROFILE_CPU(ProfileModule::Culling);

		OpaqueObjects.clear();
		TransparentObjects.clear();

		Frustum ViewFrustum(MainCamera.GetViewProjection());

		if (RenderList != nullptr)
		{
			for (auto& Object : *RenderList)
			{
				auto MeshRenderer = Object.second->GetComponent<ComponentMeshRenderer>();
				auto ParticleSystem = Object.second->GetComponent<ComponentParticleSystem>();

				if (MeshRenderer != nullptr)
				{
					auto Mesh = MeshRenderer->GetMesh();

					if (Mesh != nullptr)
					{
						if (ViewFrustum.Check(Mesh->GetBoundingBox() * Object.second->GetTransform().GetMatrix()))
						{
							Object.second->GetMaterial().ReflectionMap = Sky->GetIrradianceMap();

							if (Object.second->GetMaterial().Transparent)
							{
								TransparentObjects.emplace_back(Mesh, nullptr, Object.second->GetTransform(), Object.second->GetMaterial());
							}
							else
							{
								OpaqueObjects.emplace_back(Mesh, Object.second->GetTransform(), Object.second->GetMaterial());
							}
						}
					}
				}

				if (ParticleSystem != nullptr)
				{
					auto Emitter = &ParticleSystem->Emitter;

					if (Emitter != nullptr)
					{
						TransparentObjects.emplace_back(nullptr, Emitter, Object.second->GetTransform(), Object.second->GetMaterial());
					}
				}
			}
		}
	}

	void Renderer::SortLists()
	{
		auto OpaqueSorter = [&](const OpaqueRenderData& A, const OpaqueRenderData& B)->bool
		{
			return MainCamera.Pos.LengthSquare(A.ObjectTransform.Position) < MainCamera.Pos.LengthSquare(B.ObjectTransform.Position);
		};

		auto TransparentSorter = [&](const TransparentRenderData& A, const TransparentRenderData& B)->bool
		{
			return MainCamera.Pos.LengthSquare(A.ObjectTransform.Position) > MainCamera.Pos.LengthSquare(B.ObjectTransform.Position);
		};

		std::sort(OpaqueObjects.begin(), OpaqueObjects.end(), OpaqueSorter);
		std::sort(TransparentObjects.begin(), TransparentObjects.end(), TransparentSorter);
	}

	void Renderer::RenderOpaqueStage()
	{
		PROFILE_GPU(ProfileModuleGPU::OpaqueStage);

		State.Clear();

		State.SetBlending(false);

		for (auto& Object : OpaqueObjects)
		{
			ShaderProgram* CurrentShader = Object.ObjectMaterial.GetShader();
				
			if (CurrentShader != nullptr)
			{
				State.SetCulling(Object.ObjectMaterial.Culling);
				State.SetDepthTesting(Object.ObjectMaterial.DepthTesting);
				State.SetDepthWriting(Object.ObjectMaterial.DepthWriting);
				State.SetShaderProgram(Object.ObjectMaterial.GetShader());
				State.SetMaterial(Object.ObjectMaterial, Object.ObjectTransform.GetMatrix(), Sky);
				State.SetLights(Object.Object->Lights);
				State.SetMesh(Object.Object);

				PolygonsRendered += Object.Object->Render();
				OpaqueObjectsRendered++;
			}
		}
	}

	void Renderer::RenderSkyStage()
	{
		PROFILE_GPU(ProfileModuleGPU::SkyStage);

		if (Sky != nullptr)
		{
			Sky->Render();
		}
	}

	void Renderer::RenderTransparentStage()
	{
		PROFILE_GPU(ProfileModuleGPU::TransparentStage);

		if (RenderList != nullptr && TransparentObjects.size() != 0)
		{
			State.Clear();
			State.SetBlending(true);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			for (auto& Object : TransparentObjects)
			{
				if (Object.MeshObject != nullptr)
				{
					ShaderProgramOpenGL* CurrentShader = (ShaderProgramOpenGL*)Object.ObjectMaterial.GetShader();
					Mesh* CurrentMesh = Object.MeshObject;

					if (CurrentShader != nullptr)
					{
						State.SetDepthTesting(Object.ObjectMaterial.DepthTesting);
						State.SetShaderProgram(Object.ObjectMaterial.GetShader());
						State.SetMaterial(Object.ObjectMaterial, Object.ObjectTransform.GetMatrix(), Sky);
						State.SetLights(Object.MeshObject->Lights);
						CurrentMesh->Bind();

						int32 Transparent = CurrentShader->GetFastUniform("Transparent");

						if (Object.ObjectMaterial.Culling == Material::Cull::No)
						{
							State.SetDepthWriting(true);
							CurrentShader->SetUniform(Transparent, 0);
							State.SetCulling(Material::Cull::No);
							CurrentMesh->Render();

							State.SetDepthWriting(false);
							CurrentShader->SetUniform(Transparent, 1);
							State.SetCulling(Material::Cull::Front);
							CurrentMesh->Render();
							State.SetCulling(Material::Cull::Back);
							PolygonsRendered += CurrentMesh->Render();
						}
						else
						{
							State.SetCulling(Object.ObjectMaterial.Culling);

							State.SetDepthWriting(true);
							CurrentShader->SetUniform(Transparent, 0);
							CurrentMesh->Render();

							State.SetDepthWriting(false);
							CurrentShader->SetUniform(Transparent, 1);
							PolygonsRendered += CurrentMesh->Render();
						}

						TransparentObjectsRendered++;

						CurrentMesh->Unbind();
					}
				}

				if (Object.Particles != nullptr)
				{
					State.SetShaderProgram(Object.ObjectMaterial.GetShader());
					ParticlesRender.Render(*Object.Particles, MainCamera, Object.ObjectMaterial);
				}
			}
		}

		State.SetDepthWriting(true);
	}

	void Renderer::Render(Renderer::Stage RenderStage)
	{
		if (RenderList != nullptr)
		{
			switch (RenderStage)
			{
			case Renderer::Stage::Opaque:      RenderOpaqueStage();      break;
			case Renderer::Stage::Sky:         RenderSkyStage();         break;
			case Renderer::Stage::Transparent: RenderTransparentStage(); break;
			}
		}

		State.SetCulling(Material::Cull::No);
	}

	void Renderer::RenderBloom()
	{
		PROFILE_GPU(ProfileModuleGPU::BloomStage);

		static int BrightShaderTexture = ((ShaderProgramOpenGL*)BloomBrightShader)->GetFastUniform("BaseTexture");
		static int BrightShaderTreshold = ((ShaderProgramOpenGL*)BloomBrightShader)->GetFastUniform("Treshold");

		static int BloomBlurTexture = ((ShaderProgramOpenGL*)GaussBlurShader)->GetFastUniform("BaseTexture");
		static int BloomBlurHorizontal = ((ShaderProgramOpenGL*)GaussBlurShader)->GetFastUniform("Horizontal");
		static int BloomBlurRadius = ((ShaderProgramOpenGL*)GaussBlurShader)->GetFastUniform("Radius");

		static int BloomFinalPassBaseTexture = ((ShaderProgramOpenGL*)BloomShader)->GetFastUniform("BaseTexture");
		static int BloomFinalPassVerticalBlur = ((ShaderProgramOpenGL*)BloomShader)->GetFastUniform("Blur");
		static int BloomFinalPassIntensity = ((ShaderProgramOpenGL*)BloomShader)->GetFastUniform("Intensity");

		BloomBrightPass.Bind({}, {0}, ContextSize);

		((ShaderProgramOpenGL*)BloomBrightShader)->Bind();
		((ShaderProgramOpenGL*)BloomBrightShader)->SetUniform(BrightShaderTexture, (TextureOpenGL*)BaseEffect.ColorTextures[0], 0);
		((ShaderProgramOpenGL*)BloomBrightShader)->SetUniform(BrightShaderTreshold, BloomTreshold);
		Quad.Render();

		BloomBrightPass.Unbind();

		((ShaderProgramOpenGL*)GaussBlurShader)->Bind();
		((ShaderProgramOpenGL*)GaussBlurShader)->SetUniform(BloomBlurRadius, BloomRadius);

		iVector2 Resolution;

		switch (BloomResolution)
		{
		case PostEffectResolution::Full: Resolution = ContextSize;     break;
		case PostEffectResolution::Half: Resolution = ContextSize / 2; break;
		case PostEffectResolution::Quad: Resolution = ContextSize / 4; break;
		}

		for (int i = 0; i < BloomIterations; i++)
		{
			auto Horiz = i == 0 ? BloomBrightPass.ColorTextures[0] : BloomVerticalBlurPass.ColorTextures[0];

			BloomHorizontalBlurPass.Bind({}, {0}, Resolution);
			((ShaderProgramOpenGL*)GaussBlurShader)->SetUniform(BloomBlurTexture, (TextureOpenGL*)Horiz, 0);
			((ShaderProgramOpenGL*)GaussBlurShader)->SetUniform(BloomBlurHorizontal, 1);
			Quad.Render();
			BloomHorizontalBlurPass.Unbind();

			BloomVerticalBlurPass.Bind({}, {0}, Resolution);
			((ShaderProgramOpenGL*)GaussBlurShader)->SetUniform(BloomBlurTexture, (TextureOpenGL*)BloomHorizontalBlurPass.ColorTextures[0], 0);
			((ShaderProgramOpenGL*)GaussBlurShader)->SetUniform(BloomBlurHorizontal, 0);
			Quad.Render();
			BloomVerticalBlurPass.Unbind();
		}

		BloomFinalPass.Bind({}, {0}, ContextSize);

		((ShaderProgramOpenGL*)BloomShader)->Bind();
		((ShaderProgramOpenGL*)BloomShader)->SetUniform(BloomFinalPassBaseTexture, (TextureOpenGL*)BaseEffect.ColorTextures[0], 0);
		((ShaderProgramOpenGL*)BloomShader)->SetUniform(BloomFinalPassVerticalBlur, (TextureOpenGL*)BloomVerticalBlurPass.ColorTextures[0], 1);
		((ShaderProgramOpenGL*)BloomShader)->SetUniform(BloomFinalPassIntensity, BloomIntensity);
		Quad.Render();

		BloomFinalPass.Unbind();
	}

	void Renderer::RenderIcons()
	{
		auto DrawIcon = [&](Vector4 Coords)
		{
			Coords = MainCamera.GetViewProjection() * Coords;

			if (Coords.W > 0.0f)
			{
				Coords /= Coords.W;

				float Aspect = (float)ContextSize.X / (float)ContextSize.Y;

				Quad.Render(Coords.XY(),  0.1f / Vector2(Aspect, 1));
			}
		};

		static int LensFlareShaderTextureID = ((ShaderProgramOpenGL*)(LensFlareShader))->GetFastUniform("Texture");
		((ShaderProgramOpenGL*)(LensFlareShader))->Bind();

		State.SetBlending(true);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		((ShaderProgramOpenGL*)(LensFlareShader))->SetUniform(LensFlareShaderTextureID, (TextureOpenGL*)EditorIconSun, 0);
		for (const auto& Elem : Scn->Lights)
		{
			if (Elem != nullptr)
				if (Elem->Type == 0)
					DrawIcon(Vector4(Elem->Pos, 1));
		}

		((ShaderProgramOpenGL*)(LensFlareShader))->SetUniform(LensFlareShaderTextureID, (TextureOpenGL*)EditorIconLamp, 0);
		for (const auto& Elem : Scn->Lights)
		{
			if (Elem != nullptr)
				if (Elem->Type == 1)
					DrawIcon(Vector4(Elem->Pos, 1));
		}

		((ShaderProgramOpenGL*)(LensFlareShader))->SetUniform(LensFlareShaderTextureID, (TextureOpenGL*)EditorIconFlashlight, 0);
		for (const auto& Elem : Scn->Lights)
		{
			if (Elem != nullptr)
				if (Elem->Type == 2)
					DrawIcon(Vector4(Elem->Pos, 1));
		}

		((ShaderProgramOpenGL*)(LensFlareShader))->SetUniform(LensFlareShaderTextureID, (TextureOpenGL*)EditorIconAudio, 0);
		for (const auto& Elem : Scn->Audio.Mixer.Sources)
		{
			if (Elem != nullptr)
				DrawIcon(Vector4(Elem->Position, 1));
		}

		((ShaderProgramOpenGL*)(LensFlareShader))->SetUniform(LensFlareShaderTextureID, (TextureOpenGL*)EditorIconParticles, 0);
		for (const auto& Elem : TransparentObjects)
		{
			if (Elem.Particles != nullptr)
				DrawIcon(Vector4(Elem.ObjectTransform.Position, 1));
		}

		((ShaderProgramOpenGL*)(LensFlareShader))->Unbind();
	}

	void Renderer::Render()
	{
		PROFILE_GPU(ProfileModuleGPU::GPU);

		static int NoneShaderBaseTextureID = ((ShaderProgramOpenGL*)(NoneShader))->GetFastUniform("BaseTexture");
		static int NoneShaderGamma = ((ShaderProgramOpenGL*)NoneShader)->GetFastUniform("Gamma");
		static int NoneShaderExposure = ((ShaderProgramOpenGL*)NoneShader)->GetFastUniform("Exposure");

		PolygonsRendered = 0;
		OpaqueObjectsRendered = 0;
		TransparentObjectsRendered = 0;

		CompileLists();
		SortLists();

		BaseEffect.Bind({ 1, 1, 1, 0 }, {0}, ContextSize);

		RenderOpaqueStage();
		RenderSkyStage();
		RenderTransparentStage();

		BaseEffect.Unbind();

		Texture* Final = BaseEffect.ColorTextures[0];

		if (BloomEnable)
		{
			RenderBloom();
			Final = BloomFinalPass.ColorTextures[0];
		}

		Vector2 Origin = (Vector2)ViewportOrigin / (Vector2)ContextSize;
		Vector2 Size = (Vector2)ViewportSize / (Vector2)ContextSize;
		Vector2 Center = Size * 0.5f + Origin;

		// Final stage
		{
			PROFILE_GPU(ProfileModuleGPU::FinalStage);

			FinalPass.Bind({}, {}, ContextSize);

			((ShaderProgramOpenGL*)NoneShader)->Bind();
			((ShaderProgramOpenGL*)NoneShader)->SetUniform(NoneShaderBaseTextureID, (TextureOpenGL*)Final, 0);
			((ShaderProgramOpenGL*)NoneShader)->SetUniform(NoneShaderExposure, Exposure);
			((ShaderProgramOpenGL*)NoneShader)->SetUniform(NoneShaderGamma, Gamma);
			//Quad.Render(Center * 2.0f - 1.0f, Size);
			Quad.Render();

			if (EditMode)
			{
				RenderIcons();
			}

			FinalPass.Unbind();
		}

		// Lens flare rendering test, I will use it in the future

		/*static int LensFlareShaderTextureID = ((ShaderProgramOpenGL*)(LensFlareShader))->GetFastUniform("Texture");

		Vector4 Coords(Vector3(0), 1);
		Coords = MainCamera.GetViewProjection() * Coords;

		if (Coords.W != 0.0f)
			Coords /= Coords.W;

		float Aspect = (float)ContextSize.X / (float)ContextSize.Y;

		LensFlareShader->Bind();
		((ShaderProgramOpenGL*)(LensFlareShader))->SetUniform(LensFlareShaderTextureID, (TextureOpenGL*)Blob, 0);
		Quad.Render(Coords.XY(),  0.2f / Vector2(Aspect, 1));
		LensFlareShader->Unbind();*/
	}

	Texture* Renderer::GetFramebufferTexture() const
	{
		return FinalPass.ColorTextures[0];
	}

	Renderer::~Renderer()
	{
		delete Blob;
		delete EditorIconLamp;
		delete NoneShader;
		delete BloomBrightShader;
		delete GaussBlurShader;
		delete BloomShader;
		delete LensFlareShader;
	}

}


