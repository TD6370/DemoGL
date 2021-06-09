//@@@
#include "ModelData.h"
//#include "BindHeader.h"

#include "ConfigBuffers.h"
#include "Shaders.h"
#include "ConfigUniformArg.h"
#include "LoaderModelObj.h"
#include "LoadBmp.h"
#include "GeometryLib.h"
#include "FileReader.h"

#include <string>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

//OpenGL Mathematics
#include <glm/glm.hpp>

using std::vector;
using glm::vec3;


//#using <system.dll>
//using namespace System::Diagnostics;


ModelData::ModelData() {

}

//struct StringHelper {
//	const char* p;
//	StringHelper(const std::string& s) : p(s.c_str()) {}
//	operator const char** () { return &p; }
//};

void ModelData::Init() {
	
	string pathShaderVertStr = std::string();
	pathShaderVertStr.append(PathShaderFolder);
	pathShaderVertStr.append(PathShaderVertex);
	const GLchar* pathShaderVert = pathShaderVertStr.c_str();
	//Load vertex shader
	GLuint vertShader = GenShader(pathShaderVert, true);

	string pathShaderFragStr = std::string();
	pathShaderFragStr.append(PathShaderFolder);
	pathShaderFragStr.append(PathShaderFrag);
	const GLchar* pathShaderFrag = pathShaderFragStr.c_str();

	

	//Load fragment shader
	GLuint fragShader = GenShader(pathShaderFrag, false);
	//----------------------------------------------------

	//----- test
	if (PathShaderVertex == "TextUI.vert") {
		GLuint ShaderProgram = ProgramConfig(vertShader, fragShader);
	}

	ShaderProgram = ProgramConfig(vertShader, fragShader);

	//------------------------------- Load texture
	DataImage = LoadBmp(PathTexture, &WidthImage, &HeightImage);
	if (DataImage == NULL)
	{
		std::cerr << "Could not read image file " << PathTexture << ". File does not exist." << std::endl;
		return;
	}
	//---------------------------------------- Load mesh

	bool isGetIndices = false;
	bool result = false;
	result = LoadModelObj(PathModel3D,
		Vertices,
		UV,
		Normals,
		Indices,
		isGetIndices);

	if (IsDebug) {
		DebugVec3(Normals, "Normals");
		DebugVec3(Vertices, "Vertices");
		DebugUV(UV);
	}

	if (!result)
	{
		fprintf(stderr, "Error load Model.obj");
	}
	TrianglesCount = Vertices.size();
	IndicesSize = Indices.size();
	//-----------------------------

	VAO = InitVAO();
	VBO = InitVBO();

	SetVAO();

	if (VAO == -1)
	{
		fprintf(stderr, "Error load VAO");
	}

	Texture_ID = InitImage();
	BufferUV_ID = InitUV();
	BufferNormal_ID = InitNormals();

	SetModelInBuffer();

	ConfigUniform();

	//FillPlanes();
}

//---------------------------------------------------

void ModelData::ConfigUniform() {

	//if(TypeMaterial == TypeModel::ModelBase)
		ConfUniform = ConfigUniformBase(ShaderProgram);
	//if (TypeMaterial == TypeModel::ModelGUI)
	//	ConfUniform = ConfigUniformTextGUI(ShaderProgram);
}

void ModelGUI::ConfigUniform() {

	ConfUniform = ConfigUniformTextGUI(ShaderProgram);
}

//---------------------------------------------------

void ModelData::DebugUV(vector<vec2> list_uv) {

	std::string data = "";
	for (const vec2 uv : list_uv)
	{
		//std::wcout << uv.x << "," << uv.y << std::endl;
		data += std::to_string(uv.x)  + "," + std::to_string(uv.y) + '\n';
	}
	WriteFile("", data, "UV");
}

void ModelData::DebugVec3(vector<vec3> list_v, std::string name) {

	std::string data = "";
	for (const vec3 v : list_v)
	{
		data += std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + '\n';
	}
	WriteFile("", data, name);
}

void ModelData::SetVAO() {
	GenVertexArrayObject(IsIndex,
		Vertices,
		Indices,
		VAO, VBO);
}

void ModelData::SetVAO(std::vector< glm::vec3 > vertices) {
	GenVertexArrayObject(IsIndex,
		vertices,
		Indices,
		VAO, VBO);
}

void ModelData::SetModelInBuffer(bool isUpdate)
{
	SetImage(DataImage, WidthImage, HeightImage, Texture_ID);
	SetBufferUV(UV, BufferUV_ID);
	SetNormals(Normals, BufferNormal_ID);
}

void ModelData::SetUV(vector< vec2 > uv) {
	SetBufferUV(uv, BufferUV_ID);
}

void ModelData::UpdateBufferUV() {
	SetBufferUV(UV, BufferUV_ID);
}

ModelData ModelData::Clone() {
	ModelData newModel = ModelData();
	newModel.PathShaderVertex = PathShaderVertex;
	newModel.PathShaderFrag = PathShaderFrag;
	newModel.PathModel3D = PathModel3D;
	newModel.PathTexture = PathTexture;
	newModel.RadiusCollider = RadiusCollider;
	newModel.Init();
	return newModel;
}


