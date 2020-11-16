#version 430 core

/** Structs */
struct PointLight {
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};

#define NR_POINT_LIGHTS 5

/** Inputs */
in vec3 v_Normal;
in vec2 v_TexCoords;
in vec3 v_FragPos;

/** Outputs */
out vec4 FragColor;

/** Uniforms */
layout(binding = 0) uniform sampler2D u_Texture;
uniform vec3 u_ViewPos;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];

/** Functions */
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos);

void main()
{
	/** Texture */
	vec3 texColor = pow(texture(u_Texture, v_TexCoords).rgb, vec3(2.2));

	/** Calculating lighting from all point lights */
	vec3 blinn_phong = vec3(0.0);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		blinn_phong += CalcPointLight(u_PointLights[i], normalize(v_Normal), v_FragPos, u_ViewPos);

	texColor *= blinn_phong;
	texColor = pow(texColor, vec3(1.0/2.2));

	FragColor = vec4(texColor, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos)
{
   /** Ambient Lighting */
	float ambientStrength = 0.00000000000000000001;
	vec3 ambient = ambientStrength * light.color;

	/** Diffuse Lighting */
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * light.color;

	/** Blinn-Phong Specular Lighting */
	int shininess = 64;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	vec3 specular = spec * light.color;

	/** Attenuation */
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (distance * distance);

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

    return (ambient + diffuse + specular);
} 