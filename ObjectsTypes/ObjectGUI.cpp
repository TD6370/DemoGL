#include "ObjectGUI.h"
#include "ObjectPhysic.h"
#include "ObjectTextBox.h"
#include "ObjectButton.h"

#include "..\CreatorModelData.h"
#include "..\Serialize\SceneSerialize.h"
#include "..\ModelData.h"
#include "../CoreSettings.h"

#include "../GeomertyShapes/ShapeBase.h"
#include "../GeomertyShapes/ShapeSquare.h"
#include "../ShellObjects/BaseShell.h"
#include "../Components/RenderComponent.h"

void ObjectGUI::InitData() {

	AnimationParam = new AnimationParamGUI();
	Shape = new ShapeSquare();
	Shape->UpdateShapeInfo(this);
	
	ObjectPhysic::InitData();

	Size = vec3(0);

	if (EngineData->SceneData->IndexBackgroundGUIObj == -1)//First create gui
		EngineData->SceneData->IndexBackgroundGUIObj = Index;

	ActionObjectCurrent = Stay;
}

void ObjectGUI::RunAction() {

	GetShapeSquare()->FormUpdate();

	if (ActionObjectCurrent != Lock)
	{
		switch (ActionObjectCurrent)
		{
			case Woking:
				ActionWork();
				break;
			case Moving:
				ActionMoving();
				break;
			case Transforming:
				ActionTransforming();
				break;
			default:
				break;
		}
	}

	ObjectData::ActionBase();
}

void ObjectGUI::ActionMoving()
{
	if (ActionObjectCurrent != Moving)
		return;

	MaterialData.Color = m_color_transforming;
}


bool ObjectGUI::GetVisible() {

	return EngineData->SceneData->IsGUI && IsVisible;
}


void ObjectGUI::SizeControlUpdate()
{
	GetShapeSquare()->SetSizeControl(vec3(SizePanel.x, SizePanel.y, 1));
}

//----------------------------------------------------------------

vector<ObjectFiledsSpecific> ObjectGUI::GetSpecificFiels() {

	SceneSerialize* serializer = new SceneSerialize();

	vector<ObjectFiledsSpecific> result { 
		{"StartPos:", serializer->Vec3Str(StartPos)} ,
		{"SizePanel:", serializer->Vec2Str(SizePanel)}
	};

	return result;
}

void ObjectGUI::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (IndexObjectOwner == -1) //Back GUI
		return;

	SceneSerialize* serializer = new SceneSerialize();

	StartPos = serializer->StrToVec3(filedsSpecific[0].Value);
	SizePanel = serializer->StrToVec2(filedsSpecific[1].Value);
	
	GetShapeSquare()->SetSizeControl(vec3(SizePanel.x, SizePanel.y, 1));
}

//----------------------------------------------------------------

void ObjectGUI::Click() {
	ActionObjectCurrent = Woking;
}

void ObjectGUI::ActionWork() {

	MaterialData.Color = m_color_work;
	ActionObjectCurrent = Stay; //Off
}


void ObjectGUI::DefaultView() {

	ObjectData::DefaultView();

	ParamValue = 0;

}

float ObjectGUI::GetTopLayer() {
	
	if (ShellIndex != -1)
	{
		if (Shell != nullptr && Shell->RootObjIndex != -1 && Shell->RootObj != nullptr) {
			shared_ptr<ObjectGUI> objGUI = std::dynamic_pointer_cast<ObjectGUI>(Shell->RootObj);
			return objGUI->StartPos.y;
		}
	}
	return StartPos.y;
}

float ObjectGUI::GetZ() {

	return StartPos.z;
}

void ObjectGUI::SetZ(float z)
{
	if (IndexObjectOwner == -1)
		return;
	StartPos.z = z;
}

string ObjectGUI::GetInfo() {

	//std::cout << "<<GET info>> " << Name << "\n";
	string info = ObjectData::GetInfo();

	std::stringstream ss;
	//ss << info << "	TOP: " << StartPos.y;
	//ss << info << "\nZ: " << StartPos.z << "  T: " << StartPos.y;
	ss << info << "\n							Z: " << StartPos.z << "  T: " << GetTopLayer();
	return ss.str();
}
