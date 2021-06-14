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

void RoomUseInterface::Work() {

	if (Scene->Storage->SceneData->IsGUI == false)
		return;

	if (Scene->IsBreakUpdate())
		return;

	if (Scene->IsFirstCurrentObject) {
		IndexObjectFocused = -1;
	}

	std::shared_ptr<ObjectGUI> objGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);
	if(objGUI == nullptr)
		return;
	if (Scene->ObjectCurrent->IndexObjectOwner == -1)
		return;

	bool isFocusable = objGUI->IsFocusable;
	bool isTransformable = objGUI->IsTransformable;
	bool isUsable = objGUI->IsUsable;
	bool isFocused = false;
	bool isCursorClickEvent = Scene->Storage->Inputs->MBT == m_KeyPush && Scene->Storage->Inputs->ActionMouse == GLFW_PRESS;
	vec2 endPosRect, startPosRect;
	float zOrder;

	if (objGUI->TypeObj == TypeObject::CursorGUI) {
		CursorMovePos = objGUI->StartPos;
	}
	
	if (isUsable)
	{
		if (isFocusable && isTransformable && objGUI->ActionObjectCurrent == ActionObject::Moving && IndexObjectSelected == objGUI->Index)
		{
			Scene->Debug("GUI moving");
			vec3 newPos = CursorMovePos - SelectObjectOffsetPos;
			objGUI->StartPos = vec3(newPos.x, newPos.y, objGUI->StartPos.z);
		}

		vec3 posCursor = GetMousePosWorld();
		objGUI->GetPositRect(startPosRect, endPosRect, zOrder);
		if (CheckPointInRectangle(posCursor, startPosRect, endPosRect))
		{
			//std::cout << "Select: " << objGUI->Name << "\n";
			IndexObjectFocused = objGUI->Index;
			isFocused = true;
		}

		if (isFocusable && isCursorClickEvent) {

			if (objGUI->ActionObjectCurrent == ActionObject::Moving && IndexObjectSelected == objGUI->Index)
			{
				Scene->Debug("Complete moving");
				IndexObjectSelected == -1;
				objGUI->ActionObjectCurrent = ActionObject::Stay;
			}
			else if (isFocused) 
			{
				Scene->Debug("Start moving");
				objGUI->Click();
				objGUI->ActionObjectCurrent = ActionObject::Moving;
				IndexObjectSelected = objGUI->Index;
				SelectObjectOffsetPos = CursorMovePos - objGUI->StartPos;
			}
		}
	}

	if (!isFocusable)
		return;

	if (isFocusable && objGUI->ActionObjectCurrent == Stay) {
		if (isFocused) {
			objGUI->Color = color_selected;
		}
		else {
			objGUI->Color = color_default;
		}
	}
}

vec3 RoomUseInterface::GetMousePosWorld() {

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
	return m_tempMousePosWorld;
}

