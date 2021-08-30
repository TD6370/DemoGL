#pragma once

#include "AspectCreateControlsGUI.h"

#include "../AspectBase.h"
#include "../RoomUseInterface.h"
#include "../../CoreSettings.h"
#include "../../SceneConstructor.h"

//------------------------------
#include "../../ObjectsTypes\ObjectData.h"
#include "../../ObjectsTypes\ObjectGUI.h"

#include "../../GeomertyShapes//ShapeBase.h"
#include "../AspectDispatcherCommands.h"

#include "../../CreatorModelData.h"
#include "../../ShellObjects/BaseShell.h"
#include "../../ObjectsTypes\ObjectEditBox.h"

void AspectCreateControlsGUI::Config() {

}

void AspectCreateControlsGUI::Init() {

}

void AspectCreateControlsGUI::Init(RoomUseInterface* base) {

	m_base = base;
}

void AspectCreateControlsGUI::Work() {

	//EventSelectedInfoCreateObject();

}


//--- Event selected info create object
void  AspectCreateControlsGUI::EventSelectedInfoCreateObject() {

	//---- Save pos cursor ray for World create
	if (Scene->Storage->SceneData->IsGUI == false)
		SaveEditObjectWoldInfo();

	CommandPack* commButtonCreate = Scene->ObjectCurrent->SceneCommand;

	//-- is Button - Start Create object
	if (commButtonCreate->CommandType != TypeCommand::SelectPosForObject)
		return;

	//---- KEY COMMAND CREATE  (FOR WORLD)
	bool isPressUse = Scene->Storage->Inputs->Key == Scene->Storage->Inputs->Default_KeyUse && Scene->Storage->Inputs->Action == GLFW_PRESS;
	if (isPressUse) {

		//-- run command in Button create object
		if (!Scene->ObjectCurrent->SceneCommand->Enable)
			Scene->ObjectCurrent->SceneCommand->Enable = true;
	}
	//------------------------

	//--- set position cursor for create objects (not GUI)
	if (Scene->Storage->SceneData->IndexObjectFieldsEdit > 0)
	{
		int selectedType = commButtonCreate->ValueI;
		if (selectedType != -1 && Scene->IsValidTypeObjectWorld((TypeObject)selectedType))
			commButtonCreate->ValueV4 = vec4(CursorRayPos, 1);
	}

	CommandPack* command = &Scene->CurrentSceneCommand;
	if (!command->Enable || command->CommandType != SelectItemValue)
		return;

	if (((int)command->ValueF) != TypeCommand::CreateObject)
		return;

	Scene->ReadCommand(SelectItemValue);

	//--- set to Button "Create object" -> selected type Object
	commButtonCreate->ValueS = command->Description;
	commButtonCreate->ValueI = command->ValueI;
	commButtonCreate->ValueF = command->ValueF;
	commButtonCreate->ValueV4 = vec4(-1);
}

void AspectCreateControlsGUI::EventStartCreateObject() {

	if (!Scene->CurrentSceneCommand.Enable)
		return;

	CommandPack* command = &Scene->CurrentSceneCommand;

	if (m_base->State.IsBackgroundFrame || Scene->Storage->SceneData->IsGUI == false)
	{
		int typeCreate = (int)TypeObject::Button; //--default
		typeCreate = (int)TypeObject::ListBox;

		if (Scene->ReadCommand(SelectPosForObject))
		{
			if (command->ValueI != -1)
				typeCreate = command->ValueI;

			//--- World object
			if (command->ValueV4.w != -1)
			{
				//-- run create World object
				Scene->RunCommandCreateObject((TypeObject)typeCreate, command->ValueS, command->ValueV4, false);
			}
			else if (m_base->State.IsBackgroundFrame)
			{
				//--- Set background command - select pos for Create control ---
				SetCommand(Scene->ObjectCurrent, TypeCommand::SelectedPosForObject, typeCreate, command->ValueS);
			}
		}

		//--- position selected
		if (m_base->State.IsBackgroundFrame && Scene->ReadCommand(SelectedPosForObject))
		{
			string typeObjectText = command->ValueS;

			if (command->ValueI != -1)
				typeCreate = command->ValueI;

			Scene->RunCommandCreateObject((TypeObject)typeCreate, typeObjectText, command->ValueV4, true);

			SetCommand(Scene->ObjectCurrent, TypeCommand::None);
		}
	}

	if (command->TargetIndex == Scene->ObjectCurrent->Index) {

		//--- position selected and Object created
		if (Scene->ReadCommand(ObjectCreated))
		{
			//--- set new position object created
			m_base->IndexObjectCreating = m_base->IndexObjectSelected = command->TargetIndex;
			Scene->ObjectCurrent->ActionObjectCurrent = ActionObject::Moving;
			m_base->State.IsCreatingObject = true;
		}
	}
}


