#version 430 core

/** Inputs */
layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texCoords;

/** Outputs */
out vec2 v_TexCoords;

/** Uniforms */
uniform mat4 u_Projection;
uniform mat4 u_Model;
uniform vec2 posTex;

void main()
{
	gl_Position = u_Projection * u_Model * position;
	v_TexCoords = texCoords + posTex;
}