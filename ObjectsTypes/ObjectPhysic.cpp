//---------------------
#include "..\WorldCollision.h"
#include "..\ModelData.h"
//#include "..\GeometryLib.h"
//------------------------

#include "ObjectPhysic.h"

void ObjectPhysic::RunAction() {

	if (ActionObjectCurrent != Lock)
	{
		switch (ActionObjectCurrent)
		{
		case Starting:
			GenStartPosition();
			break;
		case Stay:
			//TestGravity();
			break;
		default:
			break;
		}
	}
	//RunTransform();
	ObjectData::ActionBase();
}

void ObjectPhysic::InitData() {

	ObjectData::InitData();

	IsVisible = true;

	FillPlanes();
}


bool ObjectPhysic::IsContactWorldBorder(vec3 pos) {

	World WorldSetting;
	bool result = false;
	if (pos.x > WorldSetting.Radius || pos.x < -WorldSetting.Radius || pos.z > WorldSetting.Radius || pos.z < -WorldSetting.Radius)
		result = true;
	return result;
}

void ObjectPhysic::CheckStartPosition() {
	if (!CheckIsLock())
	{
		//SaveClusterObject(Index);
		SaveToCluster();
		Postranslate = NewPostranslate;
		ActionObjectCurrent = Search;
	}
}

bool ObjectPhysic::CheckIsPolygon() {

	//string keyPosSectorStr = GetKeySectorPolygon(true);
	float step = 0.3;
	Plane* plane = NULL;
	//vec3 vertex = vec3(-5000);
	vec4 vertex = vec4(-5000);

	CollisionPolygonState = Storage->Clusters->IsCollisionPolygon(Index, plane, vertex);

	if (IsGravity) {
		switch (CollisionPolygonState)
		{
		case COLLISE_UP:
		{
			//Postranslate.y = NewPostranslate.y = (NewPostranslate.y + step);
			NewPostranslate.y = (NewPostranslate.y + step);
			break;
		}
		case COLLISE_DOWN:
			//Postranslate.y = NewPostranslate.y = (NewPostranslate.y - step);
			NewPostranslate.y = (NewPostranslate.y - step);
			break;
		case COLLISE_NORMAL:
			break;
		default:
			break;
		}
	}
	return CollisionPolygonState == COLLISE_UP || CollisionPolygonState == COLLISE_NORMAL;
}


bool ObjectPhysic::CheckIsLock() {

	int indObjHit = -1;
	bool isPolygonHit = CheckIsPolygon();
	if (isPolygonHit)
		LockPolygonResult();


	bool isHit = IsContactWorldBorder(NewPostranslate);
	if (!isHit)
		isHit = IsCollisionObject(Index, indObjHit, true);
	if (isHit)
	{
		Color = vec3(1, 0, 0);
		LockResult();
		return true;
	}
	else
		return false;
	return true;
}

//void ObjectPhysic::TestGravity()
//{
//	if (Postranslate == Move)
//		return;
//	CheckIsLock();
//}

void ObjectPhysic::FillPlanes()
{
	if (IsCubeModel()) {
		Vertices = ModelPtr->Vertices;
		FillPlanesCube();
	}
}

void ObjectPhysic::FillPlanesCube()
{
	std::vector<vec3> CubeVectors;
	int indexPlaneT = 0;
	int indexPlaneB = 0;
	int indVert = 0;

	BottomVectors.clear();
	TopVectors.clear();

	while (indVert < GetVertices().size())
	{
		vec3 vertexNext = GetVertices()[indVert];
		indVert++;
		bool isExist = std::find(CubeVectors.begin(), CubeVectors.end(), vertexNext) != CubeVectors.end();
		if (isExist)
			continue;

		CubeVectors.push_back(vertexNext);
		if (vertexNext.y < 0) {
			BottomVectors.insert(std::pair<int, vec3>(indexPlaneB, vertexNext));
			indexPlaneB++;
		}
		else {
			TopVectors.insert(std::pair<int, vec3>(indexPlaneT, vertexNext));
			indexPlaneT++;
		}
	}
	/*if (TopVectors.find(indVert) == TopVectors.end()) {}*/
}

void ObjectPhysic::LockResult() {

}

void ObjectPhysic::LockPolygonResult() {

}

bool ObjectPhysic::IsCollisionObject(int index, int& indexObjHit, bool isNewPosition)
{
	return Storage->Clusters->IsCollisionObject(index, indexObjHit, isNewPosition);
}

void ObjectPhysic::SelectedEvent() {
	Color = vec3(0, 1, 0);
}

void ObjectPhysic::UnselectedEvent() {
	Color = vec3(0, 0, 0);
}

void ObjectPhysic::SaveToCluster()
{
	Storage->Clusters->SaveClusterObject(Index);
}

bool ObjectPhysic::IsShow() {
	return IsVisible;
}


//------------

bool ObjectPhysic::IsCubeModel() {
	return false;
}

std::vector< glm::vec3 > ObjectPhysic::GetVertices() {
	if (Vertices.size() != 0)
	//if(TypeObj == Block)
		return Vertices;
	else	
		return ObjectData::GetVertices();
}

void ObjectPhysic::SetMesh() {
	if (Vertices.size() != 0)
	//if (TypeObj == Block)
		ModelPtr->SetVAO(Vertices);
	else
		ObjectData::SetMesh();
}

vec4 ObjectPhysic::GetLine(int index) {
	vec3 pos1 = BottomVectors[index];
	vec3 pos2;
	if (index < 3)
		pos2 = BottomVectors[index + 1];
	else
		pos2 = BottomVectors[0];

	vec4 line = vec4(pos1.x + Postranslate.x, pos1.z + Postranslate.z, pos2.x + Postranslate.x, pos2.z + Postranslate.z);
	return line;
}

vec3 ObjectPhysic::GetBottom(int index) {
	return BottomVectors[index];
}

vec3 ObjectPhysic::GetTop(int index) {
	return TopVectors[index];
}

void ObjectPhysic::SetBottom(int index, vec3 value) {

	vec3 oldValue = BottomVectors[index];
	BottomVectors[index] = value;
	int indVert = 0;
	while (indVert < Vertices.size())
	{
		vec3 vertexNext = GetVertices()[indVert];
		if (oldValue == vertexNext)
		{
			Vertices[indVert] = value;
		}
		indVert++;
	}
}

void ObjectPhysic::SetTop(int index, vec3 value) {
	vec3 oldValue = TopVectors[index];
	TopVectors[index] = value;
	int indVert = 0;
	while (indVert < Vertices.size())
	{
		vec3 vertexNext = GetVertices()[indVert];
		if (oldValue == vertexNext)
		{
			Vertices[indVert] = value;
		}
		indVert++;
	}
}

void ObjectPhysic::GetPositRect(vec2& startPos, vec2& endPos, float& zOrder) {

	glm::mat4 MVP = Storage->ConfigMVP->MVP;
	glm::mat4 transform = TransformResult;
	vec3 vertBottomLeft = GetBottom(1);
	vec3 vertBottomRight = GetBottom(0);
	vec3 vertTopLeft = GetTop(1);
	vec3 vertTopRight = GetTop(0);
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

