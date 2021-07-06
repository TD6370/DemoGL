﻿#include "RoomUseInterface.h"

//#include "..\GeometryLib.h"
#include "..\CoreSettings.h"
#include "..\SceneConstruction.h"
#include "..\ObjectsTypes\ObjectData.h"
#include "..\ObjectsTypes\ObjectGUI.h"
#include "..\CreatorModelData.h"
#include "..\ModelData.h"
#include "../GeomertyShapes//ShapeBase.h"
#include "AspectDispatcherCommands.h"

RoomUseInterface::~RoomUseInterface() {
}

void RoomUseInterface::Config() {

}

void RoomUseInterface::Init() {
	
	AnimationParams = new AnimationParamGUI();
	//color_selected = color_yelow;
	color_selected = color_yelow;
	auto winHeight = Scene->m_heightWindow;
	auto winHWidth = Scene->m_widthWindow;
	m_projectionPerspective = glm::perspective(45.0f, (float)(winHeight) / (float)(winHeight), 0.1f, 1000.0f);
	//IsEditControls = true;
	IsEditControls = false;


}

void RoomUseInterface::SetCurrentEventParam(shared_ptr<ObjectGUI> obj, int value)
{
	if (EndTimer > 0) {
		auto currTimer = glfwGetTime();
		if (currTimer < EndTimer)
			return;
		EndTimer = -1;
	}

	if (obj->IsFocusable) 
		obj->ParamValue = value;
	else if (AnimationParams->StartDefaultParamShaderID == value) {
		obj->DefaultView();
	}

	m_CurrentStartedEventID = value;
}

void RoomUseInterface::SetTimeAnimate(shared_ptr<ObjectGUI> obj, float timeAnim)
{
	obj->SetStartTimer();
	StartTimer = glfwGetTime();
	EndTimer = StartTimer + (timeAnim * 1);
}

//==================================== START MOVE
void  RoomUseInterface::EventStartMovingControl(std::shared_ptr<ObjectGUI> obj) {
	if (!IsEditControls)
		return;
	if (!obj->IsTransformable)
		return;
	if (!IsCursorClickEvent)
		return;
	if (!IsFocused)
		return;
	if(obj->ActionObjectCurrent == ActionObject::Transforming)
		return;

	if (IndexObjectSelected != -1)	//-- when click after order
		return;

	Scene->Debug("Start moving");
	obj->ActionObjectCurrent = ActionObject::Moving;
	IndexObjectSelected = obj->Index;
	SelectObjectOffsetPos = CursorMovePos - obj->StartPos;

	SetCurrentEventParam(obj, AnimationParams->StartMoveParamShaderID);

	IsCursorClickEvent = false;//!!!
}

#pragma region MOVE

//----------------------- MOVE
void  RoomUseInterface::EventMovingControl(std::shared_ptr<ObjectGUI> obj) {

	//---- TEST
	if (IndexObjectSelected != obj->Index)
		return;
	if(!obj->IsTransformable)
		return;
	if (obj->ActionObjectCurrent != ActionObject::Moving)
		return;

	//Scene->Debug("GUI moving");
	vec3 newPos = CursorMovePos - SelectObjectOffsetPos;
	obj->StartPos = vec3(newPos.x, newPos.y, obj->StartPos.z);
	
}

//----------------------- END MOVE
bool RoomUseInterface::EventEndMovingControl(std::shared_ptr<ObjectGUI> obj) {
	
	if (!IsCursorClickEvent)
		return false;

	if (IndexObjectSelected != obj->Index)	//-- when click after order
		return false;

	if(obj->ActionObjectCurrent != ActionObject::Moving)
		return false;

	//Scene->Debug("End moving");
	IndexObjectSelected = -1;	
	obj->ActionObjectCurrent = ActionObject::Stay;
	SelectObjectOffsetPos = vec3(0);
	
	//TODO:
	IsCursorClickEvent = false; //---- VVVV
	SetCurrentEventParam(obj, AnimationParams->StartDefaultParamShaderID);

	return true;
}

