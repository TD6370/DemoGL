#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 posNormal;

out vec3 fragmentColor;
out vec2 UV;
out float fragParamCase;
out float fragTime;

uniform mat4 MVP;
uniform mat4 VIEW;
uniform mat4 MODEL;

uniform vec3 setColor; 
uniform float paramCase;
uniform mat4 transform;
uniform float fTime;

void main()
{
	gl_Position = MVP * transform *vec4(position, 1.0);

	// ---- OUT 
	fragmentColor = setColor; 
	UV = vertexUV;
	fragParamCase = paramCase;
	fragTime = fTime;
}