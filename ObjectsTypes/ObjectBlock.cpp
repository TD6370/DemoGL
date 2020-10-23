
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

	Storage->Clusters->SaveClusterObject(Index);

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

	if (Vertices.size() != 0) {
		Vertices[0]++;
		Vertices[1]++;
		Vertices[2]++;
	}
}