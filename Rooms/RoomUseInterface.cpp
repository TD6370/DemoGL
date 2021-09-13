#include "RoomUseInterface.h"

//#include "..\GeometryLib.h"
#include "..\CoreSettings.h"
#include "..\SceneConstructor.h"
#include "..\ObjectsTypes\ObjectData.h"
#include "..\ObjectsTypes\ObjectGUI.h"
//#include "..\ObjectsTypes\ObjectEditBox.h"
//#include "..\ObjectsTypes\ObjectButton.h"

#include "..\CreatorModelData.h"
#include "..\ModelData.h"
#include "../GeomertyShapes//ShapeBase.h"
#include "AspectDispatcherCommands.h"
#include "../ShellObjects/BaseShell.h"

RoomUseInterface::~RoomUseInterface() {
}

void RoomUseInterface::Config() {
}

void RoomUseInterface::Init() {
	
	AnimationParams = new AnimationParamGUI();
	color_selected = m_palette.color_yelow;
	auto winHeight = Scene->m_heightWindow;
	auto winHWidth = Scene->m_widthWindow;
	m_projectionPerspective = glm::perspective(45.0f, (float)(winHeight) / (float)(winHeight), 0.1f, 1000.0f);
	State.IsEditControls = false;

	AspectTransformControls.Init(this);
	AspectCreateControls.Init(this);
	AspectUseControls.Init(this);
}

void RoomUseInterface::SetCurrentEventParam(shared_ptr<ObjectData> obj, int value)
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

void RoomUseInterface::SetTimeAnimate(shared_ptr<ObjectData> obj, float timeAnim)
{
	obj->SetStartTimer();
	StartTimer = glfwGetTime();
	EndTimer = StartTimer + (timeAnim * 1);
}

//----------------------- FOCUS
void RoomUseInterface::CheckFocusBoxAndBorderControl() {

	auto obj = Scene->ObjectCurrent;

	if (State.IsBackgroundFrame && obj->SceneCommand->CommandType == TypeCommand::None)
		return;

	if (obj->ActionObjectCurrent == Moving ||
		obj->ActionObjectCurrent == Transforming)
		return;

	bool isCheckOrder = true;
	vec2 endPosRect, startPosRect;
	float zOrder;
	State.IsFocused = false;
	State.IsCheckBorder = false;

	obj->Shape->GetPositRect(startPosRect, endPosRect, zOrder);

	//--- Check Focused
	if (CheckPointInRectangle(m_tempMousePosWorld, startPosRect, endPosRect))
	{
		if (IndexObjectFocused != -1 && Scene->CurrentIndexOrder < FocusedOrder)
			isCheckOrder = false;

		if (isCheckOrder) {
			FocusedOrder = Scene->CurrentIndexOrder;
			IndexObjectFocused = obj->Index;
			State.IsFocused = true;
		}
	}

	if (!obj->IsTransformable)
		return;

	if (isCheckOrder) {
		//--- Check Focused border
		State.IsCheckBorder = CheckPointInRectangleBorder(m_tempMousePosWorld, startPosRect, endPosRect, m_sizeBorder);
	}
}

void RoomUseInterface::EventFocusControl() {

	auto obj = Scene->ObjectCurrent;

	if (obj->ActionObjectCurrent != Stay)
		return;

	bool isOrderParam = IsCompareF(m_CurrentStartedEventID, AnimationParams->StartResizeParamShaderID);
	bool isUpdateView = obj->IsFocusable || State.IsEditControls;
	if (State.IsBackgroundFrame)
		isUpdateView = false;

	if (State.IsFocused && isUpdateView) {
		obj->MaterialData.Color = color_selected;
		if (!isOrderParam)
		{ 
			if (State.IsCheckBorder && State.IsEditControls)
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

//===================== Check state controls =========================== 

void RoomUseInterface::CheckStateObjects() {

	if (Scene->ReadCommand(CheckStateObjectCommand))
	{
		State.IsUpdatingStateObjects = true;
	}
	if (!State.IsUpdatingStateObjects)
		return;

	if (State.IsUpdatingStateObjects) {
		Scene->ObjectCurrent->Refresh();
	}

	if (Scene->IsLastCurrentObject) {

		State.IsUpdatingStateObjects = false;
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

	if (Scene->IsBreakUpdate())
		return;

	//IsBackgroundFrame = Scene->ObjectCurrent->IndexObjectOwner == -1;
	State.IsBackgroundFrame = Scene->ObjectCurrent->Index == Scene->Storage->SceneData->IndexBackgroundGUIObj;

	State.IsFocused = false;

	if (Scene->IsFirstCurrentObject) {
		State.IsCursorClickEvent = Scene->Storage->Inputs->MBT == m_KeyPush &&
			Scene->Storage->Inputs->ActionMouse == GLFW_PRESS;
	}

	//--- select info object
	AspectCreateControls.EventSelectedInfoCreateObject();
	//--- Create new control
	AspectCreateControls.EventStartCreateObject();

	if (Scene->Storage->SceneData->IsGUI == false)
		return;

	AspectCreateControls.EventFillFieldsEdit();

	auto objGUI = Scene->ObjectCurrent;
	if (!objGUI->IsGUI)
		return;

	AspectUseControls.ModeEditControls();

	CheckStateObjects();
	
	//--- Focused
	CalculateMousePosWorld();

	AspectTransformControls.Config();

	AspectCreateControls.EventEndCreateObject();

	AspectUseControls.EventStartRenameObject();
	
	if (objGUI->IsUsable)
	{
		//--- Focus box & focus Border
		CheckFocusBoxAndBorderControl();

		//----Start event Click Control
		AspectUseControls.EventStartClickControl();

		//--- Moving to Cusror position
		//--- Resize control to Cusror position
		//-- End event resize control
		//-- Start event resize colntrol
		//-- End event on control
		//----Start event Moving Control
		AspectTransformControls.Work();

		AspectUseControls.EventEditTextControl();
	}

	AspectUseControls.EventReadKeyInput();

	//===== Event Focus Control
	EventFocusControl();

	//------------- Total orders
	/*if (Scene->IsLastCurrentObject) {
		
	}*/
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

