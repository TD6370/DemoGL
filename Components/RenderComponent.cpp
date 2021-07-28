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



void RenderComponent::InitBase(map<string, GLuint>& shaderPrograms) {

	ConstructShaderProgramm(shaderPrograms);

	LoadingTexture();

	LoadModelData();

	SetModelInBuffer();

	InitUniform();
}

void RenderComponent::Init(map<string, GLuint>& shaderPrograms) {

	//TypeName = FormatTypeName(typeid(this).name());
	InitBase(shaderPrograms);
}

void RenderComponent::ConstructShaderProgramm(map<string, GLuint>& shaderPrograms) {

	bool isNew = true;
	string keyShaderPrograms = m_material.PathShaderVertex + "|" + m_material.PathShaderFrag;

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

	Buffers.VAO = InitVAO();
	Buffers.VBO = InitBuffer();

	SetVAO();

	if (Buffers.VAO == -1)
	{
		fprintf(stderr, "Error load VAO");
	}

	Buffers.Texture_ID = InitImage();

	//if (!(PathTextureAtlas && !PathTextureAtlas[0]))
	if (m_material.PathTextureAtlas.size() != 0)
		Buffers.TextureAtlas_ID = InitImage();

	Buffers.BufferUV_ID = InitBuffer();
	Buffers.BufferNormal_ID = InitBuffer();
	Buffers.BufferColor_ID = InitBuffer();
}


void RenderComponent::InitUniform() {

	//ConfUniform = ConfigUniform(ShaderProgram);
	//ConfUniform.Init();

	ConfUniform = new ConfigUniform(ShaderProgram);
	ConfUniform->Init();

	//void ModelFrame::InitUniform() {
	if(IsGUI)
		ConfUniform->InitBox();
	//}
}

void RenderComponent::SetVAO() {
	GenVertexArrayObject(IsIndex,
		m_mesh.Vertices,
		m_mesh.Indices,
		Buffers.VAO, Buffers.VBO,
		IsLoadedIntoMem_Vertex);
	IsLoadedIntoMem_Vertex = true;
}

void RenderComponent::SetVAO(std::vector< glm::vec3 >& vertices, GLuint p_VAO, GLuint p_VBO, bool isLoadedIntoMem) {
	GenVertexArrayObject(IsIndex,
		vertices,
		m_mesh.Indices,
		p_VAO, p_VBO, isLoadedIntoMem);
}

//TODO: delete
void RenderComponent::SetModelInBuffer(vector<vec2>& uv, vector<vec3>& normals, bool isUpdateTexture, GLuint p_bufferUV_ID, GLuint p_bufferNormal_ID,
	bool p_isLoadedIntoMem_UV, bool p_isLoadedIntoMem_Normals)
{
	// -- Update texture
	if (isUpdateTexture) {
		SetImage(DataImage, SizeImage.x, SizeImage.y, Buffers.Texture_ID, 0, IsLoadedIntoMem_Texture);
		if (Buffers.TextureAtlas_ID != EmptyID)
			SetImage(DataImageAtlas, SizeImageAtlas.x, SizeImageAtlas.y, Buffers.TextureAtlas_ID, 1, IsLoadedIntoMem_Texture);
		IsLoadedIntoMem_Texture = true;
	}

	// -- Update UV
	if (&uv == nullptr || uv.size() == 0)
		SetBufferUV(m_mesh.UV, Buffers.BufferUV_ID, IsLoadedIntoMem_UV); // Default
	else {
		if (p_bufferUV_ID == EmptyID)
			p_bufferUV_ID = Buffers.BufferUV_ID;
		SetBufferUV(uv, p_bufferUV_ID, p_isLoadedIntoMem_UV);
	}

	// -- Update Normals
	if (normals.size() == 0)
		SetNormals(m_mesh.Normals, Buffers.BufferNormal_ID, IsLoadedIntoMem_Normals); // Default
	else {
		if (p_bufferNormal_ID == EmptyID)
			p_bufferNormal_ID = Buffers.BufferNormal_ID;
		SetNormals(normals, p_bufferNormal_ID, p_isLoadedIntoMem_Normals);
	}
}

void RenderComponent::SetBuffer(std::vector< glm::vec3>& buffer)
{
	if (buffer.size() != 0)
		GenBufferColors(buffer, Buffers.BufferColor_ID);
}


void RenderComponent::SetTextureModel() {

	// -- Update texture - Default
	SetImage(DataImage, SizeImage.x, SizeImage.y, Buffers.Texture_ID, 0, IsLoadedIntoMem_Texture);
	if (Buffers.TextureAtlas_ID != EmptyID)
		SetImage(DataImageAtlas, SizeImageAtlas.x, SizeImageAtlas.y, Buffers.TextureAtlas_ID, 1, IsLoadedIntoMem_Texture);
	IsLoadedIntoMem_Texture = true;
}

void RenderComponent::SetNormalsModel(vector<vec3>& normals, GLuint p_bufferNormal_ID) {

	bool _isLoadedIntoMem_Normals = IsLoadedIntoMem_Normals;
	if (p_bufferNormal_ID == EmptyID) // Default
		p_bufferNormal_ID = Buffers.BufferNormal_ID;
	else // Object
		_isLoadedIntoMem_Normals = false;

	if (normals.size() == 0)
		SetNormals(m_mesh.Normals, p_bufferNormal_ID, _isLoadedIntoMem_Normals);
	else
		SetNormals(normals, p_bufferNormal_ID, false);

	IsLoadedIntoMem_Normals = true;
}

void RenderComponent::SetUV(vector< vec2 >& uv, GLuint p_bufferUV_ID, bool isLoadedIntoMem) {

	SetBufferUV(uv, p_bufferUV_ID, isLoadedIntoMem);
}

void RenderComponent::UpdateBufferUV() {
	// Default
	SetBufferUV(m_mesh.UV, Buffers.BufferUV_ID, IsLoadedIntoMem_UV);
	IsLoadedIntoMem_UV = true;
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




void RenderComponent::SetDataToShader(ObjectData& obj) {

	if(obj.IsGUI)
	{
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

	bool isTextBox;
	if (obj.TypeObj == TextBox)
	{
		SetBuffer(m_mesh.Buffer);

		//if (!isInitSlotsMessage) {
		//	isInitSlotsMessage = true;
		if (!IsLoadedIntoMem_UV) {
			IsLoadedIntoMem_UV = true;

			SetModelInBuffer(m_mesh.UV, m_mesh.Normals, false); //TODO: delete
		}
	}
}