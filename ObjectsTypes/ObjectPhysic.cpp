//---------------------
#include "..\WorldCollision.h"
#include "..\ModelData.h"
#include "../GeomertyShapes/ShapeBase.h"
#include "../GeomertyShapes/ShapeHexagon.h"  //!!!!!!!!!!!!!!!!!!
#include "../GeomertyShapes/ShapeSquare.h"  //!!!!!!!!!!!!!!!!!!
#include "../CreatorModelData.h"

#include "../Components/RenderComponent.h"
#include "../Components/GUIComponent.h"
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

	if (IsGUIComponent) {
		Shape = new ShapeSquare();
	}
	
	Shape->UpdateShapeInfo(this);

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
		SaveToCluster();
		Postranslate = NewPostranslate;
		ActionObjectCurrent = Search;
	}
}

bool ObjectPhysic::CheckIsPolygon() {

	//string keyPosSectorStr = GetKeySectorPolygon(true);
	float step = 0.3;
	Plane* plane = NULL;
	vec4 vertex = vec4(-5000);

	//--- TEST - SPEED - COLLISION
	/*if (EngineData->Inputs->ParamCase == 5)
		CollisionPolygonState = Clusters->IsCollisionPolygon(Index, plane, vertex);
	else*/
		CollisionPolygonState = Clusters->IsCollisionPolygon(this, plane, vertex);

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
		MaterialData.Color = vec3(1, 0, 0);
		LockResult();
		return true;
	}
	else
		return false;
	return true;
}

void ObjectPhysic::FillPlanes()
{
	if (IsSquareModel) {

		MeshData.Vertices = ModelPtr->MeshData.Vertices;
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
	return Clusters->IsCollisionObject(this, indexObjHit, isNewPosition);
}

void ObjectPhysic::SelectedEvent() {
	MaterialData.Color = vec3(0, 1, 0);
}

void ObjectPhysic::UnselectedEvent() {
	MaterialData.Color = vec3(0, 0, 0);
}

void ObjectPhysic::SaveToCluster()
{
	Clusters->SaveClusterObject(this);
}

bool ObjectPhysic::GetVisible() {
	return IsVisible;
}

std::vector< glm::vec3 > ObjectPhysic::GetVertices() {
	if (MeshData.Vertices.size() != 0)
		return MeshData.Vertices;
	else	
		return ObjectData::GetVertices();
}

void ObjectPhysic::ActionMoving()
{
	//-----????
	/*if (IsGUIComponent) {
		return ComponentGUI->ActionMoving();
	}*/
}

//void ObjectPhysic::ActionTransforming()
//{
//
//}

void ObjectPhysic::CalculateNextPosition() {

}

void ObjectPhysic::SaveNewPosition() {

}