#pragma endregion

//==================================== START RESIZE
void RoomUseInterface::EventStartResizeControl(shared_ptr<ObjectGUI> obj) {
	if (!IsEditControls)
		return;
	if (!obj->IsTransformable)
		return;
	if (!obj->IsFocusable)
		return;
	if (!IsCursorClickEvent)
		return;
	if (!IsFocused)
		return;
	if (obj->ActionObjectCurrent == ActionObject::Moving)
		return;
	if (IndexObjectSelected != -1)	//-- when click after order
		return;
	if (!IsCheckBorder)
		return;

	SelectObjectOffsetPos = CursorMovePos - obj->StartPos;

	//Scene->Debug("Start resize");
	obj->ActionObjectCurrent = ActionObject::Transforming;
	obj->Color = color_resize;
	IndexObjectSelected = obj->Index;
	m_startSizePanel = obj->SizePanel;
	IsCursorClickEvent = false;//!!!
	SetCurrentEventParam(obj, AnimationParams->StartResizeParamShaderID);
}

//----------------------- RESIZE
void  RoomUseInterface::EventResizeControl(shared_ptr<ObjectGUI> obj) {
	if (!IsEditControls)
		return;
	if (IndexObjectSelected != obj->Index)
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
bool RoomUseInterface::EventEndResizeControl(shared_ptr<ObjectGUI> obj) {
	if (!IsEditControls)
		return false;
	if(!obj->IsFocusable)
		return false;
	if (!IsCursorClickEvent)
		return false;
	if (obj->ActionObjectCurrent != ActionObject::Transforming)
		return false;
	if (IndexObjectSelected != obj->Index)
		return false;

	//Scene->Debug("End resize");
	IndexObjectSelected = -1;
	obj->ActionObjectCurrent = ActionObject::Stay;
	SelectObjectOffsetPos = vec3(0);

	IsCursorClickEvent = false;
	SetCurrentEventParam(obj, AnimationParams->StartDefaultParamShaderID);
	return true;
}

//----------------------- FOCUS
void RoomUseInterface::CheckFocusBoxAndBorderControl(std::shared_ptr<ObjectGUI> obj) {

	if (IsBackgroundFrame && Scene->ObjectCurrent->SceneCommand->CommandType == TypeCommand::None)
		return;
	if (Scene->ObjectCurrent->ActionObjectCurrent == Moving || 
		Scene->ObjectCurrent->ActionObjectCurrent == Transforming)
		return;

	bool isCheckOrder = true;
	vec2 endPosRect, startPosRect;
	float zOrder;
	IsFocused = false;
	IsCheckBorder = false;

	obj->Shape->GetPositRect(startPosRect, endPosRect, zOrder);

	//--- Check Focused
	if (CheckPointInRectangle(m_tempMousePosWorld, startPosRect, endPosRect))
	{
		if (IndexObjectFocused != -1 && obj->Index < FocusedOrder)
			isCheckOrder = false;

		if (isCheckOrder) {
			FocusedOrder = obj->Index;
			IndexObjectFocused = obj->Index;
			IsFocused = true;
		}
	}

	if (!obj->IsTransformable)
		return;

	if (isCheckOrder) {
		//--- Check Focused border
		IsCheckBorder = CheckPointInRectangleBorder(m_tempMousePosWorld, startPosRect, endPosRect, m_sizeBorder);
	}
}

void RoomUseInterface::EventFocusControl(std::shared_ptr<ObjectGUI> obj) {
	if (obj->ActionObjectCurrent != Stay)
		return;


	bool isOrderParam = IsCompareF(m_CurrentStartedEventID, AnimationParams->StartResizeParamShaderID);
	bool isUpdateView = obj->IsFocusable || IsEditControls;
	if (IsBackgroundFrame)
		isUpdateView = false;

	if (IsFocused && isUpdateView) {
		obj->Color = color_selected;
		if (!isOrderParam)
		{ 
			if (IsCheckBorder && IsEditControls)
				SetCurrentEventParam(obj, AnimationParams->StartCheckBorderParamShaderID);
			else
				SetCurrentEventParam(obj, AnimationParams->StartFocusParamShaderID);
		}
	}
	else {

		if (IndexObjectFocused == obj->Index) {
			FocusedOrder = -1;
			IndexObjectFocused = -1;
		}
		SetCurrentEventParam(obj, AnimationParams->StartDefaultParamShaderID);

		if(obj->ActionObjectCurrent == ActionObject::Stay)
			obj->DefaultView(); //default color
	}
}

//==================================== START CLICK 
void  RoomUseInterface::EventStartClickControl(std::shared_ptr<ObjectGUI> obj) {
	if (IsEditControls && obj->IsTransformable)
		return;
	if (!obj->IsFocusable)
		return;
	if (!IsCursorClickEvent)
		return;
	if (!IsFocused)
		return;
	if (obj->ActionObjectCurrent == ActionObject::Transforming)
		return;

	//Scene->Debug("Start click");
	obj->Click();
	SetCurrentEventParam(obj, AnimationParams->StartClickParamShaderID);

	SetTimeAnimate(obj, 0.3);
	
	IsCursorClickEvent = false;
}


//===================== Event Create Object ===========================

void RoomUseInterface::EventCreateObject(shared_ptr<ObjectGUI> objGUI) {

	int typeCreate = (int)TypeObject::Button;
	typeCreate = (int)TypeObject::EditBox;

	CommandPack* command = &Scene->CurrentSceneCommand;
	if (!command->Enable)
		return;

	if (IsBackgroundFrame)
	{
		if (Scene->ReadCommand(SelectPosForObject))
		{
			//--- Set background command - select pos for Create control ---
			//Scene->ObjectCurrent->SceneCommand->CommandType = TypeCommand::SelectedPosForObject;
			SetCommand(Scene->ObjectCurrent, TypeCommand::SelectedPosForObject);
			//Scene->ObjectCurrent->SceneCommand->Enable = true;	--- after click
		}

		//--- position selected
		if (Scene->ReadCommand(SelectedPosForObject))
		{
			Scene->AddCommand(TypeCommand::CreateObject, -1, -1, "TypeObject", typeCreate);
			//Scene->ObjectCurrent->SceneCommand->CommandType = TypeCommand::None;
			SetCommand(Scene->ObjectCurrent, TypeCommand::None);
			//##FIX.3
			IndexObjectSelected = -1;
		}
	}
	
	if (command->TargetIndex == Scene->ObjectCurrent->Index) {
		//--- position selected
		if (Scene->ReadCommand(ObjectCreated))
		{
			//SelectObjectOffsetPos = vec3(0);
			IndexObjectSelected = command->TargetIndex;
			Scene->ObjectCurrent->ActionObjectCurrent = ActionObject::Moving;
		}
	}
}

//===================== Event Edit controls ===========================

void RoomUseInterface::ModeEditControls(shared_ptr<ObjectGUI> objGUI)
{
	
	CommandPack* command = &Scene->CurrentSceneCommand;
	if (!command->Enable)
		return;

	if (Scene->ReadCommand(EditGUI_OnOff))
	{
		if (command->Options.size() != 0)
			IsEditControls = command->Options["ButtonEditOn"];
	}
}

//===================== Check state controls =========================== TODO: aspect ??

void RoomUseInterface::CheckStateObjects() {


	if (Scene->ObjectCurrent->Name == "Button_EditBox")
	{
		auto t = Scene->ObjectCurrent->IsVisible;
		//Scene->ObjectCurrent->IsVisible = false;
	}

	if (Scene->ReadCommand(CheckStateObjectCommand))
	{
		IsUpdatingStateObjects = true;
	}
	if (!IsUpdatingStateObjects)
		return;

	if (IsUpdatingStateObjects) {
		
		Scene->ObjectCurrent->Refresh();

	}

	if (Scene->IsLastCurrentObject) {
		
		IsUpdatingStateObjects = false;
	}
}

//===================== Event Edit Name controls ===========================

void RoomUseInterface::EventEditTextControl(shared_ptr<ObjectGUI> objGUI) {

	//---------- Complete edit
	if (objGUI->TypeObj == EditBox && objGUI->ActionObjectCurrent == ActionObject::Woking) {
		if (Scene->Storage->Inputs->Key == m_endEditKey &&
			Scene->Storage->Inputs->Action == GLFW_PRESS) {
			objGUI->ActionObjectCurrent == ActionObject::Stay;
			objGUI->DefaultView();
			objGUI->Refresh();
			int ownerButton =  objGUI->IndexObjectOwner;
			Scene->AddCommand(StopWorking, objGUI->IndexObjectOwner, objGUI->Index);
		}
	}

	//----------- Start edit
	CommandPack* command = &Scene->CurrentSceneCommand;
	if (!command->Enable)
		return;
	if (command->TargetIndex != objGUI->Index)
		return;

	if (Scene->ReadCommand(StopWorking))
	{
		//Stop button frame 
		objGUI->Click();
		IndexObjectSelected = -1;
		if (objGUI->Name == name_EditBoxControl)
		{
			objGUI->IsVisible = false;
			Scene->AddCommand(CheckStateObjectCommand);
		}
	}

	//if (Scene->ReadCommand(EditObjectCommand))
	if (command->CommandType ==  EditObjectCommand)
	{
		string key = command->Description;
		int isChecked =  command->Options[key];
		
		Scene->ReadCommand(EditObjectCommand);

		if (isChecked)
		{
			//Start edit box
			IndexObjectSelected = objGUI->Index;
			objGUI->Click();
		}
		else {
			//Cancel edit box - unClick
			objGUI->ActionObjectCurrent = ActionObject::Stay;
			objGUI->DefaultView();
			objGUI->Refresh();
			IndexObjectSelected = -1;
		}
	}
}

//==========================================
//obj->ParamCase = m_startResizeParamShaderID;
//float m_startDefaultParamShaderID = 0;
//float m_startFocusParamShaderID = 1;
//float m_startMoveParamShaderID = 2;
//float m_startResizeParamShaderID = 3;
//float m_startClickParamShaderID = 4;
//m_startCheckBorderParamShaderID
//====================================

void RoomUseInterface::Work() {
	
	if (Scene->Storage->SceneData->IsGUI == false)
		return;

	if (Scene->IsBreakUpdate())
		return;

	IsFocused = false;

	if (Scene->IsFirstCurrentObject) {
		//IndexObjectFocused = -1;
		IsCursorClickEvent = IsCursorClickEventConst = Scene->Storage->Inputs->MBT == m_KeyPush && Scene->Storage->Inputs->ActionMouse == GLFW_PRESS;
	}

	std::shared_ptr<ObjectGUI> objGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);
	if(objGUI == nullptr)
		return;

	ModeEditControls(objGUI);

	CheckStateObjects();

	IsBackgroundFrame = Scene->ObjectCurrent->IndexObjectOwner == -1;

	//--- Focused
	CalculateMousePosWorld();

	if (objGUI->TypeObj == TypeObject::CursorGUI) {
		CursorMovePos = objGUI->StartPos;
	}

	if (objGUI->IsUsable)
	{
		//--- Moving to Cusror position
		EventMovingControl(objGUI);

		//--- Create new control
		EventCreateObject(objGUI);
		
		//--- Resize control to Cusror position
		EventResizeControl(objGUI);

		//--- Focus box & focus Border
		CheckFocusBoxAndBorderControl(objGUI);

		//----Start event Click Control
		EventStartClickControl(objGUI);

		//-- End event resize control
		//bool isEndRsizeControl = 
		EventEndResizeControl(objGUI);
		//-- Start event resize colntrol
		EventStartResizeControl(objGUI);

		//-- End event on control
		//bool isEndMovingControl = 
		EventEndMovingControl(objGUI);
		//----Start event Moving Control
		EventStartMovingControl(objGUI);

		EventEditTextControl(objGUI);
	}

	//===== Event Focus Control
	EventFocusControl(objGUI);

	//------------- Total orders
	if (Scene->IsLastCurrentObject) {
		
	}
}


