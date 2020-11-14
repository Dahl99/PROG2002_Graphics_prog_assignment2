#version 430 core

/** Inputs */
in vec2 v_TexCoords;

/** Outputs */
out vec4 FragColor;

/** Uniforms */
layout(binding = 0) uniform sampler2D u_Texture;


void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoords);
	FragColor = texColor;
}