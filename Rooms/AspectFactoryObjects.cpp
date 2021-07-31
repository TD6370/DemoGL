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
	
	m_startContructing = false;
	CreateInfo info;

	int value = command.Options[Scene->CommandsAttribute.TypeObjectAttr];
	TypeObject typeObj = (TypeObject)value;

	if (typeObj == TypeObject::TextBox)
	{
		if (!isBackgroundFrame)
			return;
		Scene->ReadCommand(CreateObject);

		info.Message = string();
		info.Pos = vec3(1.);
		info.Size = vec2(0.7, 0.1);

		if (command.Description.size() != 0)
			info.Message = command.Description;
		if(info.Message.size() == 0 && command.ValueS.size() != 0)
			info.Message = command.ValueS;
		if (info.Message.size() != 0)
			info.Init = true;

		CreateTextBox(info);
		isCompleted = true;
	}
	if (typeObj == TypeObject::Button)
	{
		if (!isBackgroundFrame)
			return;

		Scene->ReadCommand(CreateObject);
		CreateButton(info);
		isCompleted = true;
	}
	if (typeObj == TypeObject::EditBox)
	{
		if (!isBackgroundFrame)
			return;

		Scene->ReadCommand(CreateObject);
		CreateEditBox(info);
		isCompleted = true;
	}
	if (typeObj == TypeObject::ListBox ||
		typeObj == TypeObject::ListTextBox ||
		typeObj == TypeObject::ListEditBox
		)
	{
		if (!isBackgroundFrame)
			return;

		Scene->ReadCommand(CreateObject);
		CreateListBox(command.ValueS, typeObj);
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

void AspectFactoryObjects::CreateEditBox(CreateInfo info) {
	string caption = "?";
	string childModel;
	string objName;
	shared_ptr<ObjectButton> objCreateButton;
	shared_ptr<ObjectEditBox> objCreateEditBox;
	shared_ptr<ObjectData> objCreate;
	vec3 pos = vec3(1.);
	vec2 size = vec2(0.7, 0.06);
	if (info.Init) {
		pos = info.Pos;
		size = info.Size;
		caption = info.Message;
	}
	pos.z = Scene->Storage->StartPosGUI_Z;
		
	// ---- Object Button create obj GUI
	objName = "FrameEditBox";
	childModel = "ButtonEditBoxModel";
	shared_ptr<ObjectGUI> objBackGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);
	
	objCreate = Scene->Storage->AddChildObject(objBackGUI, caption, childModel, objName, pos, size, Button, vec3(1));
	objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);
	objCreateButton->IsToogleButon = true;

		// ---- Object Edit box create	
		auto objCreateEditBox_Data = Scene->Storage->ControlConstruct(objCreateButton, caption, EditBox);

	if(!m_startContructing)
		Scene->AddCommand(ObjectCreated, -1, objCreateButton->Index);

	if (m_startContructing)
		m_lastObjectCreated = objBackGUI;
}

void AspectFactoryObjects::CreateTextBox(CreateInfo info) {

	// ---- Object text box GUI
	string objName = "TextBoxObject";
	string childModel = "TextBoxModel";
	vec3 color = vec3(0.117, 0.351, 0.950);
	vec3 pos = vec3(1.);
	vec2 size = vec2(0.7, 0.1);
	string caption = "???";
	if (info.Init) {
		pos = info.Pos;
		size = info.Size;
		caption = info.Message;
	}
	pos.z = Scene->Storage->StartPosGUI_Z;

	shared_ptr<ObjectGUI> objBackGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);
	shared_ptr<ObjectData> objCreate = Scene->Storage->AddChildObject(objBackGUI, caption, childModel, objName, pos, size, TextBox, color);

	if (!m_startContructing)
		Scene->AddCommand(ObjectCreated, -1, objCreate->Index);

	if (m_startContructing)
		m_lastObjectCreated = objCreate;
}

