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

void ObjectBullet::TargetCompleted()
{
	ActionObjectCurrent = Stay;
}

void ObjectBullet::Push() {

	if (Storage->SceneData->IsGUI)
		return;

	if (Storage->Inputs->MBT == KeyPush) {
		vec3 posCursorObject = GetVectorForwardFace(Storage->ConfigMVP, StartLenght, Storage->Oper);
		vec3 posTarget = GetVectorForward(Storage->ConfigMVP, EndLenght, Storage->Oper);
		Postranslate = posCursorObject;
		Target = posTarget;
		ActionObjectCurrent = Moving;
	}
}