void AspectCreateControlsGUI::EventEndCreateObject() {

	auto objGUI = Scene->ObjectCurrent;

	if (!m_base->State.IsCreatingObject)
		return;

	CommandPack* command = &Scene->CurrentSceneCommand;

	//--------------------------- Translate Caption to Button root
	if (Scene->IsHaveShell && !command->Enable)
	{
		if (Scene->ShellCurrent->RootObjIndex == m_base->IndexObjectCreating &&
			Scene->ShellCurrent->CaptionObjIndex == objGUI->Index)
		{
			auto objTextBox = std::dynamic_pointer_cast<ObjectTextBox>(objGUI);
			if (objTextBox != nullptr && objTextBox->TypeObj == TextBox)
			{
				Scene->AddCommand(TypeCommand::ObjectReading,
					objGUI->Index,
					m_base->IndexObjectCreating, -1, -1, vec4(-1),
					objTextBox->Message,
					"",
					true);
			}
		}
	}
	//---------------------------

	bool isValidObj = objGUI->Index == m_base->IndexObjectCreating;
	if (!isValidObj)
		return;

	if (objGUI->ActionObjectCurrent == Moving)
		return;

	string textButton = "";

	//--- position selected
	if (Scene->ReadCommand(ObjectReading))
	{
		textButton = command->ValueS;
	}

	//--- obj created Control position
	m_base->IndexObjectCreating = -1;

	if (Scene->IsHaveShell && Scene->ShellCurrent->CaptionObjIndex != -1 &&
		Scene->ShellCurrent->CaptionObj->TypeObj != EditBox)
	{
		//--- Start Renaming created control
		Scene->AddCommand(TypeCommand::RenameObject,
			objGUI->Index,
			Scene->Storage->SceneData->IndexBaseEditBox,
			-1, 0, vec4(objGUI->StartPos, 1),
			textButton);

		//--- Start read name TextBox creating control
		Scene->AddCommand(TypeCommand::ObjectReading, objGUI->Index,
			Scene->ShellCurrent->CaptionObjIndex);

	}
	else {
		m_base->State.IsCreatingObject = false;
	}
}


//=================== Event Edit Object fields ====================

