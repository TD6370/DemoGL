#include "ObjectGUI.h"
#include "ObjectPhysic.h"
//#include "ObjectButton.h"

#include "..\CreatorModelData.h"
#include "..\Serialize\SceneSerialize.h"
#include "..\ModelData.h"
#include "../CoreSettings.h"

#include "../GeomertyShapes/ShapeBase.h"
#include "../GeomertyShapes/ShapeSquare.h"
#include "../ShellObjects/BaseShell.h"
#include "../Components/RenderComponent.h"

#include "../Components/TextBoxComponent.h"
#include "../Components/ButtonComponent.h"

void ObjectGUI::InitData() {

	
	AnimationParam = new AnimationParamGUI();
	Shape = new ShapeSquare();
	Shape->UpdateShapeInfo(this);
	
	ObjectPhysic::InitData();

	Size = vec3(0);

	if (EngineData->SceneData->IndexBackgroundGUIObj == -1)//First create gui
		EngineData->SceneData->IndexBackgroundGUIObj = Index;

	ActionObjectCurrent = Stay;

	if (TypeObj == TypeObject::CursorGUI) {
		IsAbsolutePosition = true;
		IsFocusable = false;
		IsTransformable = false;
		IsUsable = false;

		ActionObjectCurrent = ActionObject::Moving;
	}
}

void ObjectGUI::RunAction() {

	GetShapeSquare()->FormUpdate();

	if (ActionObjectCurrent != Lock)
	{
		switch (ActionObjectCurrent)
		{
			case Woking:
				/*if (IsButtonComponent)
				{
					ActionWork();
					ComponentButton->ActionWork();
				}
				else if (IsTextBoxComponent)
				{
					TextBox->ActionWork();
				}
				else*/
					ActionWork();
				break;
			case Moving:
				ActionMoving();
				break;
			/*case Transforming:
				ActionTransforming();
				break;*/
			default:
				break;
		}
	}

	ObjectData::ActionBase();
}

void ObjectGUI::ActionMoving()
{
	if (ActionObjectCurrent != Moving)
		return;

	if (TypeObj == TypeObject::CursorGUI) {
		SaveNewPosition();
		return;
	}

	MaterialData.Color = m_color_transforming;
}


bool ObjectGUI::GetVisible() {

	return EngineData->SceneData->IsGUI && IsVisible;
}

//----------------------------------------------------------------

vector<ObjectFiledsSpecific> ObjectGUI::GetSpecificFiels() {

	SceneSerialize* serializer = new SceneSerialize();

	vector<ObjectFiledsSpecific> result { 
		{"StartPos:", serializer->Vec3Str(StartPos)} ,
		{"SizePanel:", serializer->Vec2Str(SizePanel)}
	};

	if (IsTextBoxComponent)
	{
		vector<ObjectFiledsSpecific> resultTextBox = TextBox->GetSpecificFiels();
		result.insert(result.end(), resultTextBox.begin(), resultTextBox.end());
	}
	if (IsButtonComponent)
	{
		vector<ObjectFiledsSpecific> resultTextBox = ComponentButton->GetSpecificFiels();
		result.insert(result.end(), resultTextBox.begin(), resultTextBox.end());
	}

	return result;
}

void ObjectGUI::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (IndexObjectOwner == -1) //Back GUI
		return;

	SceneSerialize* serializer = new SceneSerialize();

	StartPos = serializer->StrToVec3(filedsSpecific[0].Value);
	SizePanel = serializer->StrToVec2(filedsSpecific[1].Value);

	if (IsTextBoxComponent)
	{
		TextBox->SetSpecificFiels(filedsSpecific);
	}
	if (IsButtonComponent)
	{
		ComponentButton->SetSpecificFiels(filedsSpecific);
	}
	
	GetShapeSquare()->SetSizeControl(vec3(SizePanel.x, SizePanel.y, 1));
}

//----------------------------------------------------------------

