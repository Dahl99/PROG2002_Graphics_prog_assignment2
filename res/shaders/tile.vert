#version 430 core

/** Inputs */
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

/** Outputs */
out vec4 VertexColor;

/** Uniforms */
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * position;
	VertexColor = color;
}