void AspectFactoryObjects::CreateButton(CreateInfo info) {

	string caption = "??";
	string childModel;
	string objName;
	vec3 color = vec3(0.3);
	shared_ptr<ObjectButton> objCreateButton;
	shared_ptr<ObjectData> objCreate;
	vec3 pos = vec3(1.);
	vec2 size = vec2(0.7, 0.1);
	if (info.Init) {
		pos = info.Pos;
		size = info.Size;
		caption = info.Message;
	}
	pos.z = Scene->Storage->StartPosGUI_Z;

	// ---- Object Button create obj GUI
	objName = "ButtonCreateObjGUI";
	childModel = "ButtonModel";
	shared_ptr<ObjectGUI> objBackGUI = std::dynamic_pointer_cast<ObjectGUI>(Scene->ObjectCurrent);
	
		objCreate = Scene->Storage->AddChildObject(objBackGUI, caption, childModel, objName, pos, size, Button, vec3(1));
		objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);
		objCreateButton->IsToogleButon = false;
		objCreateButton->SceneCommand->CommandType = TypeCommand::None;
	
		auto objCreateTextBox_Data = Scene->Storage->ControlConstruct(objCreateButton, caption, Button);
	
	if (!m_startContructing)
		Scene->AddCommand(ObjectCreated, - 1, objCreateButton->Index);

	if (m_startContructing)
		m_lastObjectCreated = objBackGUI;
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
	//shared_ptr<ObjectButton> objCreateButton_Prev = nullptr;
	shared_ptr<ObjectData> objCreateItem_Prev = nullptr;
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
	float heightItem = 0.06;
	float widthFrame = 0.6;
	float heightFrame = (listCommand.size() * heightItem) + ((listCommand.size() -1) * interligne) + (border*2);
	vec2 sizeFrame = vec2(widthFrame, heightFrame);
	vec3 posFrame = vec3(pos.x, pos.y, posZ);

	objBaseFrame = Scene->Storage->AddChildObject(objBackGUI, caption, childModel, objName, posFrame, sizeFrame, ListBox, vec3(1));
	objBaseFrame->IsFocusable = false;
	
	CreateInfo infoItem;
	infoItem.Init = true;
	infoItem.Pos = vec3(posFrame.x + border, posFrame.y + border, posZ);
	infoItem.Size = vec2(sizeFrame.x - (border * 2), heightItem);

	for (CommandPack commItem : listCommand)
	{
		infoItem.Message = commItem.Description;

		/*if (typeListBox == ListTextBox){
			m_startContructing = true;
			CreateTextBox(infoItem);
			objCreate = m_lastObjectCreated;
		}
		if (typeListBox == ListEditBox){
			m_startContructing = true;
			CreateEditBox(infoItem);
			objCreate = m_lastObjectCreated;
		}*/
		if (typeListBox == ListTextBox)
		{
			bool isBorder = true;
			if (isBorder) {
				objName = "FrameItem_ListTextBox";
				childModel = "FrameModel";
				objCreate = Scene->Storage->AddChildObject(objBaseFrame, infoItem.Message, childModel, objName, infoItem.Pos, infoItem.Size, GUI, vec3(1));
				objCreate->IsFocusable = false;
			}
			//-- create Item "TextBox"
			objName = "TextBoxItem_ListTextBox";
			childModel = "TextBoxModel";
			color = vec3(0.117, 0.351, 0.950);
			if (isBorder) 
				Scene->Storage->ControlConstruct(objCreate, infoItem.Message, TextBox);
			else
				Scene->Storage->ControlConstruct(objBaseFrame, infoItem.Message, TextBox);
		}
		if (typeListBox == ListEditBox)
		{
			// ---- Object Edit box create	
			objName = "EditBoxItem_ListEditBox";
			childModel = "ButtonEditBoxModel";
			objCreate = Scene->Storage->AddChildObject(objBaseFrame, infoItem.Message, childModel, objName, infoItem.Pos, infoItem.Size, Button, vec3(1));
			objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);
			objCreateButton->IsToogleButon = true;
			auto objCreateEditBox_Data = Scene->Storage->ControlConstruct(objCreateButton, infoItem.Message, EditBox);

			//Command - create type Obj 
			objCreateEditBox_Data->SceneCommand->Description= commItem.Description;
			objCreateEditBox_Data->SceneCommand->ValueS = commItem.ValueS; 
			objCreateEditBox_Data->SceneCommand->ValueI = commItem.ValueI;
			//-- source object command
			objCreateEditBox_Data->SceneCommand->Options.insert({ Scene->CommandsAttribute.SourceCommandObjIndexAttr, objBaseFrame->Index });
		}
		if (typeListBox == ListBox)
		{
			//-- create Item "BUTTON"
			objName = "ButtonItem_ListBox";
			childModel = "ButtonModel";
			objCreate = Scene->Storage->AddChildObject(objBaseFrame, infoItem.Message, childModel, objName, infoItem.Pos, infoItem.Size, Button, vec3(1));
			objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);
			/*if(commItem.CommandType == SelectItemValue)
				objCreateButton->IsToogleButon = true;
			else*/
			objCreateButton->IsToogleButon = false;

			//-- create Text item 
			auto objCreateTextBox_Data = Scene->Storage->ControlConstruct(objCreateButton, infoItem.Message, Button);
			shared_ptr<ObjectTextBox> objCreateTextBox = std::dynamic_pointer_cast<ObjectTextBox>(objCreateTextBox_Data);

			SetCommand(objCreateButton, commItem);
		}
		//--- list index items for shell
		listItemsIndex.push_back(objCreate->Index);
		//---  join items links
		if (objCreateItem_Prev != nullptr) {
			objCreateItem_Prev->SetNextItemShellObject(objCreate);
		}
		objCreateItem_Prev = objCreate;

		infoItem.Pos.y += infoItem.Size.y + interligne;
	}

	//Create shell
	m_lastShellCreated = Scene->Storage->AddShell("ListBoxShell_" + nameListCommand,
		objBaseFrame->Index,
		-1,
		false,
		listItemsIndex);

	//- command complete
	if (!m_startContructing)
		Scene->AddCommand(ObjectCreated, -1, objBaseFrame->Index);

	//------------------------------
	if (m_startContructing)
		m_lastObjectCreated = objBackGUI;
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
