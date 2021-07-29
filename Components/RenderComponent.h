#pragma once

#include "../CoreSettings.h"

//#include "../ConfigBuffers.h"
//#include "../Shaders.h"
//#include "../LoaderModelObj.h"
//#include "../LoadBmp.h"
//#include "../OperationString.h"

#include <string>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

//OpenGL Mathematics
#include <glm/glm.hpp>

using glm::uvec2;

class ConfigUniform;
class ObjectData;

static vector<vec3> DEFAULT_VECTOR_V3;
static vector<vec2> DEFAULT_VECTOR_V2;

struct DataBuffers {
	GLuint VAO = EmptyID;
	GLuint VBO = EmptyID;
	GLuint Texture_ID = EmptyID;
	GLuint TextureAtlas_ID = EmptyID;
	GLuint BufferUV_ID = EmptyID;
	GLuint BufferNormal_ID = EmptyID;
	GLuint BufferColor_ID = EmptyID;
};

class RenderComponent
{
private:
	string PathShaderFolder = "./Shaders/";

	void SetModelInBuffer();

public:
	Material& m_material;

	Mesh& m_mesh;

	RenderComponent(Material& p_material, Mesh& p_mesh)
		: Buffers(), SizeImage(uvec2()), SizeImageAtlas(uvec2()), DataImage(), DataImageAtlas() , m_material(p_material), m_mesh(p_mesh)
	{
	};

	~RenderComponent() {};
	
	ConfigUniform* ConfUniform;

	DataBuffers Buffers;

	GLuint ShaderProgram = 0; 

	bool IsIndex = false;
	bool IsDebug = false;
	bool IsSquareModel = false;

	uvec2 SizeImage;
	uvec2 SizeImageAtlas;	
	unsigned char* DataImage;
	unsigned char* DataImageAtlas;	
	bool IsLoadedIntoMem_Texture = false;
	bool IsLoadedIntoMem_Normals = false;
	bool IsLoadedIntoMem_UV = false;
	bool IsLoadedIntoMem_Vertex = false;
	bool IsLoadedIntoMem_Buffer = false;

	void Clone(RenderComponent* renderModel);
	
	void Init(map<string, GLuint>& shaderPrograms);

	void ConstructShaderProgramm(map<string, GLuint>& shaderPrograms);

	void LoadingTexture();

	void LoadModelData();

	void InitUniform();

	void InitUniformBox();

	//----------------------

	void UpdateVAO();

	void UpdateCustomBuffer();

	void UpdateTexture();

	void UpdateNormals();

	void UpdateUV();

	void SetDataToShader(ObjectData& m_obj);

	//------------------ Animation info

	void SetWidth(GLfloat width);
	void SetHeight(GLfloat width);
	void SetPosMove(vec3 posMove);
	void SetPosMoveSize(vec3 posMoveS);

	//-------------------

	void ResetMem_UV();

	void ResetMem_Vertex();

	bool HasSquareModel();

	//-------------------
	void DebugUV(vector<vec2> list_uv);
	void DebugVec3(vector<vec3> list_v, string name);

};

