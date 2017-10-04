#version 130

<<<<<<< HEAD
#define LIGHT_NUM 4

=======
>>>>>>> dev
varying vec3 varPos;
varying vec2 varUV;
varying vec3 varNormal;
varying vec3 varTangent;
varying vec3 varBitangent;
varying vec3 varFragPos;

struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
};

struct Camera
{
	vec3 pos;
};

<<<<<<< HEAD
uniform samplerCube uReflectionMap;
uniform Material uMaterial;
uniform Camera uCamera;
uniform float MaterialUnif[14];
uniform float LightUnif[15];
=======
uniform Material uMaterial;
uniform Camera uCamera;
uniform float MaterialUnif[14];
uniform samplerCube uReflectionMap;
>>>>>>> dev

vec4 DiffuseMap;
vec3 SpecularMap;
vec3 NormalMap;

vec3 Normal;

<<<<<<< HEAD
vec3 AmbientColor = vec3(0);
vec3 DiffuseColor = vec3(0);
vec3 SpecularColor = vec3(0);
vec3 CubemapColor = vec3(0);
=======
vec3 AmbientColor;
vec3 DiffuseColor;
vec3 SpecularColor;
vec3 CubemapColor;
>>>>>>> dev

vec4 FinalColor;

mat3 TBN;

void Init(void);
<<<<<<< HEAD
void Light(int id);
=======
void Ambient(void);
void Diffuse(void);
void Specular(void);
>>>>>>> dev
void Cubemap(void);
void Final(void);

void main(void)
{
	Init();

<<<<<<< HEAD
	Light(0);

=======
	Ambient();
	Diffuse();
	Specular();
>>>>>>> dev
	Cubemap();

	Final();

	gl_FragColor = FinalColor;
}

void Init(void)
{
	DiffuseMap = texture(uMaterial.diffuseMap, varUV);
	SpecularMap = vec3(texture(uMaterial.specularMap, varUV));
	NormalMap = vec3(texture(uMaterial.normalMap, varUV));

	TBN = transpose(mat3(varTangent, varBitangent, varNormal));

	if (NormalMap != vec3(0))
		Normal = normalize(NormalMap * 2.0 - 1.0) * TBN;
	else
		Normal = varNormal;
}

<<<<<<< HEAD
void Light(int id)
{
	vec4 MaterialColor = vec4(MaterialUnif[0], MaterialUnif[1], MaterialUnif[2], MaterialUnif[3]);
	vec3 MaterialAmbient = vec3(MaterialUnif[4], MaterialUnif[5], MaterialUnif[6]);
	vec3 MaterialDiffuse = vec3(MaterialUnif[7], MaterialUnif[8], MaterialUnif[9]);
	vec3 MaterialSpecular = vec3(MaterialUnif[10], MaterialUnif[11], MaterialUnif[12]);

	int offset = id * 15;

	vec3 LightColor = vec3(LightUnif[0 + offset], LightUnif[1 + offset], LightUnif[2 + offset]);
	vec3 LightPos = vec3(LightUnif[3 + offset], LightUnif[4 + offset], LightUnif[5 + offset]);
	vec3 LightDir = vec3(LightUnif[6 + offset], LightUnif[7 + offset], LightUnif[8 + offset]);
	float LightType = LightUnif[9 + offset];
	float LightConstant = LightUnif[10 + offset];
	float LightLinear = LightUnif[11 + offset];
	float LightQuadratic = LightUnif[12 + offset];
	float LightInnerAngle = LightUnif[13 + offset];
	float LightOuterAngle = LightUnif[14 + offset];

	vec3 lightDir;

	float attenuation = 0.0;

	switch (int(LightType))
	{
	case 0:
		lightDir = normalize(-LightPos);
		break;
	default:
		lightDir = normalize(-LightPos - varPos);
		break;
	};

	vec3 viewDir = normalize(uCamera.pos - varPos);

	float diff = max(0.0, dot(Normal, -lightDir));

	vec3 reflect = normalize(reflect(lightDir, Normal));
	float spec = pow(max(0.0, dot(viewDir, reflect)), 32);
	vec3 specular = MaterialSpecular * vec3(1) * spec * 0.5;

	AmbientColor += MaterialAmbient * vec3(1, 1, 1) * vec3(MaterialColor);
	DiffuseColor += vec3(1) * MaterialDiffuse * diff;

	if (SpecularMap != vec3(0))
		SpecularColor += specular * MaterialSpecular * SpecularMap;
	else
		SpecularColor += specular * MaterialSpecular;

	if (int(LightType) != 0)
	{
		float distance = length(LightPos - varPos);
		attenuation = 1.0 / (LightConstant +
							LightLinear * distance +
							LightQuadratic * (distance * distance));

		AmbientColor *= attenuation;
		DiffuseColor *= attenuation;
		SpecularColor *= attenuation;
	}
=======
void Ambient(void)
{
	vec4 MaterialColor = vec4(MaterialUnif[0], MaterialUnif[1], MaterialUnif[2], MaterialUnif[3]);
	vec3 MaterialAmbient = vec3(MaterialUnif[4], MaterialUnif[5], MaterialUnif[6]);
	AmbientColor = MaterialAmbient * vec3(1, 1, 1) * vec3(MaterialColor);
}

void Diffuse(void)
{
	vec3 MaterialDiffuse = vec3(MaterialUnif[7], MaterialUnif[8], MaterialUnif[9]);

	vec3 lightDir = normalize(vec3(-uCamera.pos));
	vec3 viewDir = normalize(uCamera.pos - varPos);
	float diff = max(0.0, dot(Normal, -lightDir));
	DiffuseColor = vec3(1) * MaterialDiffuse * diff;
}

void Specular(void)
{
	vec3 MaterialSpecular = vec3(MaterialUnif[10], MaterialUnif[11], MaterialUnif[12]);

	vec3 lightDir = normalize(vec3(-uCamera.pos));
	vec3 viewDir = normalize(uCamera.pos - varPos);
	vec3 reflect = normalize(reflect(lightDir, Normal));
	float spec = pow(max(0.0, dot(viewDir, reflect)), 32);

	vec3 specular = MaterialSpecular * vec3(1) * spec * 0.5;

	if (SpecularMap != vec3(0))
		SpecularColor = specular * MaterialSpecular * SpecularMap;
	else
		SpecularColor = specular * MaterialSpecular;
>>>>>>> dev
}

void Cubemap(void)
{
	vec3 I = normalize(uCamera.pos - varFragPos);
    //vec3 R = reflect(I, normalize(varFragPos));
   	vec3 R = normalize(reflect(I, Normal));
   	CubemapColor = textureCube(uReflectionMap, vec3(R.x, R.y, R.z)).rgb * MaterialUnif[13];

   	if (SpecularMap != vec3(0))
		CubemapColor *= SpecularMap;
}

void Final(void)
{
	vec4 Lighting = vec4(AmbientColor + DiffuseColor + SpecularColor, 1.0);

	if (DiffuseMap != vec4(0))
	{
		if (CubemapColor != vec3(0))
			FinalColor = Lighting * DiffuseMap + vec4(CubemapColor, 1.0);
		else
			FinalColor = Lighting * DiffuseMap;
	}
	else
	{
		if (CubemapColor != vec3(0))
			FinalColor = Lighting + vec4(CubemapColor, 1.0);
		else
			FinalColor = Lighting;
	}
<<<<<<< HEAD
=======

	//FinalColor = vec4(CubemapColor, 1.0);
>>>>>>> dev
}



