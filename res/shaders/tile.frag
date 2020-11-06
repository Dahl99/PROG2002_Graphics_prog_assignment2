#version 430 core

/** Inputs */
in vec4 VertexColor;

/** Outputs */
out vec4 FragColor;

void main()
{
	FragColor = VertexColor;
}