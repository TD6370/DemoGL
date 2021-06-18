//---------------------
#include "..\WorldCollision.h"
#include "..\ModelData.h"
#include "../GeomertyShapes/ShapeHexagon.h"  //!!!!!!!!!!!!!!!!!!
#include "../GeomertyShapes/ShapeSquare.h"  //!!!!!!!!!!!!!!!!!!
//#include "..\GeometryLib.h"
//------------------------

#include "ObjectPhysic.h"

void ObjectPhysic::RunAction() {

	auto info = GetInfo();

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

	Shape->UpdateShapeInfo(this);

	ObjectData::InitData();

	IsVisible = true;

	FillPlanes();
}

ShapeHexagon* ObjectPhysic::GetShapeHexagon() {

	ShapeHexagon* shapeCast = static_cast<ShapeHexagon*>(Shape);
	return shapeCast;
}

ShapeSquare* ObjectPhysic::GetShapeSquare() {

	ShapeSquare* shapeCast = static_cast<ShapeSquare*>(Shape);
	return shapeCast;
}
//ShapeSquare* GetShapeSquare();

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
	if (IsSquareModel) {

		Vertices = ModelPtr->Vertices;
		ObjectPhysic* objPhysic = static_cast<ObjectPhysic*>(this);
		Shape->FillVertextBox();
	}
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

bool ObjectPhysic::GetVisible() {
	return IsVisible;
}

std::vector< glm::vec3 > ObjectPhysic::GetVertices() {
	if (Vertices.size() != 0)
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

void ObjectPhysic::ActionMoving()
{

}

void ObjectPhysic::ActionTransforming()
{

}

void ObjectPhysic::CalculateNextPosition() {

}

void ObjectPhysic::SaveNewPosition() {

}
