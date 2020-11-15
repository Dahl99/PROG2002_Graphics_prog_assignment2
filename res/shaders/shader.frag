#version 430 core

/** Inputs */
in vec3 v_Normal;
in vec2 v_TexCoords;
in vec3 v_FragPos;

/** Outputs */
out vec4 FragColor;

/** Uniforms */
layout(binding = 0) uniform sampler2D u_Texture;
uniform vec3 u_LightSrcPos;
uniform vec3 u_ViewPos;


void main()
{
	vec3 lightColor = vec3(1.0);

	/** Texture */
	vec4 texColor = texture(u_Texture, v_TexCoords);

	/** Ambient Lighting */
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	/** Diffuse Lighting */
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightSrcPos - v_FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	/** Specular Lighting */
	float specularStrength = 0.5;
	int shininess = 32;
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	/** Phong Lighting Model */
	vec3 result = (ambient + diffuse + specular) * texColor.xyz;

	FragColor = vec4(result, 1.0);
}