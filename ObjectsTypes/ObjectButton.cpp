#include "ObjectButton.h"

#include "../GeomertyShapes//ShapeBase.h"
#include "ObjectData.h"
//#include "ObjectGUI.h"
//#include "ObjectPhysic.h"
#include "../CoreSettings.h"
//#include "../CreatorModelData.h"
#include "../Serialize/SceneSerialize.h"
#include "../OperationString.h"
#include "../Rooms/AspectDispatcherCommands.h"
#include "../CreatorModelData.h"

ObjectButton::~ObjectButton()
{

}


void ObjectButton::InitData()
{
	ObjectGUI::InitData();

	if (Name == EngineData->SceneData->NameSystemEditBox)
	{
		EngineData->SceneData->IndexBaseEditBox = Index;
	}
}

void ObjectButton::RunAction() {
	
	ObjectGUI::RunAction();

	DefaultView();
}


void ObjectButton::Click() {

	ObjectGUI::Click();

	if (IsToogleButon) {
		IsChecked = !IsChecked;
	}
}

void ObjectButton::DefaultView() {
	if (IsToogleButon)
	{
		if (IsChecked)
			ParamValue = AnimationParam->StartPressedParamShaderID;
	}
}

void ObjectButton::Refresh()
{
	
}


void ObjectButton::ActionWork() {

	Color = m_color_work;

	bool isValidStartCommand = SceneCommand->CommandType != TypeCommand::None;
	if (isValidStartCommand) {
		SceneCommand->Enable = true;
		if (IsToogleButon)
		{
			UpdateCommandOptions(this, Name, (int)IsChecked);
		}
	}

	ActionObjectCurrent = Stay; //Off
}
// ----------------------------------------------------------------

vector<ObjectFiledsSpecific> ObjectButton::GetSpecificFiels() {

	vector<ObjectFiledsSpecific> result = ObjectGUI::GetSpecificFiels();
	result.push_back({ "IsToogleButon:", BoolToStr(IsToogleButon) });
	//result.push_back({ "IsChecked:", BoolToStr(IsChecked) });
	//result.push_back({ "Command:", std::to_string(SceneCommand->CommandType) });
	return result;
}

void ObjectButton::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (IndexObjectOwner == -1) //Back GUI
		return;

	string valueStr;
	ObjectGUI::SetSpecificFiels(filedsSpecific);

	valueStr = GetSpecifValue(filedsSpecific, 1);
	IsToogleButon = StrToBool(valueStr);

	//valueStr = GetSpecifValue(filedsSpecific, 3);
	//IsToogleButon = StrToBool(valueStr);
	//valueStr = GetSpecifValue(filedsSpecific, 2);
	//IsChecked = StrToBool(valueStr);
	//int valueInt = std::stoi(GetSpecifValue(filedsSpec//ific, 1));
	//SceneCommand->CommandType = (TypeCommand)valueInt;
}

//----------------------------------------------------------------
