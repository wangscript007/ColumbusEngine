/************************************************
*                  Component.cpp                *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*                Nika(Columbus) Red             *
*                   12.11.2017                  *
*************************************************/
#include <Scene/Component.h>

namespace Columbus
{

	//////////////////////////////////////////////////////////////////////////////
	Component::Component()
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	bool Component::OnCreate()
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	bool Component::OnUpdate()
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	void Component::Update(const float aTimeTick)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	void Component::Render(Transform& aTransform)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	Component::Type Component::GetType() const
	{
		return COMPONENT;
	}
	//////////////////////////////////////////////////////////////////////////////
	void Component::Destroy()
	{
		delete this;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	Component::~Component()
	{

	}

}


