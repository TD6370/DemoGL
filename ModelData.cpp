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
using std::map;
using glm::vec3;
using glm::vec2;


//#using <system.dll>
//using namespace System::Diagnostics;


ModelData::ModelData() {

}

void ModelData::ConstructShaderProgramm(map<string, GLuint>& shaderPrograms) {

	bool isNew = true;
	string keyShaderPrograms = PathShaderVertex + "|" + PathShaderFrag;

	if (shaderPrograms.size() != 0) {
		map <string, GLuint> ::iterator it;
		it = shaderPrograms.find(keyShaderPrograms);
		isNew = it == shaderPrograms.end();
	}
	//TEST
	//isNew = true;
	if (isNew)
	{
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

		ShaderProgram = ProgramConfig(vertShader, fragShader);

		shaderPrograms.insert(std::pair< string, GLuint >(keyShaderPrograms, ShaderProgram));
	}
	else {
		ShaderProgram = shaderPrograms[keyShaderPrograms];
	}
}

void ModelData::LoadingTexture() {

	//------------------------------- Load texture
	DataImage = LoadBmp(PathTexture, &SizeImage.x, &SizeImage.y);
	if (PathTextureAtlas.size() != 0)
		DataImageAtlas = LoadBmp(PathTextureAtlas, &SizeImageAtlas.x, &SizeImageAtlas.y);

	if (DataImage == NULL)
	{
		std::cerr << "Could not read image file " << PathTexture << ". File does not exist." << std::endl;
		return;
	}
	//---------------------------------------- Load mesh
}

void ModelData::LoadModelData() {

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

	//if (!(PathTextureAtlas && !PathTextureAtlas[0]))
	if (PathTextureAtlas.size() != 0)
		TextureAtlas_ID = InitImage();

	BufferUV_ID = InitUV();
	BufferNormal_ID = InitNormals();
	BufferColor_ID = InitBuffer();
}

//---------------------------------------------------

void ModelData::InitUniform() {

	//ConfUniform = ConfigUniform(ShaderProgram);
	ConfUniform = new ConfigUniform(ShaderProgram);
	ConfUniform->Init();
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

void ModelData::SetModelInBuffer(vector<vec2>& uv, vector<vec3>& normals, bool isUpdateTexture)
{
	if (isUpdateTexture) {
		//SetImage(DataImage, WidthImage, HeightImage, Texture_ID);
		SetImage(DataImage, SizeImage.x, SizeImage.y, Texture_ID);
		if(TextureAtlas_ID != 666)
			SetImage(DataImageAtlas, SizeImageAtlas.x, SizeImageAtlas.y, TextureAtlas_ID, 1);
	}
		

	if (&uv == nullptr || uv.size() == 0)
		SetBufferUV(UV, BufferUV_ID);
	else
		SetBufferUV(uv, BufferUV_ID);

	if (normals.size() == 0)
		SetNormals(Normals, BufferNormal_ID);
	else
		SetNormals(normals, BufferNormal_ID);
}

void ModelData::SetTextureModel() {
	//SetImage(DataImage, WidthImage, HeightImage, Texture_ID);
	SetImage(DataImage, SizeImage.x, SizeImage.y, Texture_ID);
	if(TextureAtlas_ID != 666)
		SetImage(DataImageAtlas, SizeImageAtlas.x, SizeImageAtlas.y, TextureAtlas_ID, 1);
}

void ModelData::SetNormalsModel(vector<vec3>& normals) {
	if (normals.size() == 0)
		SetNormals(Normals, BufferNormal_ID);
	else
		SetNormals(normals, BufferNormal_ID);
}

void ModelData::SetBuffer(std::vector< glm::vec3>& buffer)
{
	
}

void ModelData::SetUV(vector< vec2 > uv) {
	SetBufferUV(uv, BufferUV_ID);
}

void ModelData::UpdateBufferUV() {
	SetBufferUV(UV, BufferUV_ID);
}

void ModelData::InitBase(map<string, GLuint>& shaderPrograms) {

	ConstructShaderProgramm(shaderPrograms);

	LoadingTexture();

	LoadModelData();

	SetModelInBuffer();

	InitUniform();

	//FillPlanes();
}


void ModelData::Init(map<string, GLuint>& shaderPrograms) {

	TypeName = FormatTypeName(typeid(this).name());

	//ConstructShaderProgramm(shaderPrograms);

	InitBase(shaderPrograms);
}

//ModelData ModelData::Clone() {
//	ModelData newModel = ModelData();
//	newModel.PathShaderVertex = PathShaderVertex;
//	newModel.PathShaderFrag = PathShaderFrag;
//	newModel.PathModel3D = PathModel3D;
//	newModel.PathTexture = PathTexture;
//	newModel.RadiusCollider = RadiusCollider;
//	newModel.Init(ShaderPrograms);
//	return newModel;
//}

//---------------- Model Frame

void  ModelFrame::Init(map<string, GLuint>& shaderPrograms) {

	TypeName = FormatTypeName(typeid(this).name());
	
	InitBase(shaderPrograms);
}

void ModelFrame::InitUniform() {

	ModelData::InitUniform();
	ConfUniform->InitBox();
}

void ModelFrame::SetBuffer(std::vector< glm::vec3>& buffer)
{
	if (buffer.size() != 0)
		GenBufferColors(buffer, BufferColor_ID);
}

void ModelFrame::SetWidth(GLfloat width) 
{
	ConfUniform->SetWidth(width);
}

void ModelFrame::SetHeight(GLfloat height) 
{
	ConfUniform->SetHeight(height);
}

void ModelFrame::SetPosMove(vec3 posMove)
{
	ConfUniform->SetPosMove(posMove);
}

void ModelFrame::SetPosMoveSize(vec3 posMoveS)
{
	ConfUniform->SetPosMoveSize(posMoveS);
}

//---------------------------- ModelTextBox

void  ModelTextBox::Init(map<string, GLuint>& shaderPrograms) {

	TypeName = FormatTypeName(typeid(this).name());

	InitBase(shaderPrograms);
}
//----------------------------------



