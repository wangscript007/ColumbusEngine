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
	C_Component::C_Component()
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	bool C_Component::onCreate()
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	bool C_Component::onUpdate()
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	void C_Component::update(const float aTimeTick)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	void C_Component::render(C_Transform& aTransform)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	std::string C_Component::getType()
	{
		return "Component";
	}
	//////////////////////////////////////////////////////////////////////////////
	void C_Component::destroy()
	{
		delete this;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	C_Component::~C_Component()
	{

	}

}

