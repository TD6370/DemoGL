#include "ObjectBullet.h"
#include "..\CreatorModelData.h"
#include "..\TransformModel.h"
//#include "..\CoreSettings.h"


void ObjectBullet::InitData()
{
	ObjectData::InitData();

	ActionObjectCurrent = Stay;
	Speed = 2;// 0.4f;
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

	if (EngineData->SceneData->IsGUI)
		return;

	if (EngineData->Inputs->MBT == KeyPush) {
		vec3 posCursorObject = GetVectorForwardFace(EngineData->ConfigMVP, StartLenght, EngineData->Oper);
		vec3 posTarget = GetVectorForward(EngineData->ConfigMVP, EndLenght, EngineData->Oper);
		Postranslate = posCursorObject;
		Target = posTarget;
		ActionObjectCurrent = Moving;
	}
}



