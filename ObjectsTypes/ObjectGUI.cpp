#include "ObjectGUI.h"
#include "ObjectPhysic.h"
#include "..\CreatorModelData.h"
#include "..\Serialize\SceneSerialize.h"
#include "..\ModelData.h"

void ObjectGUI::RunAction() {

	UpdateState();

	if (ActionObjectCurrent != Lock)
	{
		switch (ActionObjectCurrent)
		{
			case Woking:
				Work();
				break;
			default:
				DefaultSate();
				break;
		}
	}
	ObjectData::ActionBase();
}


void ObjectGUI::InitData() {

	//ObjectData::InitData();
	ObjectPhysic::InitData();
}

void ObjectGUI::UpdateState() {

	if (!Storage->SceneData->IsGUI)
		return;

	vec3 directionOut = vec3(0);
	PanelDepth = 3.8;
	//PanelDepth = 4.8;
	//PanelDepth = 5.8;
	//PanelDepth = 6.8;
	if (StartPos == vec3(0)) {
		Postranslate = NewPostranslate = GetVectorForwardFace(Storage->ConfigMVP, PanelDepth, Storage->Oper);
	}else	{
		Postranslate = NewPostranslate = GetVectorForwardFaceOffset(Storage->ConfigMVP, PanelDepth - 0.01f, Storage->Oper , StartPos);
	}

	//StateRunning();
	
	Billboard();
}

bool ObjectGUI::IsShow() {
	return Storage->SceneData->IsGUI;
}

bool ObjectGUI::IsCubeModel() {
	return true;
}

void ObjectGUI::ConfigInterface(string caption, string nameModel, string nameObject, vec3 startPosChild, vec2 size, vec3 color)
{
	if(color == vec3(0))
		color = vec3(1, 1, 0);

	std::shared_ptr<ModelData> model = Storage->GetModelPrt(nameModel);
	auto modelGUI = std::dynamic_pointer_cast<ModelGUI>(model);
	if (modelGUI != nullptr)
		model = modelGUI;

	shared_ptr<ObjectData> obj = Storage->AddObject(nameObject, model, GUI, StartPos, color);
	auto objGUI = std::dynamic_pointer_cast<ObjectGUI>(obj);
	objGUI->IndexObjectOwner = Index;
	objGUI->StartPos = startPosChild;
	objGUI->Size = size;
	objGUI->Color = color;
	objGUI->SetSizeControl(vec3(size.x, size.y, 1));
}

void ObjectGUI::SetSizeControl(vec3 vertOffset) {
	//-- set transform

	if (Vertices.size() != 0) {

		vec3 vertBottomLeft = GetBottom(0);
		vec3 vertBottomRight = GetBottom(1);
		vec3 vertTopLeft = GetTop(0);
		vec3 vertTopRight = GetTop(1);

		float offsetY = 2 - vertOffset.y;
		float offsetX = 2 - vertOffset.x;
		/*float b0 = vertB0.x + vertB0.y + vertB0.z;
		float b1 = vertB1.x + vertB1.y + vertB1.z;
		float t0 = vertT0.x + vertT0.y + vertT0.z;
		float t1 = vertT1.x + vertT1.y + vertT1.z;*/

		vertTopLeft = vec3(vertTopLeft.x, (vertTopLeft.y), vertTopLeft.z + offsetX);
		SetTop(0, vertTopLeft);

		vertBottomLeft = vec3(vertBottomLeft.x, (vertBottomLeft.y + offsetY), vertBottomLeft.z);
		SetBottom(0, vertBottomLeft);

		vertBottomRight = vec3(vertBottomRight.x, (vertBottomRight.y + offsetY), vertBottomRight.z + offsetX);
		SetBottom(1, vertBottomRight);

	}
}

vector<ObjectFiledsSpecific> ObjectGUI::GetSpecificFiels() {

	SceneSerialize* serializer = new SceneSerialize();

	vector<ObjectFiledsSpecific> result { 
		{"StartPos:", serializer->Vec3Str(StartPos)} ,
		{"Size:", serializer->Vec2Str(Size)}
	};

	return result;
}

void ObjectGUI::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (IndexObjectOwner == -1) //Back GUI
		return;

	SceneSerialize* serializer = new SceneSerialize();

	StartPos = serializer->StrToVec3(filedsSpecific[0].Value);
	Size = serializer->StrToVec3(filedsSpecific[1].Value);
	
	SetSizeControl(vec3(Size.x, Size.y, 1));
}

void ObjectGUI::Click() {
	ActionObjectCurrent = Woking;
}

void ObjectGUI::Work() {

	Color = m_color_work;
	//ActionObjectCurrent = Stay;
}

void ObjectGUI::DefaultSate() {

	if (Color == m_color_work)
	{
		Color = vec3(0); //default color
	}
}