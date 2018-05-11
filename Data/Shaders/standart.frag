#define LIGHT_NUM 8

in vec3 varPos;
in vec2 varUV;
in vec3 varNormal;
in vec3 varFragPos;
in mat3 varTBN;

struct Material
{
	sampler2D DiffuseMap;
	sampler2D SpecularMap;
	sampler2D NormalMap;
	samplerCube ReflectionMap;

	vec4 Color;
	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;
	float ReflectionPower;
	bool Lighting;
};

struct Light
{
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	int Type;
	float Constant;
	float Linear;
	float Quadratic;
	float InnerCutoff;
	float OuterCutoff;
};

struct Camera
{
	vec3 Position;
};

uniform Material uMaterial;
uniform Light uLights[LIGHT_NUM];
uniform Camera uCamera;

vec4 DiffuseMap;
vec3 SpecularMap;
vec3 NormalMap;

bool IsSpecularMap = false;

vec3 Normal;

vec3 AmbientColor = vec3(0);
vec3 DiffuseColor = vec3(0);
vec3 SpecularColor = vec3(0);
vec3 CubemapColor = vec3(0);
vec4 Lighting = vec4(1);

mat3 TBN;

void Init(void);
void LightCalc(int id);
void Cubemap(void);
void Final(void);

void main(void)
{
	Init();

	if (uMaterial.Lighting == true)
	{
		LightCalc(0);
		LightCalc(1);
		LightCalc(2);
		LightCalc(3);
		LightCalc(4);
		LightCalc(5);
		LightCalc(6);
		LightCalc(7);

		Lighting = vec4(AmbientColor + DiffuseColor + SpecularColor, 1.0);
	}
	
	Cubemap();

	Final();
}

void Init(void)
{
	DiffuseMap = texture(uMaterial.DiffuseMap, varUV);
	SpecularMap = vec3(texture(uMaterial.SpecularMap, varUV));
	NormalMap = vec3(texture(uMaterial.NormalMap, varUV));

	if (DiffuseMap.w <= 0.1) discard;

	TBN = varTBN;

	if (textureSize(uMaterial.SpecularMap, 1).xy != vec2(0))
		IsSpecularMap = true;

	if (NormalMap != vec3(0))
		Normal = normalize(NormalMap * 2.0 - 1.0) * TBN;
	else
		Normal = varNormal;
}

void LightCalc(int id)
{
	if (uLights[id].Type == -1) return;

	vec3 lightDir;

	float attenuation = 0.0;

	switch (int(uLights[id].Type))
	{
	case 0:
		lightDir = normalize(-uLights[id].Position);
		break;
	default:
		lightDir = normalize(-uLights[id].Position + varPos);
		break;
	};

	vec3 viewDir = normalize(uCamera.Position - varFragPos);

	float diff = max(0.0, dot(Normal, -lightDir));

	vec3 reflect = normalize(reflect(lightDir, Normal));
	float spec = pow(max(0.0, dot(viewDir, reflect)), 32);
	vec3 specular = uMaterial.SpecularColor * uLights[id].Color * spec * 0.5;

	vec3 tmpAmbient = vec3(0);
	vec3 tmpDiffuse = vec3(0);
	vec3 tmpSpecular = vec3(0);

	tmpAmbient = uMaterial.AmbientColor * uLights[id].Color * vec3(uMaterial.Color);
	tmpDiffuse = uLights[id].Color * uMaterial.DiffuseColor * diff * uMaterial.Color.rgb;
	
	if (IsSpecularMap)
		tmpSpecular = specular * uMaterial.SpecularColor * uMaterial.Color.rgb * SpecularMap;
	else
		tmpSpecular = specular * uMaterial.SpecularColor * uMaterial.Color.rgb;

	if (int(uLights[id].Type) > 0)
	{
		float distance = length(uLights[id].Position - varPos);
		attenuation = 1.0 / (uLights[id].Constant +
							uLights[id].Linear * distance +
							uLights[id].Quadratic * (distance * distance));

		tmpAmbient *= attenuation;
		tmpDiffuse *= attenuation;
		tmpSpecular *= attenuation;
	}

	AmbientColor += tmpAmbient;
	DiffuseColor += tmpDiffuse;
	SpecularColor += tmpSpecular;
}

void Cubemap(void)
{
	vec3 I = normalize(uCamera.Position - varFragPos);
    //vec3 R = reflect(I, normalize(varFragPos));
   	vec3 R = normalize(reflect(I, Normal));
   	CubemapColor = texture(uMaterial.ReflectionMap, -vec3(R.x, R.y, R.z)).rgb * uMaterial.ReflectionPower;

   	if (IsSpecularMap)
   	{
		CubemapColor *= SpecularMap;
   	}

	CubemapColor *= uMaterial.Color.rgb;
}

void Final(void)
{
	if (DiffuseMap.xyz != vec3(0))
	{
		if (CubemapColor != vec3(0))
			FragColor = Lighting * DiffuseMap + vec4(CubemapColor, 1.0);
		else
			FragColor = Lighting * DiffuseMap;
	}
	else
	{
		if (CubemapColor != vec3(0))
			FragColor = Lighting + vec4(CubemapColor, 1.0);
		else
			FragColor = Lighting;
	}
}





