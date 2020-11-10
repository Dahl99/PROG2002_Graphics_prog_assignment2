#version 430 core

/** Inputs */
in vec2 v_TexCoords;

/** Outputs */
out vec4 FragColor;

/** Uniforms */
layout(binding = 0) uniform sampler2D u_Texture0;
layout(binding = 1) uniform sampler2D u_Texture1;
uniform int numTex;

void main()
{
	vec4 texColor;

	if(numTex == 0)
	{
		texColor = texture(u_Texture0, v_TexCoords);
	}
	else if(numTex == 1)
	{
		texColor = texture(u_Texture1, v_TexCoords);
	}

	FragColor = texColor;
}