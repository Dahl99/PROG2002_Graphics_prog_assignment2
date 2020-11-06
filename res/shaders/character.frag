#version 430 core

/** Inputs */
in vec2 v_TexCoords;

/** Outputs */
out vec4 FragColor;

/** Uniforms */
uniform sampler2D uTexture;

void main()
{
	vec4 texColor = texture(uTexture, v_TexCoords);
	FragColor = texColor;
}