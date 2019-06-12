#pragma once

#include <Audio/AudioSystem.h>
#include <Graphics/Skybox.h>
#include <Graphics/Camera.h>
#include <Graphics/Render.h>
#include <Physics/PhysicsWorld.h>
#include <Scene/GameObject.h>
#include <System/Timer.h>
#include <Core/SmartPointer.h>
#include <Core/Types.h>

#include <vector>
#include <map>

namespace Columbus
{

	class Scene
	{
	private:
		friend class Editor;

		std::map<uint32, SmartPointer<Texture>> Textures;
		std::map<uint32, SmartPointer<ShaderProgram>> ShaderPrograms;

		std::map<uint32, SmartPointer<Mesh>> Meshes;
		std::map<uint32, SmartPointer<Sound>> Sounds;

		Vector<AudioSource*> AudioSources;
		std::vector<Light*> Lights;

		Timer DeltaTime;
		PhysicsWorld PhysWorld;

		Skybox* Sky = nullptr;
		Camera* MainCamera = nullptr;
		AudioListener* Listener = nullptr;

		void RigidbodyWorkflow();
		void RigidbodyPostWorkflow();
	public:
		AudioSystem Audio;
		float TimeFactor = 1.0f;

		Renderer MainRender;

		std::map<uint32, SmartPointer<GameObject>> Objects;
	public:
		Scene();

		bool Load(const char* FileName);

		void Add(uint32 ID, GameObject&& InObject)
		{
			Objects.insert(std::make_pair(ID, SmartPointer<GameObject>(new GameObject(std::move(InObject)))));
		}

		void AddEmpty()
		{
			GameObject GO;
			std::string Name = "Object ";
			for (uint32 i = 0;; i++)
			{
				if (GetGameObject(Name + std::to_string(i)) == nullptr)
				{
					Name += std::to_string(i);
					break;
				}
			}
			GO.Name = Name;
			Add(Objects.size(), std::move(GO));
		}

		void SetSkybox(Skybox* InSky) { Sky = InSky; }
		void SetCamera(Camera& InMainCamera) { MainCamera = &InMainCamera; }
		void SetAudioListener(AudioListener& InListener) { Listener = &InListener; }

		GameObject* GetGameObject(uint32 ID) const;
		GameObject* GetGameObject(const std::string& Name) const;

		void Update();
		void Render();

		~Scene();
	};

}


