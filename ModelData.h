#pragma once

#include "ConfigBuffers.h"
#include "Shaders.h"
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
using std::map;

static vector<vec3> DEFAULT_VECTOR_V3;
static vector<vec2> DEFAULT_VECTOR_V2;

class Plane;
class ModelFrame; //!!!
class ConfigUniform;
class RenderComponent;

class ModelData {

public:
	
	string Name;
	string TypeName;
	string PathShaderFolder = "./Shaders/";
	
	//================== OBJECT INFO:
	////-----  Material data	//TODO: ????
	//vector <vec2> TextureUV;
	//vector <vec3> Buffer;
	//vec3 Color = vec3(0);

	//// ---- Mesh data	//TODO: ????
	//vector <vec3> Vertices;
	//vector <vec3> Normals;
	//GLint TrianglesCount = 0;
	//==================

	Material MaterialData;
	//---- Material Data
	string PathShaderVertex = "basic.vert";
	string PathShaderFrag = "basic.frag";
	string PathTexture = "./Textures/testTexture.bmp";
	string PathTextureAtlas = "";	//@**

	Mesh MeshData;
	//---- Mesh data
	const char* PathModel3D = "./Models3D/monkey.obj";
	GLint TrianglesCount = 0;
	GLint IndicesSize = 0;
	std::vector< glm::vec2 > UV;
	std::vector< glm::vec3 > Normals;
	std::vector< glm::vec3 > Vertices;
	std::vector<unsigned int> Indices;
	float RadiusCollider = 5; // TODO: ????
	bool IsSquareModel = false; // TODO: ????

	//==================== In Render component ------ OBJECT INFO:
	/*GLuint VAO = EmptyID;
	GLuint VBO = EmptyID;
	GLuint BufferUV_ID = EmptyID;
	GLuint BufferNormal_ID = EmptyID;
	bool IsLoadedIntoMem_Vertex = false;
	bool IsLoadedIntoMem_Normals = false;
	bool IsLoadedIntoMem_UV = false;*/

	//RenderComponent Render;
	RenderComponent* Render;

	//---- Render component TODO:!!!
	ConfigUniform* ConfUniform;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint ShaderProgram = -1;// 0;
	bool IsIndex = false;
	bool IsDebug = false;
	glm::uvec2 SizeImage;	
	glm::uvec2 SizeImageAtlas;	//@**
	unsigned char* DataImage;
	unsigned char* DataImageAtlas;	//@**
	bool IsLoadedIntoMem_Texture = false;
	bool IsLoadedIntoMem_Normals = false;
	bool IsLoadedIntoMem_UV = false;
	bool IsLoadedIntoMem_Vertex = false;
	bool IsLoadedIntoMem_Buffer = false;
	GLuint Texture_ID;
	GLuint TextureAtlas_ID = EmptyID;	//@**
	GLuint BufferUV_ID;
	GLuint BufferNormal_ID;
	GLuint BufferColor_ID;
	//------------------------

	ModelData() {};
	//ModelData() : Render(MaterialData, MeshData) {};

	void virtual InitBase(map<string, GLuint>& shaderPrograms);

	void virtual Init(map<string, GLuint>& shaderPrograms);

	void ConstructShaderProgramm(map<string, GLuint>& shaderPrograms); //---  RENDER

	void virtual LoadingTexture(); //---  RENDER

	void LoadModelData(); //---  RENDER

	void virtual InitUniform(); //---  RENDER

	void SetVAO(); //---  RENDER

	//void SetVAO(vector<vec3> vertices);
	//void SetVAO(vector<vec3> vertices, GLuint VAO, GLuint VBO, bool isLoadedIntoMem);
	void SetVAO(vector<vec3>& vertices, GLuint VAO, GLuint VBO, bool isLoadedIntoMem); //---  RENDER

	//---  RENDER
	void virtual SetModelInBuffer(vector<vec2>& uv = DEFAULT_VECTOR_V2,
									vector<vec3>& normals = DEFAULT_VECTOR_V3, 
										bool isUpdateTexture = true,
											GLuint bufferUV_ID = EmptyID,
												GLuint bufferNormal_ID = EmptyID,
													bool p_isLoadedIntoMem_UV = false, bool p_isLoadedIntoMem_Normals = false);

	//---  RENDER
	void virtual SetBuffer(vector<vec3>& buffer = DEFAULT_VECTOR_V3);

	//---  RENDER
	void SetTextureModel();

	//---  RENDER
	void SetNormalsModel(vector<vec3>& normals = DEFAULT_VECTOR_V3, GLuint bufferNormal_ID = 66666);
		
	//---  RENDER
	void SetUV(vector< vec2 >& uv, GLuint p_bufferUV_ID, bool isLoadedIntoMem);
	void UpdateBufferUV();

	void DebugUV(vector<vec2> list_uv);
	void DebugVec3(vector<vec3> list_v, string name);
	
	//ModelData Clone();
};

class ModelFrame : public ModelData {

public:

	ModelFrame() :ModelData() {
	};

	void Init(map<string, GLuint>& shaderPrograms);

	void InitUniform();

	void SetBuffer(vector<vec3>& buffer = DEFAULT_VECTOR_V3);

	void SetWidth(GLfloat width);
	void SetHeight(GLfloat width);
	void SetPosMove(vec3 posMove);
	void SetPosMoveSize(vec3 posMoveS);
	
};

class ModelTextBox : public ModelFrame {

public:
	
	ModelTextBox():ModelFrame() {
	};

	void Init(map<string, GLuint>& shaderPrograms);
};

class ModelEditBox : public ModelTextBox {

public:

	ModelEditBox() :ModelTextBox() {
	};
	
};

