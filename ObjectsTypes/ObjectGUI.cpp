#include "ObjectGUI.h"
#include "ObjectPhysic.h"
#include "ObjectTextBlock.h"
#include "ObjectButton.h"

#include "..\CreatorModelData.h"
#include "..\Serialize\SceneSerialize.h"
#include "..\ModelData.h"
#include "../CoreSettings.h"

#include "../GeomertyShapes/ShapeBase.h"
#include "../GeomertyShapes/ShapeSquare.h"


void ObjectGUI::InitData() {

	AnimationParam = new AnimationParamGUI();
	Shape = new ShapeSquare();
	Shape->UpdateShapeInfo(this);
	
	ObjectPhysic::InitData();

	Size = vec3(0);

	if (Storage->SceneData->IndexGUIObj == -1)//First create gui
		Storage->SceneData->IndexGUIObj = Index;

	ActionObjectCurrent = Stay;
}

void ObjectGUI::SetDataToShader() {

	ObjectData::SetDataToShader();

	/*if (!isUpdate)
		return;*/

	auto modelFrame =std::dynamic_pointer_cast<ModelFrame>(ModelPtr);
	if (modelFrame == nullptr) {
		return;
	}

	float width = GetShapeSquare()->WidthFactor;
	float height = GetShapeSquare()->HeightFactor;
	vec3 posMove = GetShapeSquare()->PosMoveFactor;
	vec3 posMoveSize = GetShapeSquare()->PosMoveSizeFactor;
	if (width < 0)
		return;
	modelFrame->SetWidth(width);
	modelFrame->SetHeight(height);
	modelFrame->SetPosMove(posMove);
	modelFrame->SetPosMoveSize(posMoveSize);
		
}


void ObjectGUI::RunAction() {

	GetShapeSquare()->UpdateState();

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


bool ObjectGUI::GetVisible() {

	//TODO:: IndexObjectOwner 

	return Storage->SceneData->IsGUI;
}

shared_ptr<ObjectData> ObjectGUI::ConfigInterface(string caption, string nameModel, string nameObject, vec3 startPosChild, vec2 size, TypeObject p_typeObj, vec3 color)
{
	if(color == vec3(0))
		color = vec3(1);

	std::shared_ptr<ModelData> model = Storage->GetModelPrt(nameModel);
	auto  modelTextBox = std::dynamic_pointer_cast<ModelTextBox>(model);
	if (modelTextBox != nullptr) {
		model = modelTextBox;
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
	objGUI->UpdateState();
	//objGUI->IsTextureRepeat = true;

	objGUI->GetShapeSquare()->SetSizeControl(vec3(size.x, size.y, 1));

	auto objTextBlock = std::dynamic_pointer_cast<ObjectTextBlock>(obj);
	if (objTextBlock != nullptr) {
		objTextBlock->Message = caption;
		objTextBlock->CreateMessage();
	}

	return objGUI;
}

void ObjectGUI::ControlConstruct(shared_ptr<ObjectGUI> obj, string caption)
{
	//TEST&&1
	//return;

	shared_ptr<ObjectData> objData;
	auto objButton = std::dynamic_pointer_cast<ObjectButton>(obj);
	if (objButton != nullptr) {
		if (!objButton->IsToogleButon) {
			vec2 offset = vec2(0.01);
			//vec3 startPos = vec3(StartPos.x + offset.x, StartPos.y + offset.y, 0.021);
			vec3 startPos = vec3(offset.x, offset.y, 0.021);
			startPos = vec3(.01, .01, 0.021);
			//objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.01, .01, 0.021), vec2(1.5, 1.), TextBlock, color);
			objData = objButton->ConfigInterface(caption, "TextBlockModel", "Button_TextBlock", vec3(startPos.x, startPos.y, startPos.z), vec2(1.5, 1.), TextBlock, vec3(-1));
			auto objTextButton = std::dynamic_pointer_cast<ObjectTextBlock>(objData);
			//objTextButton->InitData();
			//objTextButton->GetShapeSquare()->UpdateState(true);
		}
	}
}

void ObjectGUI::SizeControlUpdate()
{
	GetShapeSquare()->SetSizeControl(vec3(SizePanel.x, SizePanel.y, 1));
}

//----------------------------------------------------------------

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
	
	GetShapeSquare()->SetSizeControl(vec3(SizePanel.x, SizePanel.y, 1));
}

//----------------------------------------------------------------

void ObjectGUI::Click() {
	ActionObjectCurrent = Woking;
}

void ObjectGUI::Work() {

	Color = m_color_work;
	ActionObjectCurrent = Stay; //Off
}

void ObjectGUI::DefaultSate() {

	if (Color == m_color_work)
	{
		Color = vec3(0); //default color
	}
}