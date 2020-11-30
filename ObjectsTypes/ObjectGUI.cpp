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

		vec3 vertA = GetBottom(1);
		vec3 vertB = GetBottom(0);
		vec3 vertT = GetTop(0);

		//vertA = vec3(vertA.x, (vertA.y - vertB.y)/10.5f, (vertA.z - vertT.z)/ 10.5f);
		vertA = vec3(vertA.x, (vertA.y - vertB.y), vertA.z);
		SetBottom(1, vertA);

		//vertB = vec3(vertB.x, vertB.y * vertOffset.y, vertB.z);
		//SetBottom(0, vertB);
		
		//vertT = vec3(vertT.x, vertT.y, vertT.z * vertOffset.x);
		//SetTop(0, vertT);
	}


	//if (Vertices.size() != 0) {

	//	vec3 vertA = GetBottom(1);
	//	vertA = vec3(vertA.x, vertA.y * vertOffset.y, vertA.z * vertOffset.x);
	//	SetBottom(1, vertA);

	//	vec3 vertB = GetBottom(0);
	//	vertB = vec3(vertB.x, vertB.y * vertOffset.y, vertB.z);
	//	SetBottom(0, vertB);

	//	vec3 vertT = GetTop(0);
	//	vertT = vec3(vertT.x, vertT.y, vertT.z * vertOffset.x);
	//	SetTop(0, vertT);
	//}
	/*if (Vertices.size() != 0) {

		vec3 vertA = GetBottom(1);
		vertA = vec3(vertA.x, vertA.y - vertOffset.y, vertA.z - vertOffset.x);
		SetBottom(1, vertA);

		vec3 vertB = GetBottom(0);
		vertB = vec3(vertB.x, vertB.y - vertOffset.y, vertB.z);
		SetBottom(0, vertB);

		vec3 vertT = GetTop(0);
		vertT = vec3(vertT.x, vertT.y, vertT.z - vertOffset.x);
		SetTop(0, vertT);
	}*/
}
