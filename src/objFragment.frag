#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	sampler2D texture_displacement1;
	float height_scale;

	float shininess;
};

struct DirLight{
	vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	int isOn;
};

struct PointLight{
	vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;

	int isOn;
};

struct SpotLight{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;

	int isOn;
};

in vec2 TextCoord;
in mat3 TBN;
in vec3 FragPos;

uniform vec3 viewPos;

uniform Material material;
uniform vec3 objColor;

uniform DirLight dirLight;
#define MAX_POINT_LIGHTS 10
uniform int nrPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform int useDisplacement;
uniform int useNormal;
uniform int useSpecular;
uniform int useDiffuse;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 textCoord);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 textCoord);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 textCoord);

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
	// properties
	vec3 viewDir = normalize(viewPos - FragPos);
	vec2 textCoord;
	if(useDisplacement == 1){
		textCoord = ParallaxMapping(TextCoord, TBN * viewDir);
	}
	else{
		textCoord = TextCoord;
	}
	vec3 norm = TBN[2].xyz;
	if(useNormal == 1){
		norm = texture(material.texture_normal1, textCoord).rgb;
		norm = norm * 2.0 - 1.0;
	}
	norm = normalize(TBN * norm); 
	vec3 result;
	if(dirLight.isOn == 1)
		result = CalcDirLight(dirLight, norm, viewDir, textCoord);
	for(int i = 0; i < nrPointLights; i++){
		if(pointLights[i].isOn == 1)
			result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, textCoord);
	}
	if(spotLight.isOn == 1)
		result += CalcSpotLight(spotLight, norm, FragPos, viewDir, textCoord);
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 textCoord)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),
	material.shininess);
	// combine results
	vec3 ambient, diffuse, specular;
	if(useDiffuse == 1){
		ambient = light.ambient * vec3(texture(material.texture_diffuse1, textCoord));
		diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, textCoord));
	}
	else{
		ambient = light.ambient * objColor;
		diffuse = light.diffuse * diff * objColor;
	}
	if(useSpecular == 1){
		specular = light.specular * spec * vec3(texture(material.texture_specular1, textCoord).r);
	}
	else if(useDiffuse == 0){
		specular = light.specular * spec * objColor;
	}
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 textCoord)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),
	material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
	light.quadratic * (distance * distance));
	// combine results
	vec3 ambient, diffuse, specular;
	if(useDiffuse == 1){
		ambient = light.ambient * vec3(texture(material.texture_diffuse1, textCoord));
		diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, textCoord));
	}
	else{
		ambient = light.ambient * objColor;
		diffuse = light.diffuse * diff * objColor;
	}
	if(useSpecular == 1){
		specular = light.specular * spec * vec3(texture(material.texture_specular1, textCoord).r);
	}
	else if(useDiffuse == 0){
		specular = light.specular * spec * objColor;
	}
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 textCoord)
{
	vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient, diffuse, specular;
	if(useDiffuse == 1){
		ambient = light.ambient * vec3(texture(material.texture_diffuse1, textCoord));
		diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, textCoord));
	}
	else{
		ambient = light.ambient * objColor;
		diffuse = light.diffuse * diff * objColor;
	}
	if(useSpecular == 1){
		specular = light.specular * spec * vec3(texture(material.texture_specular1, textCoord).r);
	}
	else if(useDiffuse == 0){
		specular = light.specular * spec * objColor;
	}

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir){
	// number of depth layers
	const float minLayers = 15.0;
	const float maxLayers = 40.0;
	float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
	// calculate the size of each layer
	float layerDepth = 1.0 / numLayers;
	// depth of current layer
	float currentLayerDepth = 0.0;
	// amount to shift the texture coordinates per layer (from vector P)
	vec2 P = viewDir.xy * material.height_scale;
	vec2 deltaTexCoords = P / numLayers;

	// get initial values
	vec2 currentTexCoords = texCoords;
	float currentDepthMapValue = 1.0 - texture(material.texture_displacement1, currentTexCoords).r;
	while(currentLayerDepth < currentDepthMapValue)
	{
		// shift texture coordinates along direction of P
		currentTexCoords -= deltaTexCoords;
		// get depthmap value at current texture coordinates
		currentDepthMapValue = 1.0 - texture(material.texture_displacement1, currentTexCoords).r;
		// get depth of next layer
		currentLayerDepth += layerDepth;
	}
	// get texture coordinates before collision (reverse operations)
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
	// get depth after and before collision for linear interpolation
	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = 1.0 - texture(material.texture_displacement1, prevTexCoords).r - currentLayerDepth + layerDepth;
	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	return finalTexCoords;
}