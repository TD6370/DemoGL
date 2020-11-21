#include "ObjectGUI.h"
#include "..\CreatorModelData.h"

void ObjectGUI::RunAction() {

	UpdateState();
	ObjectData::ActionBase();
}

void ObjectGUI::InitData() {

	ObjectData::InitData();
}

void ObjectGUI::UpdateState() {

	if (!Storage->SceneParam->IsGUI)
		return;

	vec3 posCursorObject = GetVectorForwardFace(Storage->MVP, 3.8, Storage->Operator);
	NewPostranslate = Postranslate = posCursorObject;

	Billboard();
}

bool ObjectGUI::IsShow() {
	return Storage->SceneParam->IsGUI;
}