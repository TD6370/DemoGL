#include "ObjectCursorRay.h"

#include "..\CreatorModelData.h"
#include "..\TransformModel.h"

void ObjectCursorRay::InitData()
{
	ActionObjectCurrent = Stay;
	Speed = 0.4f;
}

void ObjectCursorRay::RunAction() {

	if (ActionObjectCurrent != Lock)
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
	RunTransform();

	Push();
}

bool ObjectCursorRay::CheckIsLock() {
	return false;
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