void RoomUseInterface::CalculateMousePosWorld() {

	auto mouseX = Scene->Storage->Oper->m_MouseX;
	auto mouseY = Scene->Storage->Oper->m_MouseY;
	if (m_tempMousePos != vec2(mouseX, mouseY))
	{
		m_tempMousePos = vec2(mouseX, mouseY);
		std::shared_ptr<ObjectGUI> objGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);
		auto mouseX = Scene->Storage->Oper->m_MouseX;
		auto mouseY = Scene->Storage->Oper->m_MouseY;
		auto winHeight = Scene->m_heightWindow;
		auto winHWidth = Scene->m_widthWindow;
		auto model = Scene->Storage->ConfigMVP->Model;

		//================================= Mouse pos calculate
		//float depthMouse = 0.978;	//  (for ObjectGUI.zOrder == 3.79)	
		//float depthMouse = 0.9845;//  (for ObjectGUI.zOrder == 4.79)
		//float depthMouse = 0.988;	//  (for ObjectGUI.zOrder == 5.79)

		float depthPixel;
		glReadPixels(mouseX, winHeight - mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthPixel);
		float magicNumberOne = 0.5722;
		float magicNumberTwo = 0.4282;
		float depthMouse = (magicNumberOne * depthPixel) + magicNumberTwo;

		vec4 viewportVec(0.0f, 0.0f, winHWidth, winHeight);
		mat4 projectionPerspective = perspective(45.0f, (float)(winHeight) / (float)(winHeight), 0.1f, 1000.0f);
		vec3 screenPos = vec3(mouseX, winHeight - mouseY, depthMouse);
		m_tempMousePosWorld = unProject(screenPos, model, m_projectionPerspective, viewportVec);
		Scene->Storage->Oper->PositionCursorWorld = m_tempMousePosWorld;
		//auto mouseX = Scene->Storage->Oper->m_MouseX;
		//auto mouseY = Scene->Storage->Oper->m_MouseY;

		if (m_isDebug) {

			float depthGUI = objGUI->PanelDepth;
			vec2 startPosRect = vec2(0);
			vec2 endPosRect = vec2(0);
			float zOrder;
			std::shared_ptr<ObjectGUI> objGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);
			objGUI->Shape->GetPositRect(startPosRect, endPosRect, zOrder);
			std::stringstream ss;
			ss << "---------------------------------\n"
				<< "Mouse depth: " << depthMouse << " \n"
				<< "Mouse: " << m_tempMousePosWorld.x << " ; " << m_tempMousePosWorld.y << " ; " << m_tempMousePosWorld.z << " \n\n"
				<< "Calc Depth: " << depthPixel << " \n\n"
				<< "gui depth: " << depthGUI << " \n"
				<< "gui start: " << startPosRect.x << " ; " << startPosRect.y << " \n"
				<< "gui end: " << endPosRect.x << " ; " << endPosRect.y << " \n"
				<< "gui zOrder: " << zOrder << " \n"
				<< std::endl;
			string strTemp = ss.str();

			if (m_stringDebug != strTemp) {
				m_stringDebug = strTemp;
				std::cout << m_stringDebug;
			}
		}
	}
}

