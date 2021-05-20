#pragma once

//#include "BindHeader.h"
//--------------
#include "ConfigBuffers.h"
#include "Shaders.h"
#include "ConfigUniformArg.h"
#include "LoaderModelObj.h"
#include "LoadBmp.h"

#include <string>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

//OpenGL Mathematics
#include <glm/glm.hpp>

using std::vector;
using glm::vec2;
using std::string;

class Plane;

class ModelData {

public:
	string Name;

	const GLchar* PathShaderVertex = "basic.vert";
	const GLchar* PathShaderFrag = "basic.frag";
	const char* PathTexture = "./Textures/testTexture.bmp";
	const char* PathModel3D = "./Models3D/monkey.obj";

	ConfigUniform ConfUniform;

	GLint TrianglesCount = 0;
	GLint IndicesSize = 0;
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint ShaderProgram = 0;
	bool IsIndex = false;
	bool IsDebug = false;
	//-------------------
	std::vector< glm::vec2 > UV;
	//std::vector< glm::vec2 > StartUV;
	std::vector< glm::vec3 > Normals;
	std::vector< glm::vec3 > Vertices;
	std::vector<unsigned int> Indices;
	//std::vector<Plane>* Planes;

	unsigned int WidthImage;
	unsigned int HeightImage;

	unsigned char* DataImage;
	//------------------------
	GLuint Texture_ID;
	GLuint BufferUV_ID;
	GLuint BufferNormal_ID;

	float RadiusCollider = 5;

	bool IsCubeModel = false;

	ModelData();

	void Init();

	void SetVAO();
	void SetVAO(std::vector< glm::vec3 > vertices);

	void SetModelInBuffer(bool isUpdate = true);

	void SetUV(vector< vec2 > uv);
	void UpdateBufferUV();

	void DebugUV(vector<vec2> list_uv);
	void DebugVec3(vector<vec3> list_v, std::string name);
	

	ModelData Clone();

	
};
