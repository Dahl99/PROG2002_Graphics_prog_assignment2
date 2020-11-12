#version 430 core

/** Inputs */
in vec2 v_TexCoords;

/** Outputs */
out vec4 FragColor;

/** Uniforms */
layout(binding = 0) uniform sampler2D u_Texture0;
layout(binding = 1) uniform sampler2D u_Texture1;
layout(binding = 2) uniform sampler2D u_Texture2;
layout(binding = 3) uniform sampler2D u_Texture3;
layout(binding = 4) uniform sampler2D u_Texture4;
layout(binding = 5) uniform sampler2D u_Texture5;
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
	else if(numTex == 2)
	{
		texColor = texture(u_Texture2, v_TexCoords);
	}
	else if(numTex == 3)
	{
		texColor = texture(u_Texture3, v_TexCoords);
	}
	else if(numTex == 4)
	{
		texColor = texture(u_Texture4, v_TexCoords);
	}
	else if(numTex == 5)
	{
		texColor = texture(u_Texture5, v_TexCoords);
	}

	FragColor = texColor;
}