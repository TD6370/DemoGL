#include "ObjectCursorRay.h"
#include "ObjectBlock.h"

#include "..\CreatorModelData.h"
#include "..\TransformModel.h"
#include "..\ModelData.h"

void ObjectCursorRay::InitData()
{
	ActionObjectCurrent = Stay;
	Speed = 0.5f;
	IsGravity = false;
	Storage->SceneParam->IndexCursorRayObj = Index;
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
	
	//ObjectPhysic::RunAction();
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

	/*ActionObjectCurrent = Stay;
	NewPostranslate.y = PlaneDownPosition.y + ModelPtr->RadiusCollider;

	SelectPositionOnPolygon();
	SelectedObjIndex = -1;
	ClearSelected();*/
}

void ObjectCursorRay::LockPolygonResult() {
	//LockResult();

	ActionObjectCurrent = Stay;
	NewPostranslate.y = PlaneDownPosition.y + ModelPtr->RadiusCollider;
	
	CusrsorAction();
}

void ObjectCursorRay::CusrsorAction() {

	SelectPositionOnPolygon();
	//SelectedObjIndex = -1;
	ClearPrevousSelected();
}

void ObjectCursorRay::SelectPositionOnPolygon()
{
	if (SelectedObjIndex != -1) {
		std::shared_ptr<ObjectData> selObject = Storage->GetObjectPrt(SelectedObjIndex);
		selObject->MeshTransform();

		/*A* pa = ...;
		B* pb1 = dynamic_cast<B*>(pa);
		B* pb2 = static_cast<B*>(pa);
		B* pb3 = (B*)pa;
		B* pb4 = reinterpret_cast<B*>(pa);*/
	}
}

void ObjectCursorRay::TargetCompleted()
{
	ActionObjectCurrent = Stay;
}

void ObjectCursorRay::Push() {

	if (Storage->Inputs->MBT == KeyPush) {
		
		Storage->Inputs->MBT = -1;
		vec3 posCursorObject = GetVectorForwardFace(Storage->MVP, StartLenght, Storage->Operator);
		//vec3 posCursorObject = GetVectorForward(Storage->MVP, StartLenght, Storage->Operator);
		vec3 posTarget = GetVectorForward(Storage->MVP, EndLenght, Storage->Operator);
		Postranslate = posCursorObject;
		Target = posTarget;
		ActionObjectCurrent = Moving;
	}
	if (Storage->Inputs->MBT == KeyClear) {
		ClearSelected();
	}
}

void ObjectCursorRay::ObjectSelected(int index) {
	if (index == -1)
		return;
	SelectedObjIndex = index;

	auto objectSelected = Storage->GetObjectPrt(SelectedObjIndex);
	objectSelected->Color = vec3(0, 1, 0);
	objectSelected->IsSelected = true;
	objectSelected->SelectedEvent();
	ClearPrevousSelected();
	PrevousSelectedObjIndex = index;
}

void ObjectCursorRay::ClearSelected() {
	SelectedObjIndex = -1;
	ClearPrevousSelected();
	UnselectedEvent();
}

void ObjectCursorRay::ClearPrevousSelected() {
	if (PrevousSelectedObjIndex != -1 && PrevousSelectedObjIndex != SelectedObjIndex) {
		auto preObject = Storage->GetObjectPrt(PrevousSelectedObjIndex);
		preObject->IsSelected = false;
		preObject->UnselectedEvent();
		PrevousSelectedObjIndex = -1;
	}
}