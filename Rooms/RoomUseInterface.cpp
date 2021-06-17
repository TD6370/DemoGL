﻿#include "RoomUseInterface.h"

//#include "..\GeometryLib.h"
#include "..\SceneConstruction.h"
#include "..\ObjectsTypes\ObjectData.h"
#include "..\ObjectsTypes\ObjectGUI.h"
#include "..\CreatorModelData.h"
#include "..\ModelData.h"

RoomUseInterface::~RoomUseInterface() {
}

void RoomUseInterface::Config() {

}

void RoomUseInterface::Init() {
	
	//color_selected = color_yelow;
	color_selected = color_yelow;
	auto winHeight = Scene->m_heightWindow;
	auto winHWidth = Scene->m_widthWindow;
	m_projectionPerspective = glm::perspective(45.0f, (float)(winHeight) / (float)(winHeight), 0.1f, 1000.0f);
}

//==================================== START MOVE
void  RoomUseInterface::EventStartMovingControl(std::shared_ptr<ObjectGUI> obj) {
	if (!obj->IsFocusable)
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
	//obj->Click();
	obj->ActionObjectCurrent = ActionObject::Moving;
	IndexObjectSelected = obj->Index;
	SelectObjectOffsetPos = CursorMovePos - obj->StartPos;

	IsCursorClickEvent = false;//!!!
}

#pragma region MOVE

//----------------------- MOVE
void  RoomUseInterface::EventMovingControl(std::shared_ptr<ObjectGUI> obj) {

	if (IndexObjectSelected != obj->Index)
		return;
	if (!obj->IsFocusable)
		return;
	if(!obj->IsTransformable)
		return;
	if (obj->ActionObjectCurrent != ActionObject::Moving)
		return;

	Scene->Debug("GUI moving");
	vec3 newPos = CursorMovePos - SelectObjectOffsetPos;
	obj->StartPos = vec3(newPos.x, newPos.y, obj->StartPos.z);
}

//----------------------- END MOVE
bool RoomUseInterface::EventEndMovingControl(std::shared_ptr<ObjectGUI> obj) {
	if (!obj->IsFocusable)
		return false;
	if (!IsCursorClickEvent)
		return false;

	if (IndexObjectSelected != obj->Index)	//-- when click after order
		return false;

	if(obj->ActionObjectCurrent != ActionObject::Moving)
		return false;

	Scene->Debug("End moving");
	IndexObjectSelected = -1;	//???
	obj->ActionObjectCurrent = ActionObject::Stay;
	
	//TODO:
	IsCursorClickEvent = false; //---- VVVV
	return true;
}

#pragma endregion

//==================================== START RESIZE
void RoomUseInterface::EventStartResizeControl(shared_ptr<ObjectGUI> obj) {
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

	Scene->Debug("Start resize");
	//obj->Click();
	obj->ActionObjectCurrent = ActionObject::Transforming;
	obj->Color = color_resize;
	IndexObjectSelected = obj->Index;
	m_startSizePanel = obj->SizePanel;
	IsCursorClickEvent = false;//!!!
}

//----------------------- RESIZE
void  RoomUseInterface::EventResizeControl(shared_ptr<ObjectGUI> obj) {
	if (IndexObjectSelected != obj->Index)
		return;
	if (!obj->IsFocusable)
		return;
	if (!obj->IsTransformable)
		return;
	if (obj->ActionObjectCurrent != ActionObject::Transforming)
		return;

	Scene->Debug("GUI resize");

	
	float sizePanelX = CursorMovePos.x - obj->StartPos.x;
	float sizePanelY = CursorMovePos.y - obj->StartPos.y;
	/*vec3 stepMouse = CursorMovePos - SelectObjectOffsetPos;
	sizePanelX -= stepMouse.x;
	sizePanelY -= stepMouse.y;*/
	//sizePanelX -= SelectObjectOffsetPos.x;
	//sizePanelY -= SelectObjectOffsetPos.y;
	float kBord = 0.15;
	sizePanelX += m_sizeBorder * kBord;
	sizePanelY += m_sizeBorder * kBord;
	//sizePanelX += glm::pow(m_sizeBorder, 2);
	//sizePanelY += glm::pow(m_sizeBorder, 2);

	if (sizePanelX < 0.01 || sizePanelY < 0.01)
		return;

	obj->SizePanel.x = sizePanelX;
	obj->SizePanel.y = sizePanelY;
	obj->SizeControlUpdate();
}

