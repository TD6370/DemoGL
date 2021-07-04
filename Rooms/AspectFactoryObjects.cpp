#include "AspectFactoryObjects.h"
#include "AspectDispatcherCommands.h"
#include "../SceneConstruction.h"
#include "..\ObjectsTypes\ObjectData.h"

//#include "RoomMarkerPlane.h"
//#include "..\GeometryLib.h"
//#include "..\SceneConstruction.h"
//#include "..\ObjectsTypes\ObjectData.h"
#include "..\CreatorModelData.h"
#include "..\ModelData.h"

//#include "ObjectsTypes/ObjectData.h"
//#include "ObjectsTypes/ObjectPhysic.h"
//#include "ObjectsTypes/ObjectDynamic.h"
//#include "ObjectsTypes/ObjectPolygon.h"
//#include "ObjectsTypes/ObjectHero.h"
//#include "ObjectsTypes/ObjectNPC.h"
//#include "ObjectsTypes/ObjectBullet.h"
//#include "ObjectsTypes/ObjectCursorRay.h"
//#include "ObjectsTypes/ObjectBlock.h"
#include "../ObjectsTypes/ObjectGUI.h"
//#include "ObjectsTypes/ObjectTextBox.h"
//#include "ObjectsTypes/ObjectCursorGUI.h"
#include "../ObjectsTypes/ObjectButton.h"
#include "../ObjectsTypes/ObjectEditBox.h"
//#include "GeomertyShapes//ShapeBase.h" //###

void AspectFactoryObjects::Config() {

}

void AspectFactoryObjects::Init() {

}

void AspectFactoryObjects::Work() {

	if (Scene->IsBreakUpdate())
		return;
	
	//TEST
	//auto test = Scene->ObjectCurrent->Name;

	//bool isBackgroundFrame1 = Scene->ObjectCurrent->IndexObjectOwner == -1;
	bool isBackgroundFrame = Scene->IsCurrentObjectBackgroundFrameGUI();

	//--- Create object
	CommandPack command = Scene->CurrentSceneCommand;
	
	if (!command.Enable || command.CommandType != TypeCommand::CreateObject)
		return;
	
	int value = command.Options["TypeObject"];
	TypeObject typeObj = (TypeObject)value;
	if (typeObj == TypeObject::Button)
	{
		if (!isBackgroundFrame)
			return;

		Scene->ReadCommand(CreateObject);

		CreateButton();
	}
	if (typeObj == TypeObject::EditBox)
	{
		if (!isBackgroundFrame)
			return;

		Scene->ReadCommand(CreateObject);

		CreateEditBox();
	}
	
}

void AspectFactoryObjects::CreateEditBox() {
	string caption;
	string childModel;
	string objName;
	vec3 color = vec3(0.3);
	shared_ptr<ObjectEditBox> objCreateButton;
	shared_ptr<ObjectData> objCreate;
	vec2 pos = vec2(1.);

	// ---- Object Edit box create
	objName = "EditBoxCreateObjGUI";
	caption = "ввод";
	childModel = "EditBoxModel";
	shared_ptr<ObjectGUI> objBackGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);
	objCreate = objBackGUI->ConfigInterface(caption, childModel, objName, vec3(pos.x, pos.y, 0.02), vec2(0.4, 0.2), EditBox, vec3(1));
	objCreateButton = std::dynamic_pointer_cast<ObjectEditBox>(objCreate);
	//objCreateButton->IsToogleButon = false;
	//objCreateButton->SceneCommand->CommandType = TypeCommand::None;
	objBackGUI->ControlConstruct(objCreateButton, caption);

	Scene->AddCommand(ObjectCreated, objCreateButton->Index);
}

void AspectFactoryObjects::CreateButton() {

	string caption;
	string childModel;
	string objName;
	vec3 color = vec3(0.3);
	shared_ptr<ObjectButton> objCreateButton;
	shared_ptr<ObjectData> objCreate;
	vec2 pos = vec2(1.);

	// ---- Object Button create obj GUI
	objName = "ButtonCreateObjGUI";
	caption = "новая";
	childModel = "ButtonModel";
	shared_ptr<ObjectGUI> objBackGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);
		objCreate = objBackGUI->ConfigInterface(caption, childModel, objName, vec3(pos.x, pos.y, 0.02), vec2(0.4, 0.2), Button, vec3(1));
		objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);
		objCreateButton->IsToogleButon = false;
		objCreateButton->SceneCommand->CommandType = TypeCommand::None;
	objBackGUI->ControlConstruct(objCreateButton, caption);

	Scene->AddCommand(ObjectCreated, objCreateButton->Index);
}