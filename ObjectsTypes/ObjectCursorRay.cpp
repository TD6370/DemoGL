#include "ObjectCursorRay.h"

#include "..\CreatorModelData.h"
#include "..\TransformModel.h"
#include "..\ModelData.h"

void ObjectCursorRay::InitData()
{
	ActionObjectCurrent = Stay;
	Speed = 2.f;
	IsGravity = false;
}

void ObjectCursorRay::RunAction() {

	if (ActionObjectCurrent != Lock)
	{
		while (ActionObjectCurrent == Moving)
		{
			switch (ActionObjectCurrent)
			{
				case Starting:
					GenStartPosition();
					break;
				case Moving:
					ActionMoving();
					break;
				case Stay:
					break;
				default:
					break;
			}
		}
	}
	RunTransform();
	Push();
}

bool ObjectCursorRay::CheckIsLock() {

	int indObjHit = -1;
	bool isPolygon = CheckIsPolygon();
	if (isPolygon)
		LockPolygonResult();

	bool isNotValidMove = IsContactWorldBorder(NewPostranslate);
	if (!isNotValidMove)
		isNotValidMove = IsCollisionObject(Index, indObjHit, true);
	if (isNotValidMove)
	{
		ObjectSelected(indObjHit);
		LockResult();
		return true;
	}
	else
		return false;
	return true;
}

void ObjectCursorRay::LockResult() {
	ActionObjectCurrent = Stay;
}

void ObjectCursorRay::LockPolygonResult() {
	ActionObjectCurrent = Stay;
	NewPostranslate.y = PlaneDownPosition.y + ModelPtr->RadiusCollider;
}

void ObjectCursorRay::TargetCompleted()
{
	ActionObjectCurrent = Stay;
}

void ObjectCursorRay::Push() {

	if (Storage->Inputs->MBT == KeyPush) {
		Storage->Inputs->MBT = -1;
		vec3 posCursorObject = GetVectorForwardFace(Storage->MVP, StartLenght, Storage->Operator);
		vec3 posTarget = GetVectorForward(Storage->MVP, EndLenght, Storage->Operator);
		Postranslate = posCursorObject;
		Target = posTarget;
		ActionObjectCurrent = Moving;
	}
}

void ObjectCursorRay::ObjectSelected(int index) {
	if (index == -1)
		return;
	SelectedObjIndex = index;

	auto objectSelected = Storage->GetObjectPrt(SelectedObjIndex);
	objectSelected->Color = vec3(0, 1, 0);
	if (PrevousSelectedObjIndex != -1 && PrevousSelectedObjIndex != SelectedObjIndex) {
		auto preObject = Storage->GetObjectPrt(PrevousSelectedObjIndex);
		preObject->Color = vec3(0, 0, 0);
	}
	PrevousSelectedObjIndex = index;
}