#version 430 core
#define DEG_TO_RAD 0.0174532925199
#define RAD_TO_DEG 57.2957795131
out vec4 FragColor;

in vec3 vertexColor;
in vec3 texCoord;
in vec4 normal;
in vec3 fragPos;

struct DirLight {
    vec4 direction;
  
    vec3 diffuse;
    vec3 specular;
}; 
struct PointLight {    
    vec4 position;
    
    float constant;
    float linear;
    float quadratic; 
	float padding;

    vec4 diffuse;
    vec4 specular;
}; //64 (60)
struct SpotLight {
    vec4  position;
    vec4  direction;
    float cutOff;
    float outCutOff;
	float padding1;
	float padding2;
	vec4 diffuse;
	vec4 specular;
}; //80 (72)

uniform sampler3D texture0;
layout (location = 0) uniform DirLight dirLight;
layout (std140) uniform pointLights
{
	PointLight pointLightArray[100];
	int pointLightCount;
};
layout (std140) uniform spotLights
{
	SpotLight spotLightArray[30];
	int spotLightCount;
};
uniform vec3 ambient;

vec3 CalcDirLight(DirLight light, vec4 normal, vec3 viewDir, float shininess);
vec3 CalcPointLight(PointLight light, vec4 normal, vec3 fragPos, vec3 viewDir, float shininess);
vec3 CalcSpotLight(SpotLight light, vec4 normal, vec3 fragPos, vec3 viewDir, float shininess);

void main()
{
	vec4 texColor = texture(texture0, texCoord) * vec4(vertexColor,1.0);
	if(texColor.a< 0.1)
		discard;
	if (normal.xyzw == vec4(0,0,0,0))
	{
		FragColor =  texColor;
	}
	else
	{
		float ambientStrength = 0.1;
		vec3 lightColor = vec3(1.0,1.0,1.0);
		float shininess = 128;
		float specularStrength = 0.5;
		//vec4 lightDir = normalize(lightPos-vec4(fragPos.xyz,0));
		vec3 viewDir = normalize(-fragPos);
		///vec4 reflectDir = reflect(-lightDir, normal);  
		//vec4 halfwayDir = normalize(lightDir + vec4(viewDir.xyz,0));

		///float spec = pow(max(dot(glm::vec4(viewDir.xyz,0), reflectDir), 0.0), shininess);
		//float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
		vec3 result = vec3(0);
		result = CalcDirLight(dirLight, normal, viewDir, shininess);
		for (int i = 0; i < pointLightCount; i++)
			result += CalcPointLight(pointLightArray[i], normal, fragPos, viewDir, shininess);
		for (int i = 0; i < spotLightCount; i++)
			result += CalcSpotLight(spotLightArray[i], normal, fragPos, viewDir, shininess);
		FragColor =  texColor*vec4(ambient+result,1.0);
	}
}

vec3 CalcDirLight(DirLight light, vec4 normal, vec3 viewDir, float shininess)
{
    vec4 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec4 halfwayDir = normalize(lightDir + vec4(viewDir.xyz,0));
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // combine results
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture0, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture0, texCoord));
    return (diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec4 normal, vec3 fragPos, vec3 viewDir, float shininess)
{
    vec4 lightDir = normalize(light.position - vec4(fragPos,0));
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec4 halfwayDir = normalize(lightDir + vec4(viewDir.xyz,0));
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // attenuation
    float dist = length(light.position - vec4(fragPos,0));
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
  			     light.quadratic * (dist * dist));    
    // combine results
    vec3 diffuse  = light.diffuse.xyz  * diff * vec3(texture(texture0, texCoord));
    vec3 specular = light.specular.xyz * spec * vec3(texture(texture0, texCoord));
    diffuse  *= attenuation;
    specular *= attenuation;
    return (diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light, vec4 normal, vec3 fragPos, vec3 viewDir, float shininess)
{
	vec4 lightDir = normalize(light.position - vec4(fragPos,0));
	float theta = dot(lightDir, normalize(-light.direction));
	//return glm::vec3(theta);
	float intensity = clamp((theta - light.outCutOff) / (light.cutOff - light.outCutOff), 0.0, 1.0);
	if (theta > light.outCutOff)
	{
		float diff = max(dot(normal, lightDir), 0.0);
		// specular shading
		vec4 halfwayDir = normalize(lightDir + vec4(viewDir.xyz,0));
		float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
		// combine results
		vec3 diffuse  = light.diffuse.xyz  * diff * vec3(texture(texture0, texCoord));
		vec3 specular = light.specular.xyz * spec * vec3(texture(texture0, texCoord));
		diffuse  *= intensity;
		specular *= intensity;
		return (diffuse + specular);
	}
	else
	{
		return vec3(0);
	}
}