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
			case Moving:
				ActionMoving();
				break;
			case Transforming:
				ActionTransforming();
				break;
			default:
				DefaultSate();
				break;
		}
	}
	
	ObjectData::ActionBase();
}

void ObjectGUI::ActionMoving()
{
	if (ActionObjectCurrent != Moving)
		return;

	Color = m_color_transforming;
}


void ObjectGUI::InitData() {

	ObjectPhysic::InitData();
	Size = vec3(0);

	if (Storage->SceneData->IndexGUIObj == -1)
		Storage->SceneData->IndexGUIObj = Index;

	ActionObjectCurrent = Stay;
}

void ObjectGUI::UpdateState() {

	if (!Storage->SceneData->IsGUI)
		return;

	//================================================== TEST
	/*std::stringstream ss;
	ss << "Name: " << Name << "  Model:" << ModelPtr->Name << "\n";
	ss << "SP: " << StartPos.x << " " << StartPos.y << "\n\n";
	ss << "PS: " << Postranslate.x << " " << Postranslate.y << " " << Postranslate.z << "\n";
	string spStart = ss.str();*/
	//==================================================

	vec3 directionOut = vec3(0);
	PanelDepth = 3.8;
	//float panelOrderZ = (0.01f);

	if (IsAbsolutePosition)
	{
		float offsetOfCenter = 0;
		//-----------------------------
		int indOwner = IndexObjectOwner;
		if (indOwner == -1)
			indOwner = Storage->SceneData->IndexGUIObj;
		if (IndexObjectOwner != -1)
		{
			auto obj = Storage->GetObjectPrt(IndexObjectOwner);
			auto  objPh = std::dynamic_pointer_cast<ObjectPhysic>(obj);
			float lenghtLineOwner = objPh->GetLineLenght(0);
			float lenghtLine = GetLineLenght(0);
			float offsetOwner = lenghtLineOwner/2;
			float offsetAbs = lenghtLine / 2;
			offsetOfCenter = offsetOwner - offsetAbs;
		}

		vec3 startPos = vec3(StartPos.x - offsetOfCenter, StartPos.y - offsetOfCenter, StartPos.z); //when SetSizeControl - is disabled
		Postranslate = NewPostranslate = GetVectorForwardFaceOffset(Storage->ConfigMVP, 
																	PanelDepth - StartPos.z, 
																	Storage->Oper, startPos);

		//================================================== TEST
		//TEST -- hystory move pos TextBox
		/*if (Postranslate != tmp_posit && TypeObj == TextBlock)
		{
			std::stringstream ssEnd;
			ssEnd << "END PS: " << Postranslate.x << " " << Postranslate.y << " " << Postranslate.z << "\n";
			string totalEnd = ssEnd.str();
			tmp_posit = Postranslate;
			if(hyst_posit.size()==0)
				hyst_posit.append(spStart);
			hyst_posit.append(totalEnd);
			hyst_posit.append("--------------------------------");
		}*/
		//==================================================
	}
	else {
		if (StartPos == vec3(0)) {
			Postranslate = NewPostranslate = GetVectorForwardFace(Storage->ConfigMVP, PanelDepth, Storage->Oper);
		}
		else {
			Postranslate = NewPostranslate = GetVectorForwardFaceOffset(Storage->ConfigMVP, PanelDepth - StartPos.z, Storage->Oper, StartPos);
		}
	}
	Billboard();
}

bool ObjectGUI::GetVisible() {

	//TODO:: IndexObjectOwner 

	return Storage->SceneData->IsGUI;
}

void ObjectGUI::ConfigInterface(string caption, string nameModel, string nameObject, vec3 startPosChild, vec2 size, TypeObject p_typeObj, vec3 color)
{
	if(color == vec3(0))
		color = vec3(1);

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

	if (IsAbsolutePosition)
		return;

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