void AspectCreateControlsGUI::EventFillFieldsEdit() {

	if (m_base->State.IsCreatingObject)
		return;

	shared_ptr<ObjectData> obj = Scene->ObjectCurrent;

	//--- Create  "ObjectFieldsEdit"
	if (Scene->Storage->SceneData->IndexObjectFieldsEdit == -1) {
		vec3 pos = vec3(.7, .1, Scene->Storage->StartPosGUI_Z);
		Scene->Storage->SceneData->IndexObjectFieldsEdit = -2;
		Scene->RunCommandCreateObject(TypeObject::ObjectFieldsEdit, "ObjectFieldsEdit", pos);

		pos = vec3(.02, .3, Scene->Storage->StartPosGUI_Z);
		Scene->RunCommandCreateObject(TypeObject::ListBox, Scene->CommandsAttribute.TypesObjectListCommand, pos);
	}

	if (Scene->Storage->SceneData->IndexObjectFieldsEdit < 0)
		return;

	int index = obj->Index;
	int indexEditObj = -1;

	//------ Save ray Object selected
	if (!m_base->State.IsEditControls)
		SaveEditObjectWoldInfo();

	indexEditObj = m_base->IndexObjectSelected;

	if (indexEditObj == -1)
		return;

	//---- save edit control to temp
	if (indexEditObj == index && indexEditObj != m_base->IndexObjectSelectedEdit)
	{
		ObjectSelectedEdit = obj;
		m_base->IndexObjectSelectedEdit = index;
		return;
	}

	//-- is cashed Selected object
	if (indexEditObj == m_base->IndexObjectSelectedEdit) {


		//---- Fill control
		if (index == Scene->Storage->SceneData->IndexObjectFieldsEdit) {

			bool isFillFields = obj->SceneCommand->TargetIndex != indexEditObj;
			if (!isFillFields) {				//==================== Save fields object
				if (!EventSaveFieldsEdit())
					return;
			}

			string resultFieldsAndValues = string();

			std::shared_ptr<ObjectGUI> objItem_Button_EditBox;
			int indItemNext = -1;

			//--- List Edit box
			shared_ptr<BaseShell> shell = Scene->ShellCurrent;

			if (isFillFields)
				Scene->CreateObjectListFieldValue(ObjectSelectedEdit);

			objItem_Button_EditBox = std::dynamic_pointer_cast<ObjectGUI>(shell->HeadObj);
			indItemNext = objItem_Button_EditBox->Index;

			// --- info Shell base
			int indShell_ListFieldsEdit = objItem_Button_EditBox->SceneCommand->SourceIndex;
			//map<string, string> listObjFieldsValue = Scene->GetObjectListFieldValue(ObjectSelectedEdit);

			while (indItemNext != -1)
			{
				//--- Button (objItemEditBoxControl)
				std::shared_ptr<ObjectEditBox> objItemEditBox = std::dynamic_pointer_cast<ObjectEditBox>(objItem_Button_EditBox->Shell->CaptionObj);

				//--- Set value
				string fieldName = objItemEditBox->SceneCommand->Description;
				if (fieldName.size() != 0)
				{
					if (isFillFields) {
						/*if (fieldName[fieldName.size() - 1] == ':')
							fieldName.erase(fieldName.end() - 1);*/

							//--- SET VALUE -- FROM TYPE FIELD
						objItemEditBox->Message = Scene->GetObjectValueByFieldName(fieldName);
						objItemEditBox->UpdateMessage();
						//objItemEditBox->Message = listObjFieldsValue[typeFieldName];
					}
					else {
						//=== SAVE FIELDS - TO OBJECT
						fieldName += ": ";
						resultFieldsAndValues.append(fieldName);
						resultFieldsAndValues.append(objItemEditBox->Message);
						resultFieldsAndValues.append("\n");
					}
				}

				indItemNext = objItem_Button_EditBox->NextItemShellIndex;
				if (indItemNext != -1)
					objItem_Button_EditBox = std::dynamic_pointer_cast<ObjectGUI>(objItem_Button_EditBox->NextItemShellObj);
			}

			if (isFillFields) {
				//-- Save edit object in Control Edit
				obj->SceneCommand->TargetIndex = indexEditObj;
			}
			else {
				//-- Save list to Object
				Scene->SaveObjectFieldValueFromList(ObjectSelectedEdit, resultFieldsAndValues);
			}
		}
		//----------

	}
}


//================== Event Save Object fields[SaveObjectFieldsEdit] ====================

bool AspectCreateControlsGUI::EventSaveFieldsEdit() {

	if (Scene->ReadCommand(SaveObjectFieldsEdit))
	{
		return true;
	}
	return false;
}

void AspectCreateControlsGUI::SaveEditObjectWoldInfo() {

	if (Scene->ObjectCurrent->Index == Scene->Storage->SceneData->IndexCursorRayObj) {
		m_base->IndexObjectSelected = Scene->ObjectCurrent->SceneCommand->TargetIndex;
		CursorRayPos = Scene->ObjectCurrent->Postranslate;
	}
}