#include "ButtonComponent.h"
#include "./GUIComponent.h"

#include "../GeomertyShapes//ShapeBase.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../CoreSettings.h"
#include "../Serialize/SceneSerialize.h"
#include "../OperationString.h"
#include "../Rooms/AspectDispatcherCommands.h"
#include "../CreatorModelData.h"

void ButtonComponent::Init(ObjectData* obj) {

	m_obj = obj;

	/*m_obj->IsSquareModel = true;
	m_obj->IsAbsolutePosition = true;
	m_obj->IsFocusable = false;*/

	if (m_obj->Name == m_obj->EngineData->SceneData->NameSystemEditBox)
	{
		m_obj->EngineData->SceneData->IndexBaseEditBox = m_obj->Index;
	}
}

void ButtonComponent::ActionWork() {

	m_obj->MaterialData.Color = m_color_work;

	bool isValidStartCommand = m_obj->SceneCommand->CommandType != TypeCommand::None;
	if (isValidStartCommand) {
		m_obj->SceneCommand->Enable = true;
		if (IsToogleButon)
		{
			UpdateCommandOptions(m_obj, m_obj->Name, (int)m_obj->IsChecked);
		}
	}

	m_obj->ActionObjectCurrent = Stay; //Off
}


void ButtonComponent::DefaultView() {
	if (IsToogleButon)
	{
		if (m_obj->IsChecked) {
			if (m_obj->SceneCommand->CommandType == TypeCommand::EditObjectCommand)
				m_obj->ParamValue = m_obj->AnimationParam->StartResizeParamShaderID;
			else
				m_obj->ParamValue = m_obj->AnimationParam->StartPressedParamShaderID;
		}
	}
}

vector<ObjectFiledsSpecific> ButtonComponent::GetSpecificFiels() {

	/*vector<ObjectFiledsSpecific> result = ObjectGUI::GetSpecificFiels();
	result.push_back({ "IsToogleButon:", BoolToStr(IsToogleButon) });
	return result;*/

	vector<ObjectFiledsSpecific> result = m_obj->ComponentGUI->GetSpecificFiels();
	result.push_back({ "IsToogleButon:", BoolToStr(IsToogleButon) });
	return result;
}

void ButtonComponent::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (m_obj->IndexObjectOwner == -1) //Back GUI
		return;

	string valueStr;
	//ObjectGUI::SetSpecificFiels(filedsSpecific);
	m_obj->ComponentGUI->SetSpecificFiels(filedsSpecific);
	
	valueStr = m_obj->GetSpecifValue(filedsSpecific, 1);
	IsToogleButon = StrToBool(valueStr);

	if (m_obj->IndexObjectOwner == -1) //Back GUI
		return;
}

