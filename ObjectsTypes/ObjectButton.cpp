#include "ObjectButton.h"

#include "../GeomertyShapes//ShapeBase.h"
#include "ObjectData.h"
//#include "ObjectGUI.h"
//#include "ObjectPhysic.h"
#include "../CoreSettings.h"
//#include "../CreatorModelData.h"
#include "../Serialize/SceneSerialize.h"
#include "../OperationString.h"

ObjectButton::~ObjectButton()
{

}


void  ObjectButton::InitData()
{
	ObjectGUI::InitData();
		
}

void ObjectButton::RunAction() {
	
	ObjectGUI::RunAction();

	UpdateState();
}

void ObjectButton::UpdateState() {

	//Updtae animation
	if (IsToogleButon)
	{
		if (IsToogleButonOn)
			ParamCase = AnimationParam->StartPressedParamShaderID;
	}
}


void ObjectButton::Work() {

	ObjectGUI::Work();

	Color = m_color_work;

	if (IsToogleButon)
		IsToogleButonOn = !IsToogleButonOn;

	SceneCommand->Enable = true;
	SceneCommand->SourceIndex = Index;

	ActionObjectCurrent = Stay; //Off
}
// ----------------------------------------------------------------

vector<ObjectFiledsSpecific> ObjectButton::GetSpecificFiels() {

	vector<ObjectFiledsSpecific> result = ObjectGUI::GetSpecificFiels();
	result.push_back({ "IsToogleButon:", BoolToStr(IsToogleButon) });
	result.push_back({ "IsToogleButonOn:", BoolToStr(IsToogleButonOn) });
	return result;
}

void ObjectButton::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (IndexObjectOwner == -1) //Back GUI
		return;

	string valueStr;
	ObjectGUI::SetSpecificFiels(filedsSpecific);

	valueStr = GetSpecifValue(filedsSpecific, 2);
	IsToogleButon = StrToBool(valueStr);
	valueStr = GetSpecifValue(filedsSpecific, 1);
	IsToogleButonOn = StrToBool(valueStr);
}

//----------------------------------------------------------------
