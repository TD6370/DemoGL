#include "./RenderComponent.h"

#include "../ConfigBuffers.h"
#include "../ConfigUniformArg.h"
#include "../Shaders.h"
#include "../LoaderModelObj.h"
#include "../LoadBmp.h"
#include "../OperationString.h"
#include "../GeometryLib.h"
#include "../FileReader.h"
#include "../ObjectsTypes/ObjectData.h"

#include "../GeomertyShapes/ShapeBase.h"
#include "../GeomertyShapes/ShapeHexagon.h"
#include "../GeomertyShapes/ShapeSquare.h"


void RenderComponent::Clone(RenderComponent* renderModel) {

	//--- for object Null
	if (renderModel == nullptr)
		return;
	if (!renderModel)
		return;

	ShaderProgram = renderModel->ShaderProgram;
	ConfUniform = renderModel->ConfUniform;

	if (renderModel->HasSquareModel())
		InitUniformBox();
}


void RenderComponent::Init(map<string, GLuint>& shaderPrograms) {

	//TypeName = FormatTypeName(typeid(this).name());

	ConstructShaderProgramm(shaderPrograms);

	LoadingTexture();

	LoadModelData();

	SetModelInBuffer();

	InitUniform();
}

void RenderComponent::ConstructShaderProgramm(map<string, GLuint>& shaderPrograms) {

	bool isNew = true;
	string keyShaderPrograms = m_material.PathShaderVertex + "|" + m_material.PathShaderFrag;

	if (shaderPrograms.size() != 0) {
		map <string, GLuint> ::iterator it;
		it = shaderPrograms.find(keyShaderPrograms);
		isNew = it == shaderPrograms.end();
	}

	if (isNew)
	{
		string pathShaderVertStr = std::string();
		pathShaderVertStr.append(PathShaderFolder);
		pathShaderVertStr.append(m_material.PathShaderVertex);
		const GLchar* pathShaderVert = pathShaderVertStr.c_str();
		//Load vertex shader
		GLuint vertShader = GenShader(pathShaderVert, true);

		string pathShaderFragStr = std::string();
		pathShaderFragStr.append(PathShaderFolder);
		pathShaderFragStr.append(m_material.PathShaderFrag);
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

void RenderComponent::LoadingTexture() {

	//------------------------------- Load texture
	DataImage = LoadBmp(m_material.PathTexture, &SizeImage.x, &SizeImage.y);
	if (m_material.PathTextureAtlas.size() != 0)
		DataImageAtlas = LoadBmp(m_material.PathTextureAtlas, &SizeImageAtlas.x, &SizeImageAtlas.y);

	if (DataImage == NULL)
	{
		std::cerr << "Could not read image file " << m_material.PathTexture << ". File does not exist." << std::endl;
		return;
	}
}

void RenderComponent::LoadModelData() {

	bool isGetIndices = false;
	bool result = false;
	result = LoadModelObj(m_mesh.PathModel3D,
		m_mesh.Vertices,
		m_mesh.UV,
		m_mesh.Normals,
		m_mesh.Indices,
		isGetIndices);

	if (IsDebug) {
		DebugVec3(m_mesh.Normals, "Normals");
		DebugVec3(m_mesh.Vertices, "Vertices");
		DebugUV(m_mesh.UV);
	}

	if (!result)
	{
		fprintf(stderr, "Error load Model.obj");
	}
	m_mesh.TrianglesCount = m_mesh.Vertices.size();
	m_mesh.IndicesSize = m_mesh.Indices.size();
	//-----------------------------

	//SetVAO();
	UpdateVAO();

	if (Buffers.VAO == -1)
	{
		fprintf(stderr, "Error load VAO");
	}

	Buffers.Texture_ID = InitImage();

	if (m_material.PathTextureAtlas.size() != 0)
		Buffers.TextureAtlas_ID = InitImage();

	Buffers.BufferUV_ID = InitBuffer();
	Buffers.BufferNormal_ID = InitBuffer();
	Buffers.BufferColor_ID = InitBuffer();
}


void RenderComponent::InitUniform() {

	ConfUniform = new ConfigUniform(ShaderProgram);
	ConfUniform->Init();
}

void RenderComponent::InitUniformBox() {

	ConfUniform->InitBox();
}

void RenderComponent::SetModelInBuffer()
{
	UpdateTexture();

	SetBufferUV(m_mesh.UV, Buffers.BufferUV_ID, false);
	SetNormals(m_mesh.Normals, Buffers.BufferNormal_ID, false);

	//UpdateNormals();
	//UpdateUV();
}


void RenderComponent::UpdateVAO() {

	if (Buffers.VAO == EmptyID) { //TODO: In Render component
		Buffers.VAO = InitVAO();
		Buffers.VBO = InitBuffer();
	}

	GenVertexArrayObject(IsIndex,
		m_mesh.Vertices,
		m_mesh.Indices,
		Buffers.VAO, Buffers.VBO,
		IsLoadedIntoMem_Vertex);
	IsLoadedIntoMem_Vertex = true;
}

void RenderComponent::UpdateCustomBuffer()
{
	if (m_mesh.Buffer.size() != 0)
		GenBufferColors(m_mesh.Buffer, Buffers.BufferColor_ID);
}


void RenderComponent::UpdateTexture() {

	// -- Update texture - Default
	SetImage(DataImage, SizeImage.x, SizeImage.y, Buffers.Texture_ID, 0, IsLoadedIntoMem_Texture);
	if (Buffers.TextureAtlas_ID != EmptyID)
		SetImage(DataImageAtlas, SizeImageAtlas.x, SizeImageAtlas.y, Buffers.TextureAtlas_ID, 1, IsLoadedIntoMem_Texture);
	IsLoadedIntoMem_Texture = true;
}

void RenderComponent::UpdateNormals() {

	if (m_mesh.Normals.size() != 0) {

		if (Buffers.BufferNormal_ID == EmptyID) {
			Buffers.BufferNormal_ID = InitBuffer();
			IsLoadedIntoMem_Normals = false;
		}

		SetNormals(m_mesh.Normals, Buffers.BufferNormal_ID, IsLoadedIntoMem_Normals);
		IsLoadedIntoMem_Normals = true;
	}
}

void RenderComponent::UpdateUV() {

	if (Buffers.BufferUV_ID == EmptyID) {
		Buffers.BufferUV_ID = InitBuffer();
		IsLoadedIntoMem_UV = false;
	}

	// Default
	SetBufferUV(m_mesh.UV, Buffers.BufferUV_ID, IsLoadedIntoMem_UV);
	IsLoadedIntoMem_UV = true;
}

void RenderComponent::SetDataToShader(ObjectData& obj) {

	if (obj.IsGUI)
	{
		if (obj.ActionObjectCurrent == ActionObject::Transforming ||
			obj.ActionObjectCurrent == ActionObject::Moving ||
			obj.ActionObjectCurrent == ActionObject::Woking) {

			float width = obj.GetShapeSquare()->WidthFactor;
			float height = obj.GetShapeSquare()->HeightFactor;
			vec3 posMove = obj.GetShapeSquare()->PosMoveFactor;
			vec3 posMoveSize = obj.GetShapeSquare()->PosMoveSizeFactor;
			if (width < 0)
				return;

			SetWidth(width);
			SetHeight(height);
			SetPosMove(posMove);
			SetPosMoveSize(posMoveSize);
		}
	}
}

void RenderComponent::SetWidth(GLfloat width)
{
	ConfUniform->SetWidth(width);
}

void RenderComponent::SetHeight(GLfloat height)
{
	ConfUniform->SetHeight(height);
}

void RenderComponent::SetPosMove(vec3 posMove)
{
	ConfUniform->SetPosMove(posMove);
}

void RenderComponent::SetPosMoveSize(vec3 posMoveS)
{
	ConfUniform->SetPosMoveSize(posMoveS);
}


void RenderComponent::ResetMem_UV() {
	IsLoadedIntoMem_UV = false;
}

void RenderComponent::ResetMem_Vertex() {
	IsLoadedIntoMem_Vertex = false;
}

bool RenderComponent::HasSquareModel() {
	return m_mesh.IsSquareModel;
}



void RenderComponent::DebugUV(vector<vec2> list_uv) {

	std::string data = "";
	for (const vec2 uv : list_uv)
	{
		//std::wcout << uv.x << "," << uv.y << std::endl;
		data += std::to_string(uv.x) + "," + std::to_string(uv.y) + '\n';
	}
	WriteFile("", data, "UV");
}

void RenderComponent::DebugVec3(vector<vec3> list_v, std::string name) {

	std::string data = "";
	for (const vec3 v : list_v)
	{
		data += std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + '\n';
	}
	WriteFile("", data, name);
}
