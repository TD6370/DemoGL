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

	float zOrder = 3.8;
	if (StartPos == vec3(0)) {
		Postranslate = NewPostranslate = GetVectorForwardFace(Storage->MVP, zOrder, Storage->Operator);
	}else	{
		Postranslate = NewPostranslate = GetVectorForwardFaceOffset(Storage->MVP, zOrder - 0.01f, Storage->Operator , StartPos);
	}

	Billboard();
}

bool ObjectGUI::IsShow() {
	return Storage->SceneParam->IsGUI;
}


void ObjectGUI::ConfigInterface(string caption, string nameModel, string nameObject, vec3 startPosChild, vec2 size)
{
	vec3 color = vec3(1, 1, 0);
	std::shared_ptr<ModelData> model = Storage->GetModelPrt(nameModel);
	auto obj = Storage->AddObject(nameObject, model, GUI, StartPos, color);
	auto objGUI = std::dynamic_pointer_cast<ObjectGUI>(obj);
	objGUI->IndexObjectOwner = Index;
	objGUI->StartPos = startPosChild;
	objGUI->Color = color;
}

