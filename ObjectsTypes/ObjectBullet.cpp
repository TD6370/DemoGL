#include "ObjectBullet.h"
#include "..\CreatorModelData.h"
#include "..\TransformModel.h"
//#include "..\CoreSettings.h"


void ObjectBullet::InitData()
{
	ActionObjectCurrent = Stay;
	Speed = 0.4f;
}

void ObjectBullet::RunAction() {

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

bool ObjectBullet::CheckIsLock() {
	return false;
}


void ObjectBullet::Push() {

	if (Storage->Inputs->MBT == KeyPush) {
		Storage->Inputs->MBT = -1;
		vec3 posCursorObject = GetVectorForwardFace(Storage->MVP, StartLenght, Storage->Operator);
		vec3 posTarget = GetVectorForward(Storage->MVP, EndLenght, Storage->Operator);
		Postranslate = posCursorObject;
		Target = posTarget;
		ActionObjectCurrent = Moving;
	}
}



