#include "ShapeBase.h"


#include "../CoreSettings.h"
#include "../ModelData.h"
#include "../GeometryLib.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../CreatorModelData.h"
#include "../WorldCollision.h" //--<< #include "CreatorModelData.h"  
#include "../ObjectsTypes/ObjectPhysic.h"
#include "../ObjectsTypes/ObjectGUI.h"	//##$$5.
#include "../Components/RenderComponent.h"

ShapeBase::ShapeBase() {

}

ShapeBase::~ShapeBase() {

}

//void ShapeBase::UpdateShapeInfo(ObjectData* obj)
void ShapeBase::UpdateShapeInfo(ObjectPhysic* obj)
{
	m_obj = obj;
	m_objPhysic = obj;
}

void ShapeBase::UpdateShapeInfo(ObjectData* obj)
{
	m_obj = obj;
}

void ShapeBase::UpdateShapeInfo(ObjectGUI* obj)
{
	m_obj = obj;
	m_objGUI = obj;
}


vec3 ShapeBase::GetVertexWorldPosition(int indVertex)
{
	ObjectData* obj	= m_obj;

	std::vector<vec3 > verticesModel = obj->GetVertices();
	if (verticesModel.size() - 1 < indVertex)
		return vec3(-1);

	vec3 posNorm = verticesModel[indVertex];

	return ToWorldPosition(posNorm);
}

vec3 ShapeBase::ToWorldPosition(vec3 pos) {

	ObjectData* obj = m_obj;

	glm::mat4 trans = Transform(1, 0, false,
		glm::mat4(1.0f),
		obj->Postranslate,
		obj->TranslateAngle,
		obj->Size);
	glm::vec3 worldPos = vec3(trans * vec4(pos, 1));
	return worldPos;
}

string ShapeBase::GetKeySectorPolygon(bool isNewPosition) {
	
	ObjectData* obj = m_obj;

	vec3 pos = vec3();
	if (isNewPosition)
		pos = obj->NewPostranslate;
	else
		pos = obj->Postranslate;

	int x_sector = pos.x / obj->EngineData->Clusters->SectorSizePlane;	//!!!!!!
	int z_sector = pos.z / obj->EngineData->Clusters->SectorSizePlane;
	return std::to_string(x_sector) + "_" + std::to_string(z_sector);
}


void ShapeBase::FillPlanes()
{
	ObjectData* obj = m_obj;

	int indVert = 0;
	int indexPlane = 0;

	glm::mat4 trans = Transform(1, 0, false,
		glm::mat4(1.0f),
		obj->Postranslate,
		obj->TranslateAngle,
		obj->Size);

	while (indVert < obj->GetVertices().size())
	{
		vec3 vertexNormA = obj->GetVertices()[indVert++];
		vec3 vertexNormB = obj->GetVertices()[indVert++];
		vec3 vertexNormC = obj->GetVertices()[indVert++];

		Plane plane(vertexNormA, vertexNormB, vertexNormC, trans);
		plane.Index = indexPlane++;
		Planes.push_back(std::make_unique<Plane>(plane));
	}
}

void ShapeBase::FillVertextBox()
{
	ObjectPhysic* obj = m_objPhysic;

	std::vector<vec3> CubeVectors;
	int indexPlaneT = 0;
	int indexPlaneB = 0;
	int indVert = 0;

	BottomVectors.clear();
	TopVectors.clear();

	while (indVert < obj->GetVertices().size())
	{
		vec3 vertexNext = obj->GetVertices()[indVert];
		indVert++;
		bool isExist = std::find(CubeVectors.begin(), CubeVectors.end(), vertexNext) != CubeVectors.end();
		if (isExist)
			continue;

		CubeVectors.push_back(vertexNext);
		if (vertexNext.y < 0) {

			//TODO:	BottomVectors -> Shape
			BottomVectors.insert(std::pair<int, vec3>(indexPlaneB, vertexNext));
			indexPlaneB++;
		}
		else {
			TopVectors.insert(std::pair<int, vec3>(indexPlaneT, vertexNext));
			indexPlaneT++;
		}
	}
}


//=========================
float ShapeBase::GetLineLenght(int index) {

	ObjectPhysic* obj = m_objPhysic;

	vec4 line = GetLine(index);
	float x1 = line.x;
	float y1 = line.y;
	float x2 = line.z;
	float y2 = line.w;
	float lenghtLine = glm::distance(vec2(x1, y1), vec2(x2, y2)); // lenght wall
	return lenghtLine;
}

vec4 ShapeBase::GetLine(int index) {

	ObjectPhysic* obj = m_objPhysic;

	vec3 pos1 = BottomVectors[index];
	vec3 pos2;
	if (index < 3)
		pos2 = BottomVectors[index + 1];
	else
		pos2 = BottomVectors[0];

	vec4 line = vec4(pos1.x + obj->Postranslate.x, pos1.z + obj->Postranslate.z, pos2.x + obj->Postranslate.x, pos2.z + obj->Postranslate.z);
	return line;
}

float ShapeBase::GetLineVertLenght(int index) {
	ObjectPhysic* obj = m_objPhysic;

	vec4 line = GetLineVert(index);
	float x1 = line.x;
	float y1 = line.y;
	float x2 = line.z;
	float y2 = line.w;
	float lenghtLine = glm::distance(vec2(x1, y1), vec2(x2, y2)); // lenght wall
	return lenghtLine;
}

