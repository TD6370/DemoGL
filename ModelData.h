#pragma once

//#include "BindHeader.h"
//--------------
#include "ConfigBuffers.h"
#include "Shaders.h"
#include "ConfigUniformArg.h"
#include "LoaderModelObj.h"
#include "LoadBmp.h"
#include "CoreSettings.h"
#include "OperationString.h"

#include <string>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

//OpenGL Mathematics
#include <glm/glm.hpp>

using std::vector;
using glm::vec2;
using glm::vec3;
using std::string;
using std::shared_ptr;

static vector<vec3> DEFAULT_VECTOR_V3;
static vector<vec2> DEFAULT_VECTOR_V2;

class Plane;
class ModelFrame; //!!!

class ModelData {

public:
	string Name;
	string PathShaderFolder = "./Shaders/";
	string PathShaderVertex = "basic.vert";
	string PathShaderFrag = "basic.frag";
	const char* PathTexture = "./Textures/testTexture.bmp";
	const char* PathModel3D = "./Models3D/monkey.obj";
	string TypeName;

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
	std::vector< glm::vec3 > Normals;
	std::vector< glm::vec3 > Vertices;
	std::vector<unsigned int> Indices;

	unsigned int WidthImage;
	unsigned int HeightImage;

	unsigned char* DataImage;
	//------------------------
	GLuint Texture_ID;
	GLuint BufferUV_ID;
	GLuint BufferNormal_ID;
	GLuint BufferColor_ID;

	float RadiusCollider = 5;

	bool IsSquareModel = false;

	ModelData();

	void virtual InitBase();

	void virtual Init();

	void ConstructShaderProgramm();

	void virtual LoadingTexture();

	void LoadModelData();

	void virtual InitUniform();

	void SetVAO();
	void SetVAO(vector<vec3> vertices);

	void virtual SetModelInBuffer(vector<vec2>& uv = DEFAULT_VECTOR_V2,
									vector<vec3>& normals = DEFAULT_VECTOR_V3);

	void virtual SetBuffer(vector<vec3>& buffer = DEFAULT_VECTOR_V3);

	void SetUV(vector< vec2 > uv);
	void UpdateBufferUV();

	void DebugUV(vector<vec2> list_uv);
	void DebugVec3(vector<vec3> list_v, string name);
	
	ModelData Clone();
};

class ModelFrame : public ModelData {

public:

	ModelFrame() :ModelData() {
	};

	void Init();

	void InitUniform();

	void SetBuffer(vector<vec3>& buffer = DEFAULT_VECTOR_V3);

	void SetWidth(GLfloat width);
	void SetHeight(GLfloat width);

};

class ModelTextBox : public ModelFrame {

public:
	
	ModelTextBox():ModelFrame() {
	};

	void Init();
};

