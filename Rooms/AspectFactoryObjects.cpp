#include "AspectFactoryObjects.h"
#include "AspectDispatcherCommands.h"
#include "../SceneConstructor.h"
#include "..\ObjectsTypes\ObjectData.h"

//#include "RoomMarkerPlane.h"
//#include "..\GeometryLib.h"
//#include "..\SceneConstructor.h"
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
#include "../SceneLayer.h"


void AspectFactoryObjects::Config() {

}

void AspectFactoryObjects::Init() {

}

void AspectFactoryObjects::Work() {

	if (Scene->IsBreakUpdate())
		return;

	bool isBackgroundFrame = Scene->IsCurrentObjectBackgroundFrameGUI();
	bool isCompleted = false;

	//--- Create object
	CommandPack command = Scene->CurrentSceneCommand;
	
	if (!command.Enable || command.CommandType != TypeCommand::CreateObject)
		return;
	
	int value = command.Options[Scene->CommandsAttribute.TypeObjectAttr];
	TypeObject typeObj = (TypeObject)value;
	if (typeObj == TypeObject::Button)
	{
		if (!isBackgroundFrame)
			return;

		Scene->ReadCommand(CreateObject);
		CreateButton();
		isCompleted = true;
	}
	if (typeObj == TypeObject::EditBox)
	{
		if (!isBackgroundFrame)
			return;

		Scene->ReadCommand(CreateObject);
		CreateEditBox();
		isCompleted = true;
	}
	if (typeObj == TypeObject::ListBox)
	{
		if (!isBackgroundFrame)
			return;

		Scene->ReadCommand(CreateObject);
		CreateListBox(command.ValueS);
		isCompleted = true;
	}
	if (typeObj == TypeObject::ObjectFieldsEdit)
	{
		if (!isBackgroundFrame)
			return;

		Scene->ReadCommand(CreateObject);
		CreateObjectFieldsEdit();
		isCompleted = true;
	}

	if (!isCompleted) {
		//Create object
	}

	

	if (isCompleted) {
		Scene->Storage->UpdateObjectsOrders();
	}
}

void AspectFactoryObjects::CreateEditBox() {
	string caption;
	string childModel;
	string objName;
	shared_ptr<ObjectButton> objCreateButton;
	shared_ptr<ObjectEditBox> objCreateEditBox;
	shared_ptr<ObjectData> objCreate;
	vec2 pos = vec2(1.);
	float posZ = Scene->Storage->StartPosGUI_Z;
		
	// ---- Object Button create obj GUI
	objName = "FrameEditBox";
	caption = "ах";
	childModel = "ButtonEditBoxModel";
	shared_ptr<ObjectGUI> objBackGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);

	objCreate = Scene->Storage->AddChildObject(objBackGUI, caption, childModel, objName, vec3(pos.x, pos.y, posZ), vec2(0.7, 0.1), Button, vec3(1));
	objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);
	objCreateButton->IsToogleButon = true;

		// ---- Object Edit box create	
		auto objCreateEditBox_Data = Scene->Storage->ControlConstruct(objCreateButton, caption, EditBox);

	Scene->AddCommand(ObjectCreated, -1, objCreateButton->Index);
}

void AspectFactoryObjects::CreateButton() {

	string caption;
	string childModel;
	string objName;
	vec3 color = vec3(0.3);
	shared_ptr<ObjectButton> objCreateButton;
	shared_ptr<ObjectData> objCreate;
	vec2 pos = vec2(1.);
	float posZ = Scene->Storage->StartPosGUI_Z;

	// ---- Object Button create obj GUI
	objName = "ButtonCreateObjGUI";
	caption = "эх";
	childModel = "ButtonModel";
	shared_ptr<ObjectGUI> objBackGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);

		objCreate = Scene->Storage->AddChildObject(objBackGUI, caption, childModel, objName, vec3(pos.x, pos.y, posZ), vec2(0.4, 0.2), Button, vec3(1));
		objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);
		objCreateButton->IsToogleButon = false;
		objCreateButton->SceneCommand->CommandType = TypeCommand::None;
	
		auto objCreateTextBox_Data = Scene->Storage->ControlConstruct(objCreateButton, caption, Button);
		
	Scene->AddCommand(ObjectCreated, - 1, objCreateButton->Index);
}


