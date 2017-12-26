/************************************************
*                MeshRenderer.cpp               *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*                Nika(Columbus) Red             *
*                   12.11.2017                  *
*************************************************/
#include <Scene/MeshRenderer.h>

namespace Columbus
{

	//////////////////////////////////////////////////////////////////////////////
	C_MeshRenderer::C_MeshRenderer(C_Mesh* aMesh) :
		mMesh(aMesh)
	{
		
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	bool C_MeshRenderer::onCreate()
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	bool C_MeshRenderer::onUpdate()
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	void C_MeshRenderer::update(const float aTimeTick)
	{
		if (mMesh != nullptr)
			mMesh->setCamera(mCamera);
	}
	//////////////////////////////////////////////////////////////////////////////
	void C_MeshRenderer::render(C_Transform& aTransform)
	{
		if (mMesh != nullptr)
			mMesh->render(aTransform);
	}
	//////////////////////////////////////////////////////////////////////////////
	std::string C_MeshRenderer::getType()
	{
		return "MeshRenderer";
	}
	//////////////////////////////////////////////////////////////////////////////
	void C_MeshRenderer::setLights(std::vector<C_Light*> aLights)
	{
		mMesh->setLights(aLights);
	}
	//////////////////////////////////////////////////////////////////////////////
	void C_MeshRenderer::setCamera(const C_Camera aCamera)
	{
		mCamera = static_cast<C_Camera>(aCamera);
	}
	//////////////////////////////////////////////////////////////////////////////
	C_Shader* C_MeshRenderer::getShader() const
	{
		if (mMesh == nullptr) return nullptr;
		return mMesh->mMat.getShader();
	}
	//////////////////////////////////////////////////////////////////////////////
	void C_MeshRenderer::setShader(C_Shader* aShader)
	{
		if (mMesh == nullptr) return;
		mMesh->mMat.setShader(aShader);
	}
	//////////////////////////////////////////////////////////////////////////////
	void C_MeshRenderer::setReflection(const C_Cubemap* aCubemap)
	{
		if (mMesh == nullptr) return;
		mMesh->mMat.setReflection(aCubemap);
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	C_MeshRenderer::~C_MeshRenderer()
	{

	}

}


