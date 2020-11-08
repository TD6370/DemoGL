
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

void ObjectBlock::SelectVertexBlock() {

	vec3 posCursor;

	int indCursor = Storage->SceneParam->IndexCursorRayObj;
	std::shared_ptr<ObjectData> CusrorObj = Storage->GetObjectPrt(indCursor);
	posCursor = CusrorObj->Postranslate;

	/*int indHero = Storage->SceneParam->IndexHeroObj;
	std::shared_ptr<ObjectData> HeroObj = Storage->GetObjectPrt(indHero);
	posCursor = HeroObj->Postranslate;*/

	int distMin = -1;
	for (int indV = 0; indV < 4; indV++) {
		vec3 vertBlock = GetBottom(indV);

		vertBlock += Postranslate;
		int dist = glm::distance(vec2(posCursor.x, posCursor.z), vec2(vertBlock.x, vertBlock.z));

		if (distMin == -1 || distMin > dist) {
			distMin = dist;
			IndexVertexTransform = indV;
		}
	}
}

void ObjectBlock::SelectedEvent() {

	ObjectPhysic::SelectedEvent();
	SelectVertexBlock();
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
	int indexUpdate = IndexVertexTransform;
	
	//-- Calculate transform
	vec3 newPos = CusrorObj->Postranslate;
	vec3 offsetPosScene = newPos - Postranslate;
	vec3 vertOffset = vec3(offsetPosScene.x, 0, offsetPosScene.z);

	//-- set transform
	if (Vertices.size() != 0) {
		vec3 vertA =  GetBottom(indexUpdate);
		vertA = vec3(vertOffset.x, vertA.y, vertOffset.z);
		SetBottom(indexUpdate, vertA);

		vec3 vertB = GetTop(indexUpdate);
		vertB = vec3(vertOffset.x, vertB.y, vertOffset.z);
		SetTop(indexUpdate, vertB);
	}

	SaveNewPosition();
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

vec4 ObjectBlock::GetLine(int index) {
	vec3 pos1 = BottomVectors[index];
	vec3 pos2;
	if(index<3)
		pos2 = BottomVectors[index + 1];
	else
		pos2 = BottomVectors[0];
	
	vec4 line = vec4(pos1.x + Postranslate.x, pos1.z + Postranslate.z, pos2.x + Postranslate.x, pos2.z + Postranslate.z);
	return line;
}