//---------------------
#include "..\WorldCollision.h"
//#include "..\ModelData.h"
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
			break;
		default:
			break;
		}
	}
	RunTransform();
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
		SaveClusterObject(Index);
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
	bool isPolygon = CheckIsPolygon();
	if (isPolygon)
		LockPolygonResult();

	bool isNotValidMove = IsContactWorldBorder(NewPostranslate);
	if (!isNotValidMove)
		isNotValidMove = IsCollisionObject(Index, indObjHit, true);
	if (isNotValidMove)
	{
		Color = vec3(1, 0, 0);
		LockResult();
		return true;
	}
	else
		return false;
	return true;
}

void ObjectPhysic::LockResult() {

}

void ObjectPhysic::LockPolygonResult() {

}

bool ObjectPhysic::IsCollisionObject(int index, int& indexObjHit, bool isNewPosition)
{
	return Storage->Clusters->IsCollisionObject(index, indexObjHit, isNewPosition);
}

void ObjectPhysic::SaveClusterObject(int index)
{
	Storage->Clusters->SaveClusterObject(index);
}


