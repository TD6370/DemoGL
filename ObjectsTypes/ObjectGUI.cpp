#include "ObjectGUI.h"
#include "ObjectPhysic.h"
#include "..\CreatorModelData.h"
#include "..\Serialize\SceneSerialize.h"
#include "..\ModelData.h"
#include "../CoreSettings.h"
#include "ObjectTextBlock.h"

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

	ObjectPhysic::InitData();
	Size = vec3(0);
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

	Billboard();
}

bool ObjectGUI::IsShow() {
	return Storage->SceneData->IsGUI;
}

bool ObjectGUI::IsCubeModel() {
	return true;
}

void ObjectGUI::ConfigInterface(string caption, string nameModel, string nameObject, vec3 startPosChild, vec2 size, TypeObject p_typeObj, vec3 color)
{
	//if(color == vec3(0))
	//	color = vec3(1, 1, 0);

	std::shared_ptr<ModelData> model = Storage->GetModelPrt(nameModel);
	auto  modelTextBlock = std::dynamic_pointer_cast<ModelTextBlock>(model);
	if (modelTextBlock != nullptr) {
		model = modelTextBlock;
	}
	auto modelFrame = std::dynamic_pointer_cast<ModelFrame>(model);
	if (modelFrame != nullptr) {
		model = modelFrame;
	}

	shared_ptr<ObjectData> obj = Storage->AddObject(nameObject, model, p_typeObj, StartPos, color);
	auto objGUI = std::dynamic_pointer_cast<ObjectGUI>(obj);
	objGUI->IndexObjectOwner = Index;
	objGUI->StartPos = startPosChild;
	objGUI->SizePanel = size;
	objGUI->Color = color;
	//objGUI->IsTextureRepeat = true;
	objGUI->SetSizeControl(vec3(size.x, size.y, 1));

	auto objTextBlock = std::dynamic_pointer_cast<ObjectTextBlock>(obj);
	if (objTextBlock != nullptr) {
		objTextBlock->Message = caption;
		objTextBlock->CreateMessage();
	}
}


void ObjectGUI::SetSizeControl(vec3 vertOffset) {

	//return;

	//-- set transform
	if (Vertices.size() != 0) {

		vec3 vertBottomLeft = GetBottom(1);
		vec3 vertBottomRight = GetBottom(0);
		vec3 vertTopLeft = GetTop(1);
		vec3 vertTopRight = GetTop(0);

		float offsetY = 2 - vertOffset.y;
		float offsetX = 2 - vertOffset.x;

		float factor = vertTopLeft.y - vertBottomLeft.y;


		vertTopLeft = vec3(vertTopLeft.x, (vertTopLeft.y), vertTopLeft.z + offsetX);
		SetTop(1, vertTopLeft);

		vertBottomLeft = vec3(vertBottomLeft.x, (vertBottomLeft.y + offsetY), vertBottomLeft.z);
		SetBottom(1, vertBottomLeft);

		vertBottomRight = vec3(vertBottomRight.x, (vertBottomRight.y + offsetY), vertBottomRight.z + offsetX);
		SetBottom(0, vertBottomRight);
	}
}


void ObjectGUI::ResizeTextureUV() {
	if (IsTextureRepeat) {
		std::vector< glm::vec2 > repeat_UV = ModelPtr->UV;
		for (auto& uv : repeat_UV) {
			uv.y *= TextureRepeat;
		}
		TextureUV = repeat_UV;
	}
}

vector<ObjectFiledsSpecific> ObjectGUI::GetSpecificFiels() {

	SceneSerialize* serializer = new SceneSerialize();

	vector<ObjectFiledsSpecific> result { 
		{"StartPos:", serializer->Vec3Str(StartPos)} ,
		{"SizePanel:", serializer->Vec2Str(SizePanel)}
	};

	return result;
}

void ObjectGUI::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (IndexObjectOwner == -1) //Back GUI
		return;

	SceneSerialize* serializer = new SceneSerialize();

	StartPos = serializer->StrToVec3(filedsSpecific[0].Value);
	SizePanel = serializer->StrToVec2(filedsSpecific[1].Value);
	
	SetSizeControl(vec3(SizePanel.x, SizePanel.y, 1));
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