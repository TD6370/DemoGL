#include "GUIComponent.h"

#include "../ObjectsTypes/ObjectGUI.h"
#include "../ObjectsTypes/ObjectPhysic.h"
//#include "../ObjectsTypes/ObjectButton.h"

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


	m_obj->AnimationParam = new AnimationParamGUI();
	
	//------------- ??? 
	m_obj->Shape = new ShapeSquare();
	m_obj->Shape->UpdateShapeInfo(m_obj);

	//------------- ??? 
	//m_obj->IsPhisicComponent = true;
	//ObjectPhysic::InitData();

	m_obj->Size = vec3(0);

	if (m_obj->EngineData->SceneData->IndexBackgroundGUIObj == -1)//First create gui
		m_obj->EngineData->SceneData->IndexBackgroundGUIObj = m_obj->Index;

	m_obj->ActionObjectCurrent = Stay;
}


void GUIComponent::RunAction() {

	m_obj->GetShapeSquare()->FormUpdate();

	if (m_obj->ActionObjectCurrent != Lock)
	{
		switch (m_obj->ActionObjectCurrent)
		{
		case Woking:
			if (m_obj->IsTextBoxComponent)
			{
				m_obj->TextBox->ActionWork();
			}
			else
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

	if (m_obj->IsTextBoxComponent)
	{
		vector<ObjectFiledsSpecific> resultTextBox = m_obj->TextBox->GetSpecificFiels();
		result.insert(result.end(), resultTextBox.begin(), resultTextBox.end());
	}

	return result;
}

void GUIComponent::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (m_obj->IndexObjectOwner == -1) //Back GUI
		return;

	SceneSerialize* serializer = new SceneSerialize();

	m_obj->StartPos = serializer->StrToVec3(filedsSpecific[0].Value);
	m_obj->SizePanel = serializer->StrToVec2(filedsSpecific[1].Value);

	if (m_obj->IsTextBoxComponent)
	{
		m_obj->TextBox->SetSpecificFiels(filedsSpecific);
	}

	m_obj->GetShapeSquare()->SetSizeControl(vec3(m_obj->SizePanel.x, m_obj->SizePanel.y, 1));
}

//----------------------------------------------------------------