void AspectFactoryObjects::CreateListEditBox(string nameListCommand) {
	
	CreateListBox(nameListCommand, ListEditBox);
}

void AspectFactoryObjects::CreateListBox(string nameListCommand, TypeObject typeListBox) {
	string caption;
	string childModel;
	string objName;
	vec3 color = vec3(0.3);
	shared_ptr<ObjectButton> objCreateButton;
	shared_ptr<ObjectButton> objCreateButton_Prev = nullptr;
	shared_ptr<ObjectData> objBaseFrame;
	
	shared_ptr<ObjectData> objCreate;
	vec2 pos = vec2(1.);
	float posZ = Scene->Storage->StartPosGUI_Z;

	// ---- background GUI
	shared_ptr<ObjectGUI> objBackGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);

	vector<int> listItemsIndex = vector<int>();
	vector<CommandPack> listCommand = Scene->GetListCommand(nameListCommand);

	//-- Frame list box
	objName = "BaseFrame_ListBox";
	caption = "BaseFrame_ListBox";
	childModel = "FrameModel"; 
	// -- Style
	float border = 0.01;
	float interligne = 0.005;
	float heightItem = 0.07;
	float widthFrame = 0.6;
	float heightFrame = (listCommand.size() * heightItem) + ((listCommand.size() -1) * interligne) + (border*2);
	vec2 sizeFrame = vec2(widthFrame, heightFrame);
	vec3 posFrame = vec3(pos.x, pos.y, posZ);

	objBaseFrame = Scene->Storage->AddChildObject(objBackGUI, caption, childModel, objName, posFrame, sizeFrame, ListBox, vec3(1));
	objBaseFrame->IsFocusable = false;
	
	vec2 sizeItem = vec2(sizeFrame.x - (border * 2), heightItem);
	vec3 posItem = vec3(posFrame.x + border, posFrame.y + border, posZ);

	for (CommandPack commItem : listCommand)
	{
		if (typeListBox == ListEditBox)
		{
			//-- create Item "EditBox"

		}
		if (typeListBox == ListBox)
		{
			//-- create Item "BUTTON"
			objName = "ButtonItem_ListBox";
			caption = commItem.Description;
			childModel = "ButtonModel";
			objCreate = Scene->Storage->AddChildObject(objBaseFrame, caption, childModel, objName, posItem, sizeItem, Button, vec3(1));
			objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);

			/*if(commItem.CommandType == SelectItemValue)
				objCreateButton->IsToogleButon = true;
			else*/
			objCreateButton->IsToogleButon = false;

			//list index items for shell
			listItemsIndex.push_back(objCreateButton->Index);

			// - join items links
			if (objCreateButton_Prev != nullptr) {
				objCreateButton_Prev->SetNextItemShellObject(objCreateButton);
			}
			objCreateButton_Prev = objCreateButton;

			SetCommand(objCreateButton, commItem);

			//-- create Text item 
			auto objCreateTextBox_Data = Scene->Storage->ControlConstruct(objCreateButton, caption, Button);

			shared_ptr<ObjectTextBox> objCreateTextBox = std::dynamic_pointer_cast<ObjectTextBox>(objCreateTextBox_Data);
		}
		posItem.y += sizeItem.y + interligne;
	}

	//Create shell
	m_lastShellCreated = Scene->Storage->AddShell("ListBoxShell_" + nameListCommand,
		objBaseFrame->Index,
		-1,
		false,
		listItemsIndex);

	//- command complete
	Scene->AddCommand(ObjectCreated, -1, objBaseFrame->Index);
}


void AspectFactoryObjects::CreateObjectFieldsEdit() {

	//------------------------------------
	// Shell Object fields Edit
	//	List[0] = ListBox Text		[NAME]
	//	List[1] = ListBox EditBox	[VALUE]
	//	List[3] = Button Create
	//	List[4] = Button Save
	//	List[5] = Button Delete
	//
	// ControlFrame->Command->Target ==> selected Object
	//------------------------------------

	CreateListBox(Scene->CommandsAttribute.ObjectFieldsListCommand);

	CreateListEditBox(Scene->CommandsAttribute.ObjectFieldsListCommand);

	//m_lastShellCreated
}
