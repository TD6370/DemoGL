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

}

void ObjectButton::RunAction() {
	
	ObjectGUI::RunAction();

	DefaultView();
}


void ObjectButton::Click() {

	ObjectGUI::Click();
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
	/*ObjectData::Refresh();

	if (IsToogleButon && IndexObjectOwner != -1)
	{
		if (IndexObjectOwner == Storage->SceneData->IndexBackgroundGUIObj)
			return;

		auto objOwner = Storage->SceneObjects[IndexObjectOwner];
		if (objOwner->TypeObj != Button)
			return;
		auto objButton = std::dynamic_pointer_cast<ObjectButton>(objOwner);
		if (objButton != nullptr && IsChecked != objButton->IsChecked) {
			Click();
		}
	}*/
}


void ObjectButton::ActionWork() {

	//ObjectGUI::ActionWork();
	
	Color = m_color_work;

	bool isValidStartCommand = SceneCommand->CommandType != TypeCommand::None;
	if (IsToogleButon) {
		IsChecked = !IsChecked;
		//isValidStartCommand = IsChecked; //@???
	}

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
	result.push_back({ "IsChecked:", BoolToStr(IsChecked) });
	result.push_back({ "Command:", std::to_string(SceneCommand->CommandType) });
	return result;
}

void ObjectButton::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (IndexObjectOwner == -1) //Back GUI
		return;

	string valueStr;
	ObjectGUI::SetSpecificFiels(filedsSpecific);

	valueStr = GetSpecifValue(filedsSpecific, 3);
	IsToogleButon = StrToBool(valueStr);
	valueStr = GetSpecifValue(filedsSpecific, 2);
	IsChecked = StrToBool(valueStr);
	int valueInt = std::stoi(GetSpecifValue(filedsSpecific, 1));
	SceneCommand->CommandType = (TypeCommand)valueInt;
}

//----------------------------------------------------------------
