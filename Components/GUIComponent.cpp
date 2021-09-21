#include "GUIComponent.h"

#include "../ObjectsTypes/ObjectPhysic.h"

#include "..\CreatorModelData.h"
#include "..\Serialize\SceneSerialize.h"
#include "..\ModelData.h"
#include "../CoreSettings.h"

#include "../GeomertyShapes/ShapeBase.h"
#include "../GeomertyShapes/ShapeSquare.h"
#include "../ShellObjects/BaseShell.h"
#include "../Components/RenderComponent.h"

#include "../Components/TextBoxComponent.h"

void GUIComponent::Init(ObjectData* obj) {

	m_obj = obj;

	//------------------------
	m_obj->IsGUI = true;
	m_obj->IsFocusable = true;
	m_obj->IsTransformable = true;
	m_obj->IsUsable = true;
	m_obj->IsAbsolutePosition = false;
	m_obj->IsSquareModel = true;
	m_obj->ActionObjectCurrent = ActionObject::Stay;
	m_obj->Layer = TypeLayer::LayerBase;

	//------------------------
	m_obj->AnimationParam = new AnimationParamGUI();
	m_obj->Size = vec3(0);
	m_obj->ActionObjectCurrent = Stay;
	//------------------------

	//-- System Background GUI
	if (m_obj->Name == m_obj->EngineData->SceneData->NameSystemBackgroundGUI)
		m_obj->EngineData->SceneData->IndexBackgroundGUIObj = m_obj->Index;

	//-- System Cursor GUI
	if (m_obj->TypeObj == TypeObject::CursorGUI) {
		m_obj->ActionObjectCurrent = Moving;
		m_obj->IsAbsolutePosition = true;
	}

	//-- System Edit Box GUI
	if (m_obj->Name == m_obj->EngineData->SceneData->NameSystemEditBox)
	{
		m_obj->EngineData->SceneData->IndexBaseEditBox = m_obj->Index;
	}
}


void GUIComponent::RunAction() {

	m_obj->GetShapeSquare()->FormUpdate();

	if (m_obj->ActionObjectCurrent != Lock)
	{
		switch (m_obj->ActionObjectCurrent)
		{
		case Woking:
			m_obj->ActionWork();
			break;
		case Moving:
			ActionMoving();
			break;
		default:
			break;
		}
	}

	m_obj->ActionBase();
}

bool GUIComponent::GetVisible() {

	return m_obj->EngineData->SceneData->IsGUI && m_obj->IsVisible;
}

void GUIComponent::ActionWork() {

	m_obj->MaterialData.Color = m_color_work;
	m_obj->ActionObjectCurrent = Stay; //Off
}

void GUIComponent::ActionMoving()
{
	if (m_obj->ActionObjectCurrent != Moving)
		return;

	if (m_obj->TypeObj == TypeObject::CursorGUI) {
		SaveNewPosition();
		return;
	}

	m_obj->MaterialData.Color = m_color_transforming;
}

float GUIComponent::GetTopLayer() {

	if (m_obj->ShellIndex != -1)
	{
		if (m_obj->Shell != nullptr && m_obj->Shell->RootObjIndex != -1 && m_obj->Shell->RootObj != nullptr) {
			return m_obj->Shell->RootObj->StartPos.y;
		}
	}
	return m_obj->StartPos.y;
}


float GUIComponent::GetZ() {

	return m_obj->StartPos.z;
}

void GUIComponent::SetZ(float z)
{
	if (m_obj->IndexObjectOwner == -1)
		return;
	m_obj->StartPos.z = z;
}

string GUIComponent::GetInfo() {

	//std::cout << "<<GET info>> " << Name << "\n";
	string info = m_obj->GetInfo();

	std::stringstream ss;
	//ss << info << "	TOP: " << StartPos.y;
	//ss << info << "\nZ: " << StartPos.z << "  T: " << StartPos.y;
	ss << info << "\n							Z: " << m_obj->StartPos.z << "  T: " << GetTopLayer();
	return ss.str();
}


//----------------------------------------------------------------

vector<ObjectFiledsSpecific> GUIComponent::GetSpecificFiels() {

	SceneSerialize* serializer = new SceneSerialize();

	vector<ObjectFiledsSpecific> result{
		{"StartPos:", serializer->Vec3Str(m_obj->StartPos)} ,
		{"SizePanel:", serializer->Vec2Str(m_obj->SizePanel)}
	};
	return result;
}

void GUIComponent::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (m_obj->IndexObjectOwner == -1) //Back GUI
		return;

	SceneSerialize* serializer = new SceneSerialize();

	m_obj->StartPos = serializer->StrToVec3(filedsSpecific[0].Value);
	m_obj->SizePanel = serializer->StrToVec2(filedsSpecific[1].Value);

	m_obj->GetShapeSquare()->SetSizeControl(vec3(m_obj->SizePanel.x, m_obj->SizePanel.y, 1));
}

//----------------------------------------------------------------


void GUIComponent::SaveNewPosition() {

	//--- Set position Cursor
	vec3 mouse = m_obj->EngineData->Oper->PositionCursorWorld;
	vec2 startPosRect = m_obj->Shape->GetStartPositWorld();

	bool notX = false;
	bool notY = false;
	float k = 0.2;

	//---- TEST -- ?? fixed frozen cursor
	//if (mouse.z > 0 || startPosRect.x != startPosRect.x)
	if (mouse.z > 0)
	{
		return;
	}

	//=========================================
	float stepFactorX = mouse.x / startPosRect.x;
	float stepFactorY = mouse.y / startPosRect.y;

	float stepX = mouse.x - startPosRect.x;
	float stepY = startPosRect.y - mouse.y;
	stepX *= k;
	stepY *= k;

	float limit = 0.001;
	//---- TEST -- ?? fixed frozen cursor
	if (stepX <limit && stepX > -limit && stepY < limit && stepY > -limit) {
		return;
	}

	vec2 sizeBack = m_obj->EngineData->SceneData->SizeBackgroungGUI;
	float offSet = (sizeBack.x - 2) / 2;
	float limitBordMin = 0.01 - offSet;
	float limitBordMax = 1.98 + offSet;

	//---- TODO
	float leftBackFactor = (sizeBack.x - BACKGROUND_GUI_WIDTH_F);
	limitBordMax += leftBackFactor;

	float corrBord = 0.02;
	if (m_obj->StartPos.x + stepX < limitBordMin) {
		m_obj->StartPos.x = limitBordMin + corrBord;
		notX = true;
	}
	if (m_obj->StartPos.x + stepX > limitBordMax) {
		m_obj->StartPos.x = limitBordMax - corrBord;
		notX = true;
	}
	if (m_obj->StartPos.y + stepY < limitBordMin) {
		m_obj->StartPos.y = limitBordMin + corrBord;
		notY = true;
	}
	if (m_obj->StartPos.y + stepY > limitBordMax) {
		m_obj->StartPos.y = limitBordMax - corrBord;
		notY = true;
	}

	if (!notX)
		m_obj->StartPos.x += stepX;
	if (!notY)
		m_obj->StartPos.y += stepY;

	m_obj->Reset();
}