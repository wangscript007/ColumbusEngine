/************************************************
*                 Material.cpp                  *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*             Nikolay(Columbus) Red             *
*                   20.07.2017                  *
*************************************************/

#include <Graphics/Material.h>

namespace C
{

	//////////////////////////////////////////////////////////////////////////////
	//Constructor
	C_Material::C_Material()
	{
		color = C_Vector4(1, 1, 1, 1);
		diffuse = C_Vector3(1, 1, 1);
		ambient = C_Vector3(0.25, 0.25, 0.25);
		specular = C_Vector3(1, 1, 1);
		shininess = 32;
		reflectionPower = 0.2;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Constructor 2
	C_Material::C_Material(const char* aFile)
	{
		loadFromXML(aFile);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set color
	void C_Material::setColor(const C_Vector4 aColor)
	{
		color = (C_Vector4)aColor;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set light ambient color
	void C_Material::setAmbient(const C_Vector3 aAmbient)
	{
		ambient = (C_Vector3)aAmbient;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set light diffuse color
	void C_Material::setDiffuse(const C_Vector3 aDiffuse)
	{
		diffuse = (C_Vector3)aDiffuse;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set light specular color
	void C_Material::setSpecular(const C_Vector3 aSpecular)
	{
		specular = (C_Vector3)aSpecular;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set specular shininess
	void C_Material::setShininess(const float aShininess)
	{
		shininess = (float)aShininess;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set diffuse texture
	void C_Material::setTexture(const C_Texture* aTexture)
	{
		texture = (C_Texture*)aTexture;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set specular texture
	void C_Material::setSpecMap(const C_Texture* aSpecMap)
	{
		specmap = (C_Texture*)aSpecMap;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set normal texture
	void C_Material::setNormMap(const C_Texture* aNormMap)
	{
		normmap = (C_Texture*)aNormMap;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set shader
	void C_Material::setShader(const C_Shader* aShader)
	{
		shader = (C_Shader*)aShader;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set cubemap reflection
	void C_Material::setReflection(const C_Cubemap* aReflecction)
	{
		envRefl = (C_Cubemap*)aReflecction;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set reflection power
	void C_Material::setReflectionPower(const float aPower)
	{
		reflectionPower = (float)aPower;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set discard alpha
	void C_Material::setDiscard(const bool aDiscard)
	{
		discard = (bool)aDiscard;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return color
	C_Vector4 C_Material::getColor()
	{
		return color;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return light ambient color
	C_Vector3 C_Material::getAmbient()
	{
		return ambient;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return light diffuer color
	C_Vector3 C_Material::getDiffuse()
	{
		return diffuse;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return light specular color
	C_Vector3 C_Material::getSpecular()
	{
		return specular;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return specular shininess
	float C_Material::getShininess()
	{
		return shininess;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return diffuse texture
	C_Texture* C_Material::getTexture()
	{
		return texture;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return specular texture
	C_Texture* C_Material::getSpecMap()
	{
		return specmap;
	}
	//////////////////////////////////////////////////////////////////////////////
	C_Texture* C_Material::getNormMap()
	{
		return normmap;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return shader
	C_Shader* C_Material::getShader()
	{
		return shader;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return cubemap reflection
	C_Cubemap* C_Material::getReflection()
	{
		return envRefl;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return reflection power
	float C_Material::getReflectionPower()
	{
		return reflectionPower;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return discard alpha
	bool C_Material::getDiscard()
	{
		return discard;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Serialize to XML file
	void C_Material::saveToXML(const char* aFile)
	{
		C_XMLDoc doc;
		C_XMLNode* root = doc.NewElement("Material");
		doc.InsertFirstChild(root);

		C_XMLElement* tmp = doc.NewElement("Color");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't save Material color: %s", aFile);
			return;
		}
		tmp->SetAttribute("R", color.x);
		tmp->SetAttribute("G", color.y);
		tmp->SetAttribute("B", color.z);
		tmp->SetAttribute("A", color.w);
		root->InsertEndChild(tmp);

		tmp = NULL;

		tmp = doc.NewElement("Ambient");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't save Material ambient: %s", aFile);
			return;
		}
		tmp->SetAttribute("R", ambient.x);
		tmp->SetAttribute("G", ambient.y);
		tmp->SetAttribute("B", ambient.z);
		root->InsertEndChild(tmp);

		tmp = NULL;

		tmp = doc.NewElement("Diffuse");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't save Material diffuse: %s", aFile);
			return;
		}
		tmp->SetAttribute("R", diffuse.x);
		tmp->SetAttribute("G", diffuse.y);
		tmp->SetAttribute("B", diffuse.z);
		root->InsertEndChild(tmp);

		tmp = NULL;

		tmp = doc.NewElement("Specular");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't save Material specular: %s", aFile);
			return;
		}
		tmp->SetAttribute("R", specular.x);
		tmp->SetAttribute("G", specular.y);
		tmp->SetAttribute("B", specular.z);
		root->InsertEndChild(tmp);

		tmp = NULL;

		tmp = doc.NewElement("Shininess");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't save Material shininess: %s", aFile);
			return;
		}
		tmp->SetText(shininess);
		root->InsertEndChild(tmp);

		tmp = NULL;

		tmp = doc.NewElement("ReflectionPower");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't save Material reflection power: %s", aFile);
			return;
		}
		tmp->SetText(reflectionPower);
		root->InsertEndChild(tmp);

		tmp = NULL;

		tmp = doc.NewElement("Discard");
		if (tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't save Material discard: %s", aFile);
			return;
		}
		tmp->SetText(discard);
		root->InsertEndChild(tmp);

		if(doc.SaveFile(aFile) != C_XML_SUCCESS)
		{
			C_Error("Can't save Material: %s", aFile);
			return;
		}

		C_Success("Material saved: %s", aFile);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Deserialize from XML file
	void C_Material::loadFromXML(const char* aFile)
	{
		C_XMLDoc doc;
		if(doc.LoadFile(aFile) != C_XML_SUCCESS)
		{
			C_Error("Can't load Material: %s\n", aFile);
			return;
		}

		C_XMLElement* root = doc.FirstChildElement("Material");
		if(root == NULL && root == nullptr)
		{
			C_Error("Can't load Material: %s\n", aFile);
			return;
		}

		C_XMLElement* tmp = root->FirstChildElement("Color");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't load Material color: %s", aFile);
			return;
		}

		tmp->QueryFloatAttribute("R", &color.x);
		tmp->QueryFloatAttribute("G", &color.y);
		tmp->QueryFloatAttribute("B", &color.z);
		tmp->QueryFloatAttribute("A", &color.w);

		tmp = NULL;

		tmp = root->FirstChildElement("Ambient");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't load Material ambient: %s", aFile);
			return;
		}

		tmp->QueryFloatAttribute("R", &ambient.x);
		tmp->QueryFloatAttribute("G", &ambient.y);
		tmp->QueryFloatAttribute("B", &ambient.z);

		tmp = NULL;

		tmp = root->FirstChildElement("Diffuse");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't load Material diffuse: %s", aFile);
			return;
		}

		tmp->QueryFloatAttribute("R", &diffuse.x);
		tmp->QueryFloatAttribute("G", &diffuse.y);
		tmp->QueryFloatAttribute("B", &diffuse.z);

		tmp = NULL;

		tmp = root->FirstChildElement("Specular");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't load Material specular: %s", aFile);
			return;
		}

		tmp->QueryFloatAttribute("R", &specular.x);
		tmp->QueryFloatAttribute("G", &specular.y);
		tmp->QueryFloatAttribute("B", &specular.z);

		tmp = NULL;

		tmp = root->FirstChildElement("Shininess");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't load Material shininess: %s", aFile);
			return;
		}

		tmp->QueryFloatText(&shininess);

		tmp = NULL;

		tmp = root->FirstChildElement("ReflectionPower");
		if(tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't load Material reflection power: %s", aFile);
			return;
		}

		tmp->QueryFloatText(&reflectionPower);

		tmp = NULL;

		tmp = root->FirstChildElement("Discard");
		if (tmp == NULL && tmp == nullptr)
		{
			C_Error("Can't load Material discard: %s", aFile);
			return;
		}

		tmp->QueryBoolText(&discard);

		tmp = NULL;

		C_Success("Material loaded: %s", aFile);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Destructor
	C_Material::~C_Material()
	{

	}

}