//----------------------- END RESIZE
bool RoomUseInterface::EventEndResizeControl(shared_ptr<ObjectGUI> obj) {
	if(!obj->IsFocusable)
		return false;
	if (!IsCursorClickEvent)
		return false;
	if (obj->ActionObjectCurrent != ActionObject::Transforming)
		return false;
	if (IndexObjectSelected != obj->Index)
		return false;

	Scene->Debug("End resize");
	IndexObjectSelected = -1;
	obj->ActionObjectCurrent = ActionObject::Stay;

	IsCursorClickEvent = false;
	return true;
}
//----------------------- FOCUS
void RoomUseInterface::CheckFocusBoxAndBorderControl(std::shared_ptr<ObjectGUI> objGUI) {

	bool isCheckOrder = true;
	vec2 endPosRect, startPosRect;
	float zOrder;
	IsFocused = false;

	objGUI->GetPositRect(startPosRect, endPosRect, zOrder);

	//--- Check Focused
	if (CheckPointInRectangle(m_tempMousePosWorld, startPosRect, endPosRect))
	{
		if (IndexObjectFocused != -1 && objGUI->Index < FocusedOrder)
			isCheckOrder = false;

		if (isCheckOrder) {
			FocusedOrder = objGUI->Index;
			IndexObjectFocused = objGUI->Index;
			IsFocused = true;
		}
	}
	if (isCheckOrder) {
		//--- Check Focused border
		IsCheckBorder = CheckPointInRectangleBorder(m_tempMousePosWorld, startPosRect, endPosRect, m_sizeBorder);
	}
}

void RoomUseInterface::EventFocusControl(std::shared_ptr<ObjectGUI> objGUI) {
	if (objGUI->ActionObjectCurrent != Stay)
		return;
	if (!objGUI->IsFocusable)
		return;

	if (IsFocused) {
		objGUI->Color = color_selected;
	}
	else {
		if (IndexObjectFocused == objGUI->Index) {
			FocusedOrder = -1;
			IndexObjectFocused = -1;
		}
		objGUI->Color = color_default;
	}
}

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
	if (Scene->ObjectCurrent->IndexObjectOwner == -1)
		return;

	//--- Focused
	CalculateMousePosWorld();

	//IsCursorClickEvent = IsCursorClickEventConst = Scene->Storage->Inputs->MBT == m_KeyPush && Scene->Storage->Inputs->ActionMouse == GLFW_PRESS;

	if (objGUI->TypeObj == TypeObject::CursorGUI) {
		CursorMovePos = objGUI->StartPos;
	}

	if (objGUI->IsUsable)
	{
		//--- Moving to Cusror position
		EventMovingControl(objGUI);

		//--- Resize control to Cusror position
		EventResizeControl(objGUI);

		//--- Focus box & focus Border
		CheckFocusBoxAndBorderControl(objGUI);

		//-- End event resize control
		//bool isEndRsizeControl = 
		EventEndResizeControl(objGUI);
		//////-- Start event resize colntrol
		//if (!isEndRsizeControl)
		EventStartResizeControl(objGUI);

		//-- End event on control
		//bool isEndMovingControl = 
		EventEndMovingControl(objGUI);
		//----Start event Moving Control
		//if (!isEndMovingControl)
		EventStartMovingControl(objGUI);
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
		//float depthMouse = 0.978;	//  (for ObjectGUI.zOrder == 3.79)	ObjectGUI::UpdateState()
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
			objGUI->GetPositRect(startPosRect, endPosRect, zOrder);
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
