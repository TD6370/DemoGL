
#include "ObjectBlock.h"

//#include "..\CreatorModelData.h"
//#include "..\TransformModel.h"
#include "..\WorldCollision.h"
#include "..\ModelData.h"
#include "ObjectDynamic.h"

void ObjectBlock::InitData()
{
	ActionObjectCurrent = Stay;
	IsGravity = true;
	Vertices = ModelPtr->Vertices;

	ObjectPhysic::InitData();
}

void ObjectBlock::LockPolygonResult() {
	SaveNewPosition();
}

void ObjectBlock::LockResult() {
	SaveNewPosition();
}

void ObjectBlock::SaveNewPosition()
{
	ActionObjectCurrent = Stay;
	Move = Postranslate;
	//Set position polygon
	Move.y = PlaneDownPosition.y + ModelPtr->RadiusCollider;
	NewPostranslate = Move;

	//Storage->Clusters->SaveClusterObject(Index);
	SaveToCluster();

	Postranslate = NewPostranslate;
}

void ObjectBlock::RunAction() {

	if (ActionObjectCurrent != Lock)
	{
		switch (ActionObjectCurrent)
		{
			case Starting:
				GenStartPosition();
				break;
			case Stay:
				TestGravity();
				break;
			default:
				break;
		}
	}
	ObjectData::ActionBase();
}

void ObjectBlock::TestGravity()
{
	if (Postranslate == Move)
		return;
	CheckIsLock();
}


void ObjectBlock::EventChange() {
	if (IsSelected) {
		Color = vec3(0, 1, 0);
	}
}

std::vector< glm::vec3 > ObjectBlock::GetVertices() {
	return Vertices;
}


void ObjectBlock::SetMesh() {
	ModelPtr->SetVAO(Vertices);
}

void ObjectBlock::MeshTransform() {

	int indCursor = Storage->SceneParam->IndexCursorRayObj;
	std::shared_ptr<ObjectData> CusrorObj = Storage->GetObjectPrt(indCursor);
	
	//-- Param transform
	int indexUpdate = 0;
	
	//-- Calculate transform
	vec3 newPos = CusrorObj->Postranslate;
	vec3 offsetPosScene = glm::normalize(newPos - Postranslate);

	vec3 factorV = newPos/Postranslate;
	float factor = factorV.x + factorV.z;
	
	vec3 vertBlock = GetBottom(indexUpdate);

	vec3 vertOffset = vec3(offsetPosScene.x, 0, offsetPosScene.z);

	//-- set transform
	if (Vertices.size() != 0) {
		vec3 vertA =  GetBottom(indexUpdate);
		vertA += vertOffset;
		SetBottom(indexUpdate, vertA);

		vec3 vertB = GetTop(indexUpdate);
		vertB += vertOffset;
		SetTop(indexUpdate, vertB);
	}
}

vec3 ObjectBlock::GetBottom(int index) {
	return BottomVectors[index];
}

vec3 ObjectBlock::GetTop(int index) {
	return TopVectors[index];
}

void ObjectBlock::SetBottom(int index, vec3 value) {

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

void ObjectBlock::SetTop(int index, vec3 value) {
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

void ObjectBlock::FillPlanes()
{
	std::vector<vec3> CubeVectors;
	int indexPlaneT = 0;
	int indexPlaneB = 0;
	int indVert = 0;

	while (indVert < GetVertices().size())
	{
		vec3 vertexNext = GetVertices()[indVert];
		indVert++;
		bool isExist = std::find(CubeVectors.begin(), CubeVectors.end(), vertexNext) != CubeVectors.end();
		if (isExist)
			continue;

		CubeVectors.push_back(vertexNext);
		if (vertexNext.y < 0){
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