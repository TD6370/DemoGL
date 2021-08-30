#pragma once

#include "AspectUseControlsGUI.h"

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

void AspectUseControlsGUI::Config() {

}

void AspectUseControlsGUI::Init() {

}

void AspectUseControlsGUI::Init(RoomUseInterface* base) {

	m_base = base;
}

void AspectUseControlsGUI::Work() {

}


//===================== Event On/Off Edit mode controls ===========================

void AspectUseControlsGUI::ModeEditControls()
{
	CommandPack* command = &Scene->CurrentSceneCommand;
	if (!command->Enable)
		return;

	if (Scene->ReadCommand(EditGUI_OnOff))
	{
		if (command->Options.size() != 0)
			m_base->State.IsEditControls = command->Options["ButtonEditOn"];
		if (m_base->State.IsEditControls)
			m_base->IndexObjectSelected = -1;
	}
}


//==================================== START CLICK 
void  AspectUseControlsGUI::EventStartClickControl() {

	auto obj = Scene->ObjectCurrent;

	if (m_base->State.IsEditControls && obj->IsTransformable)
		return;
	if (!obj->IsFocusable)
		return;
	if (!m_base->State.IsCursorClickEvent)
		return;
	if (!m_base->State.IsFocused)
		return;
	if (obj->ActionObjectCurrent == ActionObject::Transforming)
		return;
	if (m_base->IndexObjectCreating != -1)
		return;
	if (m_base->State.IsCreatingObject)
		return;

	obj->Click();
	m_base->SetCurrentEventParam(obj, m_base->AnimationParams->StartClickParamShaderID);

	m_base->SetTimeAnimate(obj, 0.3);

	m_base->State.IsCursorClickEvent = false;
}

//===================== Event Rename controls ===========================

void AspectUseControlsGUI::EventStartRenameObject() {

	auto objGUI = Scene->ObjectCurrent;

	CommandPack* command = &Scene->CurrentSceneCommand;
	//--- obj base Edit box
	bool isSystemButtonEditBox = objGUI->Index == Scene->Storage->SceneData->IndexBaseEditBox;

	//------- set position System Edit box 
	if (command->CommandType == RenameObject)
	{
		if (!command->Enable)
			return;
		if (!isSystemButtonEditBox)
			return;
		if (m_base->IndexObjectCreating != -1)
			return;

		Scene->ReadCommand(RenameObject);
		objGUI->StartPos = vec3(command->ValueV4.x, command->ValueV4.y, objGUI->StartPos.z);
		objGUI->IsVisible = true;

		Scene->RefreshGUI();

		//-- Start - click Button edit box system
		objGUI->SceneCommand->ValueS = command->ValueS;
		objGUI->Click();
	}

	auto objTextBox = std::dynamic_pointer_cast<ObjectTextBox>(objGUI);

	if (command->TargetIndex == objGUI->Index)
	{
		if (objGUI->TypeObj == TextBox || objGUI->TypeObj == EditBox) {
			if (Scene->ReadCommand(ObjectReading))
			{
				//TextBox renaming control - START
				objGUI->Click();
			}
		}
	}

	if (objGUI->ActionObjectCurrent == ActionObject::Woking)
	{

		if (objTextBox != nullptr && objTextBox->TypeObj == TextBox)
		{
			if (Scene->ReadCommand(KeyInputCommand))
			{
				// Import Message from EditBox
				objTextBox->Message = command->ValueS;
				objTextBox->UpdateMessage();
			}
		}
	}
}


//--- Use = Message - EditBox & TextBox
void AspectUseControlsGUI::EventReadKeyInput() {

	auto objGUI = Scene->ObjectCurrent;

	bool isPressEscape = Scene->Storage->Inputs->Key == GLFW_KEY_ESCAPE && Scene->Storage->Inputs->Action == GLFW_PRESS;
	bool isPressStop = Scene->Storage->Inputs->Key == m_endEditKey && Scene->Storage->Inputs->Action == GLFW_PRESS;
	bool isPressKey = Scene->Storage->Inputs->Action == GLFW_PRESS;

	//---------- Complete edit
	if (isPressStop) {
		if (objGUI->ActionObjectCurrent == ActionObject::Woking) {

			if (objGUI->TypeObj == EditBox)
			{
				//---- Stop edit box
				objGUI->Click();

				//--- Stop Button edit box
				Scene->AddCommand(StopWorking, objGUI->Index, objGUI->IndexObjectOwner); //--2.* 
				return;
			}
			if (objGUI->TypeObj == TextBox)
			{
				objGUI->Click();
				objGUI->DefaultView();
			}
		}
	}

	if (objGUI->TypeObj != EditBox)
		return;
	if (objGUI->ActionObjectCurrent != ActionObject::Woking)
		return;
	if (!Scene->IsHaveShell)
		return;
	int indexEditBox = Scene->ShellCurrent->CaptionObjIndex;
	if (objGUI->Index != indexEditBox)
		return;
	if (isPressEscape || isPressStop)
		return;
	if (!isPressKey)
		return;

	//--- Event inputs in text box
	auto objEditBox = std::dynamic_pointer_cast<ObjectEditBox>(objGUI);

	if (objEditBox != nullptr) {
		objEditBox->AddSymbolMessage(Scene->SymbolInput);
	}

	//-- Translate Message EditBox -> TextBox 
	Scene->AddCommand(TypeCommand::KeyInputCommand,
		objGUI->Index,
		-1, -1, -1, vec4(),
		objEditBox->Message);
}


//===================== Event Work Edit box controls ===========================

void AspectUseControlsGUI::EventEditTextControl() {

	auto objGUI = Scene->ObjectCurrent;
	//----------- Start edit
	CommandPack* command = &Scene->CurrentSceneCommand;
	if (!command->Enable)
		return;
	if (command->TargetIndex != objGUI->Index)
		return;

	int isChecked = command->ValueI;
	bool isSystemEditBox = objGUI->Index == Scene->Storage->SceneData->IndexBaseEditBox;

	if (Scene->ReadCommand(EditObjectCommand))
	{
		if (isChecked)
		{
			auto objEditBox = std::dynamic_pointer_cast<ObjectEditBox>(objGUI);
			if (objGUI->TypeObj == EditBox && command->ValueS.size() != 0) {
				objEditBox->Message = command->ValueS;
				objEditBox->UpdateMessage();
			}

			//Start edit box
			m_base->IndexObjectSelected = objGUI->Index;
			objGUI->Click(); //--1.*
		}
		else {
			if (objGUI->ActionObjectCurrent == ActionObject::Woking)
			{
				//Stop edit box
				objGUI->Click();	//--4.*
				m_base->IndexObjectSelected = -1;
			}
		}
	}

	if (Scene->ReadCommand(StopWorking))
	{
		//Stop button frame 
		objGUI->Click();	//--3.*
		m_base->IndexObjectSelected = -1;

		//End rename new control
		if (isSystemEditBox) {

			//-- End create control
			if (m_base->State.IsCreatingObject)
				m_base->State.IsCreatingObject = false;

			objGUI->IsVisible = false;
			objGUI->ActionObjectCurrent = ActionObject::Stay;
			Scene->RefreshGUI();
		}
	}
}
