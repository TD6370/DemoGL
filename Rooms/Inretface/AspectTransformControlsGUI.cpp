#pragma once

#include "AspectTransformControlsGUI.h"

#include "../AspectBase.h"
#include "../RoomUseInterface.h"
#include "../../CoreSettings.h"
#include "../../SceneConstructor.h"

//------------------------------
#include "../../ObjectsTypes\ObjectData.h"

#include "../../GeomertyShapes//ShapeBase.h"
#include "../AspectDispatcherCommands.h"

void AspectTransformControlsGUI::Config() {

	auto obj = Scene->ObjectCurrent;

	if (obj->TypeObj == TypeObject::CursorGUI) {
		CursorMovePos = obj->StartPos;
	}
}

void AspectTransformControlsGUI::Init() {

}

void AspectTransformControlsGUI::Init(RoomUseInterface* base) {

	m_base = base;
}

void AspectTransformControlsGUI::Work() {

	auto obj = Scene->ObjectCurrent;

	//--- Moving to Cusror position
	EventMovingControl();

	//--- Resize control to Cusror position
	EventResizeControl();

	//--- Focus box & focus Border
	//CheckFocusBoxAndBorderControl();

	//----Start event Click Control
	//EventStartClickControl();

	//-- End event resize control
	EventEndResizeControl();
	//-- Start event resize colntrol
	EventStartResizeControl();

	//-- End event on control
	EventEndMovingControl();
	//----Start event Moving Control
	EventStartMovingControl();
}


//==================================== START MOVE
void  AspectTransformControlsGUI::EventStartMovingControl() {

	auto obj = Scene->ObjectCurrent;
	if (!m_base->State.IsEditControls)
		return;
	if (!obj->IsTransformable)
		return;
	if (!m_base->State.IsCursorClickEvent)
		return;
	if (!m_base->State.IsFocused)
		return;
	if (obj->ActionObjectCurrent == ActionObject::Transforming)
		return;

	if (m_base->IndexObjectSelected != -1)	//-- when click after order
		return;

	Scene->Debug("Start moving");
	obj->ActionObjectCurrent = ActionObject::Moving;
	m_base->IndexObjectSelected = obj->Index;
	SelectObjectOffsetPos = CursorMovePos - obj->StartPos;

	m_base->SetCurrentEventParam(obj, m_base->AnimationParams->StartMoveParamShaderID);

	m_base->State.IsCursorClickEvent = false;//!!!
}

#pragma region MOVE

//----------------------- MOVE
void  AspectTransformControlsGUI::EventMovingControl() {

	auto obj = Scene->ObjectCurrent;

	if (m_base->IndexObjectSelected != obj->Index)
		return;
	if (!obj->IsTransformable)
		return;
	if (obj->ActionObjectCurrent != ActionObject::Moving)
		return;

	//Scene->Debug("GUI moving");
	vec3 newPos = CursorMovePos - SelectObjectOffsetPos;
	obj->StartPos = vec3(newPos.x, newPos.y, obj->StartPos.z);

}

//----------------------- END MOVE
bool AspectTransformControlsGUI::EventEndMovingControl() {

	auto obj = Scene->ObjectCurrent;

	if (!m_base->State.IsCursorClickEvent)
		return false;

	if (m_base->IndexObjectSelected != obj->Index)	//-- when click after order
		return false;

	if (obj->ActionObjectCurrent != ActionObject::Moving)
		return false;

	//Scene->Debug("End moving");
	m_base->IndexObjectSelected = -1;
	obj->ActionObjectCurrent = ActionObject::Stay;
	SelectObjectOffsetPos = vec3(0);

	//TODO:
	m_base->State.IsCursorClickEvent = false; //---- VVVV
	m_base->SetCurrentEventParam(obj, m_base->AnimationParams->StartDefaultParamShaderID);
	return true;
}

#pragma endregion

//==================================== START RESIZE
void AspectTransformControlsGUI::EventStartResizeControl() {

	auto obj = Scene->ObjectCurrent;
	if (!m_base->State.IsEditControls)
		return;
	if (!obj->IsTransformable)
		return;
	if (!obj->IsFocusable)
		return;
	if (!m_base->State.IsCursorClickEvent)
		return;
	if (!m_base->State.IsFocused)
		return;
	if (obj->ActionObjectCurrent == ActionObject::Moving)
		return;
	if (m_base->IndexObjectSelected != -1)	//-- when click after order
		return;
	if (!m_base->State.IsCheckBorder)
		return;

	SelectObjectOffsetPos = CursorMovePos - obj->StartPos;

	//Scene->Debug("Start resize");
	obj->ActionObjectCurrent = ActionObject::Transforming;
	obj->MaterialData.Color = m_palette.color_resize;
	m_base->IndexObjectSelected = obj->Index;
	m_startSizePanel = obj->SizePanel;
	m_base->State.IsCursorClickEvent = false;//!!!
	m_base->SetCurrentEventParam(obj, m_base->AnimationParams->StartResizeParamShaderID);
}

//----------------------- RESIZE
void  AspectTransformControlsGUI::EventResizeControl() {
	auto  obj = Scene->ObjectCurrent;

	if (!m_base->State.IsEditControls)
		return;
	if (m_base->IndexObjectSelected != obj->Index)
		return;
	if (!obj->IsFocusable)
		return;
	if (!obj->IsTransformable)
		return;
	if (obj->ActionObjectCurrent != ActionObject::Transforming)
		return;

	//Scene->Debug("GUI resize");
	float sizePanelX = CursorMovePos.x - obj->StartPos.x;
	float sizePanelY = CursorMovePos.y - obj->StartPos.y;
	float kBord = 0.15;
	sizePanelX += m_sizeBorder * kBord;
	sizePanelY += m_sizeBorder * kBord;

	if (sizePanelX < 0.01 || sizePanelY < 0.01)
		return;

	obj->SizePanel.x = sizePanelX;
	obj->SizePanel.y = sizePanelY;
	obj->SizeControlUpdate();
}

//----------------------- END RESIZE
bool AspectTransformControlsGUI::EventEndResizeControl() {
	auto obj = Scene->ObjectCurrent;

	if (!m_base->State.IsEditControls)
		return false;
	if (!obj->IsFocusable)
		return false;
	if (!m_base->State.IsCursorClickEvent)
		return false;
	if (obj->ActionObjectCurrent != ActionObject::Transforming)
		return false;
	if (m_base->IndexObjectSelected != obj->Index)
		return false;

	//Scene->Debug("End resize");
	m_base->IndexObjectSelected = -1;
	obj->ActionObjectCurrent = ActionObject::Stay;
	SelectObjectOffsetPos = vec3(0);

	m_base->State.IsCursorClickEvent = false;
	m_base->SetCurrentEventParam(obj, m_base->AnimationParams->StartDefaultParamShaderID);
	return true;
}