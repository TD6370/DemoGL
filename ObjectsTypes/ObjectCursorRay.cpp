#include "ObjectCursorRay.h"
#include "ObjectBlock.h"

#include "..\CreatorModelData.h"
#include "..\TransformModel.h"
#include "..\ModelData.h"

void ObjectCursorRay::InitData()
{
	ObjectData::InitData();

	ActionObjectCurrent = Stay;
	//Speed = 0.5f;
	Speed = 0.1f;
	IsGravity = false;
	EngineData->SceneData->IndexCursorRayObj = Index;
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
}

bool ObjectCursorRay::CheckIsLock() {

	int indObjHit = -1;
	bool isPolygon = CheckIsPolygon();
	if (isPolygon)
		LockPolygonResult();

	bool isLock = IsContactWorldBorder(NewPostranslate);
	if (!isLock)
		isLock = IsCollisionObject(Index, indObjHit, true);
	if (isLock)
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
	NewPostranslate.y = PlaneDownPosition.y + ModelPtr->MeshData.RadiusCollider;
	
	CusrsorAction();
}

void ObjectCursorRay::CusrsorAction() {

	SelectPositionOnPolygon();
	ClearPrevousSelected();
}

void ObjectCursorRay::SelectPositionOnPolygon()
{
	if (SelectedObjIndex != -1) {
		m_preSelectedObjectAction = ActionObjectCurrent;
		
		SceneCommand->CommandType = EditObjectCommand;
		SceneCommand->SourceIndex = Index;
		SceneCommand->TargetIndex = SelectedObjIndex;
		SceneCommand->ValueI = (int)Transforming;
		SceneCommand->Enable = true;
	}
}

void ObjectCursorRay::SelectedCompleted() {

	
}

void ObjectCursorRay::TargetCompleted()
{
	ActionObjectCurrent = Stay;
}

void ObjectCursorRay::Click()
{
	if (EngineData->SceneData->IsGUI)
		return;

	if (EngineData->Inputs->MBT == KeyPush) {
		
		vec3 posCursorObject = GetVectorForwardFace(EngineData->ConfigMVP, StartLenght, EngineData->Oper);
		//vec3 posCursorObject = GetVectorForward(Storage->MVP, StartLenght, Storage->Operator);
		vec3 posTarget = GetVectorForward(EngineData->ConfigMVP, EndLenght, EngineData->Oper);
		Postranslate = posCursorObject;
		Target = posTarget;
		ActionObjectCurrent = Moving;
	}
	if (EngineData->Inputs->MBT == KeyClear) {
		ClearSelected();
	}
}

void ObjectCursorRay::ObjectSelected(int index) {
	if (index == -1)
		return;

	PrevousSelectedObjIndex = SelectedObjIndex;
	SelectedObjIndex = index;

	//-----------------
	SceneCommand->CommandType = EditObjectCommand;
	SceneCommand->SourceIndex = PrevousSelectedObjIndex; //unselected obj
	SceneCommand->TargetIndex = SelectedObjIndex;
	SceneCommand->ValueI = (int)Woking;
	SceneCommand->Enable = true;
}

void ObjectCursorRay::ClearSelected() {
	
	SceneCommand->CommandType = EditObjectCommand;
	SceneCommand->SourceIndex = Index;
	SceneCommand->TargetIndex = SelectedObjIndex;
	SceneCommand->ValueI = (int)Clearing;
	SceneCommand->Enable = true;

	SelectedObjIndex = -1;
	UnselectedEvent();
}

void ObjectCursorRay::ClearPrevousSelected() {
	
}