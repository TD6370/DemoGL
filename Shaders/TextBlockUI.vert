#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 posNormal;

out vec3 fragmentColor;
out vec2 UV;
out float fragParamCase;
out float fragTime;
flat out int numSymbol;

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
	//fragmentColor = vertexColor;
	fragmentColor = setColor; 
	UV = vertexUV;
	fragParamCase = paramCase;
	fragTime = fTime;
	//numSymbol = 1;//vertexUV.x;
	//numSymbol = min(33,int(position.x));
	//numSymbol = min(33,max(0,int(abs(gl_Position.x + gl_Position.y))));
	numSymbol = min(33,max(0,int(abs(sin(fragTime)))));
}