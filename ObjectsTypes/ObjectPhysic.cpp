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

	return CollisionPolygonState == COLLISE_UP || CollisionPolygonState == COLLISE_NORMAL;
}


bool ObjectPhysic::CheckIsLock() {

	if (TypeObj == Bullet) //CHILD CLASS
		return false;

	bool isPolygon = CheckIsPolygon();
	if (TypeObj == Hero)
	{
		if (isPolygon)
			SaveHeroOperator(true);
	}

	bool isNotValidMove = IsContactWorldBorder(NewPostranslate);
	if (!isNotValidMove)
		isNotValidMove = IsCollisionObject(Index, true);
	if (isNotValidMove)
	{
		Color = vec3(1, 0, 0);
		LockResult();
		//Pathfinding();
		return true;
	}
	else
		return false;
	return true;
}

void ObjectPhysic::LockResult() {

}

bool ObjectPhysic::IsCollisionObject(int index, bool isNewPosition)
{
	return Storage->Clusters->IsCollisionObject(index, isNewPosition);
}

void ObjectPhysic::SaveClusterObject(int index)
{
	Storage->Clusters->SaveClusterObject(index);
}