vec4 ShapeBase::GetLineVert(int index) {

	ObjectPhysic* obj = m_objPhysic;

	vec3 pos1 = BottomVectors[index];
	vec3 pos2 = TopVectors[index];
	
	vec4 line = vec4(pos1.x + obj->Postranslate.x, 
					 pos1.z + obj->Postranslate.z, 
					 pos2.x + obj->Postranslate.x, 
					 pos2.z + obj->Postranslate.z);
	return line;
}

vec3 ShapeBase::GetBottom(int index) {
	return BottomVectors[index];
}

vec3 ShapeBase::GetBottomFirst() {
	return BottomVectors[0];
}

vec3 ShapeBase::GetBottomLast() {
	return BottomVectors[BottomVectors.size() - 1];
}

vec3 ShapeBase::GetTop(int index) {
	return TopVectors[index];
}

vec3 ShapeBase::GetTopFirst() {
	return TopVectors[0];
}

vec3 ShapeBase::GetTopLast() {
	return TopVectors[TopVectors.size() - 1];
}

void ShapeBase::SetBottom(int index, vec3 value) {

	ObjectPhysic* obj = m_objPhysic;

	vec3 oldValue = BottomVectors[index];
	BottomVectors[index] = value;
	int indVert = 0;
	while (indVert < obj->MeshData.Vertices.size())
	{
		vec3 vertexNext = obj->GetVertices()[indVert];
		if (oldValue == vertexNext)
		{
			obj->MeshData.Vertices[indVert] = value;
			//obj->IsLoadedIntoMem_Vertex = false;
			obj->Render->ResetMem_Vertex();
		}
		indVert++;
	}
}

void ShapeBase::SetTop(int index, vec3 value) {

	ObjectPhysic* obj = m_objPhysic;

	vec3 oldValue = TopVectors[index];
	TopVectors[index] = value;
	int indVert = 0;
	while (indVert < obj->MeshData.Vertices.size())
	{
		vec3 vertexNext = obj->GetVertices()[indVert];
		if (oldValue == vertexNext)
		{
			obj->MeshData.Vertices[indVert] = value;
			//obj->IsLoadedIntoMem_Vertex = false;
			obj->Render->ResetMem_Vertex();
		}
		indVert++;
	}
}

void ShapeBase::GetPositRect(vec2& startPos, vec2& endPos, float& zOrder) {

	ObjectPhysic* obj = m_objPhysic;

	glm::mat4 MVP = obj->EngineData->ConfigMVP->MVP;
	glm::mat4 transform = obj->TransformResult;

	vec3 vertBottomLeft;
	vec3 vertBottomRight;
	vec3 vertTopLeft;
	vec3 vertTopRight;

	//if (IsAbsolutePosition)
	if (obj->TypeObj == TextBox)
	{
		int indLastVertMessage = obj->GetRightBorderVertexIndex();
		vertBottomLeft = GetBottom(1);
		vertBottomRight = GetBottom(indLastVertMessage);
		vertTopLeft = GetTop(1);
		vertTopRight = GetTop(indLastVertMessage);
		
	}
	else {
		vertBottomLeft = GetBottomLast();
		vertBottomRight = GetBottomFirst();
		vertTopLeft = GetTopLast();
		vertTopRight = GetTopFirst();
	}

	vec3 posWorldBL = MVP * transform * vec4(vertBottomLeft, 1.0);
	vec3 posWorldBR = MVP * transform * vec4(vertBottomRight, 1.0);
	vec3 posWorldTL = MVP * transform * vec4(vertTopLeft, 1.0);
	vec3 posWorldTR = MVP * transform * vec4(vertTopRight, 1.0);
	zOrder = posWorldTR.z;
	startPos.x = posWorldBL.x,
		startPos.y = posWorldTL.y;
	endPos.x = posWorldBR.x;
	endPos.y = posWorldBL.y;
}

vec2 ShapeBase::GetStartPositWorld() {

	ObjectPhysic* obj = m_objPhysic;

	glm::mat4 MVP = obj->EngineData->ConfigMVP->MVP;
	glm::mat4 transform = obj->TransformResult;

	vec3 vertBottomLeft;
	vec3 vertTopLeft;

	if (obj->TypeObj == TextBox)
	{
		vertBottomLeft = GetBottomFirst();
		vertTopLeft = GetTopFirst();
	}
	else {
		vertBottomLeft = GetBottomLast();
		vertTopLeft = GetTopLast();
	}

	vec3 posWorldBL = MVP * transform * vec4(vertBottomLeft, 1.0);
	vec3 posWorldTL = MVP * transform * vec4(vertTopLeft, 1.0);
	vec2 startPos;
	startPos.x = posWorldBL.x,
	startPos.y = posWorldTL.y;
	return startPos;
}


void ShapeBase::Billboard() {

	ObjectPhysic* obj = m_objPhysic;
	obj->TranslateAngle.y = obj->EngineData->Oper->HorizontalAngle + (0.5 * M_PI);
	obj->TranslateAngle.z = -obj->EngineData->Oper->VerticalAngle;
}