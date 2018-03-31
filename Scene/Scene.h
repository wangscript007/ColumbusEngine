/************************************************
*              	     Scene.h                    *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*                Nika(Columbus) Red             *
*                   12.11.2017                  *
*************************************************/
#pragma once

#include <Scene/GameObject.h>
#include <Scene/ComponentLight.h>
#include <Scene/ComponentMeshRenderer.h>
#include <Scene/ComponentParticleSystem.h>
#include <Scene/ComponentRigidbody.h>
#include <Graphics/Skybox.h>
#include <Graphics/Camera.h>
#include <Graphics/Render.h>
#include <Graphics/Primitives.h>
#include <Graphics/PostEffect.h>
#include <Graphics/OpenGL/MeshOpenGL.h>
#include <Graphics/OpenGL/TextureOpenGL.h>
#include <Graphics/OpenGL/ShaderOpenGL.h>
#include <Physics/PhysicsShape.h>
#include <Physics/PhysicsShapeBox.h>
#include <Physics/PhysicsShapeCapsule.h>
#include <Physics/PhysicsShapeCone.h>
#include <Physics/PhysicsShapeConvexHull.h>
#include <Physics/PhysicsShapeCylinder.h>
#include <Physics/PhysicsShapeMultiSphere.h>
#include <Physics/PhysicsShapeSphere.h>
#include <Physics/PhysicsWorld.h>
#include <System/ResourceManager.h>
#include <Core/Types.h>

namespace Columbus
{

	class Scene
	{
	private:
		std::map<uint32, GameObject*> mObjects;
		std::vector<Light*> mLights;
		std::map<uint32, Mesh*> mMeshes;
		std::map<uint32, Texture*> mTextures;
		std::map<uint32, Shader*> mShaders;

		Timer DeltaTime;
		PhysicsWorld PhysWorld;

		Skybox* mSkybox = nullptr;
		Camera* mCamera = nullptr;

		PostEffect mNoneEffect;
		Shader* mNoneShader = nullptr;

		Vector2 mContextSize = Vector2(640, 480);

		void lightWorkflow();
		void meshWorkflow();
		void particlesWorkflow();
		void rigidbodyWorkflow();
		void rigidbodyPostWorkflow();
	public:
		Scene();

		bool load(std::string aFile);

		void add(unsigned int aID, GameObject* aMesh);
		void setSkybox(const Skybox* aSkybox);
		void setCamera(const Camera* aCamera);
		void setContextSize(const Vector2 aContextSize);

		GameObject* getGameObject(const unsigned int aID) const;
		GameObject* getGameObject(const std::string aName) const;

		void update();
		void render();

		~Scene();
	};

}
