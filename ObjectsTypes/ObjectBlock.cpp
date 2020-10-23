
#include "ObjectBlock.h"

//#include "..\CreatorModelData.h"
//#include "..\TransformModel.h"
#include "..\WorldCollision.h"
#include "..\ModelData.h"

void ObjectBlock::InitData()
{
	ActionObjectCurrent = Stay;
	IsGravity = true;
	Vertices = ModelPtr->Vertices;
}

void ObjectBlock::LockPolygonResult() {
	ActionObjectCurrent = Stay;
	Move = Postranslate;
	Move.y = PlaneDownPosition.y + ModelPtr->RadiusCollider;
	Postranslate.y = NewPostranslate.y = Move.y;
	Storage->Clusters->SaveClusterObject(Index);
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