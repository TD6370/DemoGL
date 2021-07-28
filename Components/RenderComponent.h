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

	Material& m_material;

	Mesh& m_mesh;

public:

	RenderComponent(Material& p_material, Mesh& p_mesh) 
		: Buffers(), SizeImage(uvec2()), SizeImageAtlas(uvec2()), DataImage(), DataImageAtlas(), m_material(p_material), m_mesh(p_mesh)
	{
	};

	~RenderComponent() {};
	
	ConfigUniform* ConfUniform;

	DataBuffers Buffers;

	GLuint ShaderProgram = 0; // EmptyID; // -1;// 0;

	bool IsIndex = false;
	bool IsDebug = false;
	bool IsGUI = false;

	uvec2 SizeImage;
	uvec2 SizeImageAtlas;	
	unsigned char* DataImage;
	unsigned char* DataImageAtlas;	
	bool IsLoadedIntoMem_Texture = false;
	bool IsLoadedIntoMem_Normals = false;
	bool IsLoadedIntoMem_UV = false;
	bool IsLoadedIntoMem_Vertex = false;
	bool IsLoadedIntoMem_Buffer = false;


	void InitBase(map<string, GLuint>& shaderPrograms);

	void Init(map<string, GLuint>& shaderPrograms);

	void ConstructShaderProgramm(map<string, GLuint>& shaderPrograms);

	void LoadingTexture();

	void LoadModelData();

	void InitUniform();

	void SetVAO();

	void SetVAO(vector<vec3>& vertices, GLuint VAO, GLuint VBO, bool isLoadedIntoMem);

	void SetModelInBuffer(vector<vec2>& uv = DEFAULT_VECTOR_V2,
		vector<vec3>& normals = DEFAULT_VECTOR_V3,
		bool isUpdateTexture = true,
		GLuint bufferUV_ID = EmptyID,
		GLuint bufferNormal_ID = EmptyID,
		bool p_isLoadedIntoMem_UV = false, bool p_isLoadedIntoMem_Normals = false);



	//----------------------
	void SetBuffer(vector<vec3>& buffer = DEFAULT_VECTOR_V3);
	/*
		if (buffer.size() != 0)
			GenBufferColors(buffer, BufferColor_ID);


	*/
	/*void ObjectTextBox::SetDataToShader() {

		ObjectGUI::SetDataToShader();

		ModelPtr->SetBuffer(Buffer); //----<<<

		if (!isInitSlotsMessage) {
			isInitSlotsMessage = true;

			ModelPtr->SetModelInBuffer(TextureUV, Normals, false); //TODO: delete
		}
	}
	*/

	void SetTextureModel();

	void SetNormalsModel(vector<vec3>& normals = DEFAULT_VECTOR_V3, GLuint bufferNormal_ID = 66666);

	void SetUV(vector< vec2 >& uv, GLuint p_bufferUV_ID, bool isLoadedIntoMem);
	void UpdateBufferUV();

	void DebugUV(vector<vec2> list_uv);
	void DebugVec3(vector<vec3> list_v, string name);

	//------------------ Animation info

	void SetWidth(GLfloat width);
	void SetHeight(GLfloat width);
	void SetPosMove(vec3 posMove);
	void SetPosMoveSize(vec3 posMoveS);


};

