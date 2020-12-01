#include "ObjectGUI.h"
#include "ObjectPhysic.h"
#include "..\CreatorModelData.h"

void ObjectGUI::RunAction() {

	UpdateState();
	ObjectData::ActionBase();
}

void ObjectGUI::InitData() {

	//ObjectData::InitData();
	ObjectPhysic::InitData();
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

bool ObjectGUI::IsCubeModel() {
	return true;
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
	objGUI->MeshTransform(vec3(size.x, size.y, 1));

	//resize mesh

	//objGUI->Size = vec3(size.x, size.y, 1);
	//objGUI->Size = vec3(size.x,size.y,1);
}



void ObjectGUI::MeshTransform(vec3 vertOffset) {
	//-- set transform

	if (Vertices.size() != 0) {

		vec3 vertB0 = GetBottom(0);
		vec3 vertB1 = GetBottom(1);
		vec3 vertT0 = GetTop(0);
		vec3 vertT1 = GetTop(1);

		float offsetY = 2 - vertOffset.y;
		float offsetX = 2 - vertOffset.x;
		/*float b0 = vertB0.x + vertB0.y + vertB0.z;
		float b1 = vertB1.x + vertB1.y + vertB1.z;
		float t0 = vertT0.x + vertT0.y + vertT0.z;
		float t1 = vertT1.x + vertT1.y + vertT1.z;*/

		vertB1 = vec3(vertB1.x, (vertB1.y + offsetY), vertB1.z + offsetX);
		SetBottom(1, vertB1);

		vertT0 = vec3(vertT0.x, (vertT0.y), vertT0.z + offsetX);
		SetTop(0, vertT0);

		vertB0 = vec3(vertB0.x, (vertB0.y + offsetY), vertB0.z);
		SetBottom(0, vertB0);
	}

}
