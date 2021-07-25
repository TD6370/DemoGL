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
	VBO = InitBuffer();

	SetVAO();

	if (VAO == -1)
	{
		fprintf(stderr, "Error load VAO");
	}

	Texture_ID = InitImage();

	//if (!(PathTextureAtlas && !PathTextureAtlas[0]))
	if (PathTextureAtlas.size() != 0)
		TextureAtlas_ID = InitImage();

	BufferUV_ID = InitBuffer();
	BufferNormal_ID = InitBuffer();
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
		VAO, VBO, 
		IsLoadedIntoMem_Vertex);
	IsLoadedIntoMem_Vertex = true;
}

void ModelData::SetVAO(std::vector< glm::vec3 > vertices, GLuint p_VAO, GLuint p_VBO, bool isLoadedIntoMem) {
	GenVertexArrayObject(IsIndex,
		vertices,
		Indices,
		p_VAO, p_VBO, isLoadedIntoMem);
}

//TODO: delete
void ModelData::SetModelInBuffer(vector<vec2>& uv, vector<vec3>& normals, bool isUpdateTexture, GLuint p_bufferUV_ID, GLuint p_bufferNormal_ID, 
	bool p_isLoadedIntoMem_UV, bool p_isLoadedIntoMem_Normals)
{
	// -- Update texture
	if (isUpdateTexture) {
		SetImage(DataImage, SizeImage.x, SizeImage.y, Texture_ID, 0, IsLoadedIntoMem_Texture);
		if(TextureAtlas_ID != EmptyID)
			SetImage(DataImageAtlas, SizeImageAtlas.x, SizeImageAtlas.y, TextureAtlas_ID, 1, IsLoadedIntoMem_Texture);
		IsLoadedIntoMem_Texture = true;
	}

	// -- Update UV
	if (&uv == nullptr || uv.size() == 0)
		SetBufferUV(UV, BufferUV_ID, IsLoadedIntoMem_UV); // Default
	else {
		if (p_bufferUV_ID == EmptyID) 
			p_bufferUV_ID = BufferUV_ID;
		SetBufferUV(uv, p_bufferUV_ID, p_isLoadedIntoMem_UV);
	}

	// -- Update Normals
	if (normals.size() == 0)
		SetNormals(Normals, BufferNormal_ID, IsLoadedIntoMem_Normals); // Default
	else {
		if(p_bufferNormal_ID == EmptyID)
			p_bufferNormal_ID = BufferNormal_ID;
		SetNormals(normals, p_bufferNormal_ID, p_isLoadedIntoMem_Normals);
	}
}

void ModelData::SetTextureModel() {

	// -- Update texture - Default
	SetImage(DataImage, SizeImage.x, SizeImage.y, Texture_ID, 0, IsLoadedIntoMem_Texture);
	if(TextureAtlas_ID != EmptyID)
		SetImage(DataImageAtlas, SizeImageAtlas.x, SizeImageAtlas.y, TextureAtlas_ID, 1, IsLoadedIntoMem_Texture);
	IsLoadedIntoMem_Texture = true;
}

void ModelData::SetNormalsModel(vector<vec3>& normals, GLuint p_bufferNormal_ID) {

	bool _isLoadedIntoMem_Normals = IsLoadedIntoMem_Normals;
	if (p_bufferNormal_ID == EmptyID) // Default
		p_bufferNormal_ID = BufferNormal_ID;
	else // Object
		_isLoadedIntoMem_Normals = false;

	if (normals.size() == 0)
		SetNormals(Normals, p_bufferNormal_ID, _isLoadedIntoMem_Normals);
	else
		SetNormals(normals, p_bufferNormal_ID, false);

	IsLoadedIntoMem_Normals = true;
}

void ModelData::SetBuffer(std::vector< glm::vec3>& buffer)
{
	
}

void ModelData::SetUV(vector< vec2 > uv, GLuint p_bufferUV_ID, bool isLoadedIntoMem) {

	SetBufferUV(uv, p_bufferUV_ID, isLoadedIntoMem);
}

void ModelData::UpdateBufferUV() {
	// Default
	SetBufferUV(UV, BufferUV_ID, IsLoadedIntoMem_UV);
	IsLoadedIntoMem_UV = true;
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



