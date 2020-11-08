#version 430 core

/** Inputs */
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

/** Outputs */
out vec2 v_TexCoords;

/** Uniforms */
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
	v_TexCoords = texCoords;
}