void ObjectGUI::Click() {

	if (IsTextBoxComponent)
	{
		IsChecked = !IsChecked;
		if (IsChecked)
			ActionObjectCurrent = Woking;
		else
			ActionObjectCurrent = Stay;
		return;
	}
	if (IsButtonComponent) {
		if (ComponentButton->IsToogleButon) {
			IsChecked = !IsChecked;
		}
	}

	ActionObjectCurrent = Woking;
}

void ObjectGUI::ActionWork() {

	if (IsTextBoxComponent)
	{
		TextBox->ActionWork();
		return;
	}

	MaterialData.Color = m_color_work;
	ActionObjectCurrent = Stay; //Off

	if (IsButtonComponent)
	{
		ComponentButton->ActionWork();
	}
}


float ObjectGUI::GetTopLayer() {
	
	if (ShellIndex != -1)
	{
		if (Shell != nullptr && Shell->RootObjIndex != -1 && Shell->RootObj != nullptr) {
			shared_ptr<ObjectGUI> objGUI = std::dynamic_pointer_cast<ObjectGUI>(Shell->RootObj);
			return objGUI->StartPos.y;
		}
	}
	return StartPos.y;
}

float ObjectGUI::GetZ() {

	return StartPos.z;
}

void ObjectGUI::SetZ(float z)
{
	if (IndexObjectOwner == -1)
		return;
	StartPos.z = z;
}

string ObjectGUI::GetInfo() {

	//std::cout << "<<GET info>> " << Name << "\n";
	string info = ObjectData::GetInfo();

	std::stringstream ss;
	//ss << info << "	TOP: " << StartPos.y;
	//ss << info << "\nZ: " << StartPos.z << "  T: " << StartPos.y;
	ss << info << "\n							Z: " << StartPos.z << "  T: " << GetTopLayer();
	return ss.str();
}


//=============== Cursor GUI

void ObjectGUI::SaveNewPosition() {

	//--- Set position Cursor
	vec3 mouse = EngineData->Oper->PositionCursorWorld;
	vec2 startPosRect = Shape->GetStartPositWorld();

	bool notX = false;
	bool notY = false;
	float k = 0.2;

	//---- TEST -- ?? fixed frozen cursor
	//if (mouse.z > 0 || startPosRect.x != startPosRect.x)
	if (mouse.z > 0)
	{
		return;
	}

	//=========================================
	float stepFactorX = mouse.x / startPosRect.x;
	float stepFactorY = mouse.y / startPosRect.y;

	float stepX = mouse.x - startPosRect.x;
	float stepY = startPosRect.y - mouse.y;
	stepX *= k;
	stepY *= k;

	float limit = 0.001;
	//---- TEST -- ?? fixed frozen cursor
	if (stepX <limit && stepX > -limit && stepY < limit && stepY > -limit) {
		return;
	}

	vec2 sizeBack = EngineData->SceneData->SizeBackgroungGUI;
	float offSet = (sizeBack.x - 2) / 2;
	float limitBordMin = 0.01 - offSet;
	float limitBordMax = 1.98 + offSet;

	//---- TODO
	float leftBackFactor = (sizeBack.x - BACKGROUND_GUI_WIDTH_F);
	limitBordMax += leftBackFactor;

	float corrBord = 0.02;
	if (StartPos.x + stepX < limitBordMin) {
		StartPos.x = limitBordMin + corrBord;
		notX = true;
	}
	if (StartPos.x + stepX > limitBordMax) {
		StartPos.x = limitBordMax - corrBord;
		notX = true;
	}
	if (StartPos.y + stepY < limitBordMin) {
		StartPos.y = limitBordMin + corrBord;
		notY = true;
	}
	if (StartPos.y + stepY > limitBordMax) {
		StartPos.y = limitBordMax - corrBord;
		notY = true;
	}

	if (!notX)
		StartPos.x += stepX;
	if (!notY)
		StartPos.y += stepY;
}
