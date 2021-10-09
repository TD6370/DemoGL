#include "AspectFactoryObjects.h"
#include "AspectDispatcherCommands.h"
#include "../SceneConstructor.h"
#include "..\ObjectsTypes\ObjectData.h"

#include "..\CreatorModelData.h"
#include "..\ModelData.h"

#include "../SceneLayer.h"

#include "../Components/ButtonComponent.h"
#include "../Components/GUIComponent.h"

void AspectFactoryObjects::Config() {

}

void AspectFactoryObjects::Init() {

}

void AspectFactoryObjects::LastWork() {

	if (m_poolNext < 1)
		return;

	CommandPack command;
	CreateInfo info;
	vec4 posC = command.ValueV4;
	int value;
	TypeObject typeObj;
	string objName;
	vec3 color = vec3(0.156, 0.784, 0.301);

	for (int i = 0; i < m_poolNext; i++) {
		command = poolObjectCreate[i];

		posC = command.ValueV4;
		if (posC.w != -1) {
			info.Pos = vec3(posC.x, posC.y, posC.z);
		}

		value = command.Options[Scene->CommandsAttribute.TypeObjectAttr];
		typeObj = (TypeObject)value;

		info.Message = string();

		if (command.Description.size() != 0)
			info.Message = command.Description;
		if (info.Message.size() == 0 && command.ValueS.size() != 0)
			info.Message = command.ValueS;

		//Create object
		objName = "Obj" + info.Message;
		shared_ptr<ObjectData> objCreate = Scene->Storage->AddObjectDefault(objName, typeObj, info.Pos, color);

		if (objCreate != nullptr)
			Scene->Storage->LayerScene->SaveOrderIndex(objCreate);

		//Clear
		ClearCommandPack(poolObjectCreate[i]);
	}
	m_poolNext = -1;
}

void  AspectFactoryObjects::Work() {

	if (Scene->IsBreakUpdate())
		return;

	bool isBackgroundFrame = Scene->IsCurrentObjectBackgroundFrameGUI();
	bool isCompleted = false;

	//--- Create object
	CommandPack command = Scene->CurrentSceneCommand;

	if (!command.Enable || command.CommandType != TypeCommand::CreateObject)
		return;

	//bool isCreateStatic = false;
	m_startContructing = false;
	CreateInfo info;
	vec4 posC = command.ValueV4;
	if (posC.w != -1) {
		m_startContructing = true;
		info.Pos = vec3(posC.x, posC.y, posC.z);
	}

	int value = command.Options[Scene->CommandsAttribute.TypeObjectAttr];
	TypeObject typeObj = (TypeObject)value;
	bool isGuiEdit = command.TargetIndex;

	if (isGuiEdit) {
		if (!isBackgroundFrame)
			return;
	}
	else  
	{
		//--- World after building
		if (m_poolNext == -1)
			m_poolNext = 0;

		assert(m_poolNext <= POOL_SIZE);

		poolObjectCreate[m_poolNext] = command;

		m_poolNext++;
		if (m_poolNext > POOL_SIZE) {
			std::cout << "ERROR POOL LIMIT !!!!! \n";
			m_poolNext = 0;
		}

		Scene->ReadCommand(CreateObject);
		return;
	}

	info.Message = string();

	if (typeObj == TypeObject::TextBox)
	{
		Scene->ReadCommand(CreateObject);

		if (command.Description.size() != 0)
			info.Message = command.Description;
		if (info.Message.size() == 0 && command.ValueS.size() != 0)
			info.Message = command.ValueS;
		if (info.Message.size() != 0)
			info.Init = true;

		info.Pos = vec3(1.);
		info.Size = vec2(0.7, 0.1);

		CreateTextBox(info);
		isCompleted = true;
	}
	if (typeObj == TypeObject::Button)
	{
		Scene->ReadCommand(CreateObject);
		CreateButton(info);
		isCompleted = true;
	}
	if (typeObj == TypeObject::EditBox)
	{
		Scene->ReadCommand(CreateObject);
		CreateEditBox(info);
		isCompleted = true;
	}
	if (typeObj == TypeObject::ListBox ||
		typeObj == TypeObject::ListTextBox ||
		typeObj == TypeObject::ListEditBox
		)
	{
		Scene->ReadCommand(CreateObject);
		CreateListBox(command.ValueS, typeObj, info);
		isCompleted = true;
	}
	if (typeObj == TypeObject::ObjectFieldsEdit)
	{
		Scene->ReadCommand(CreateObject);
		CreateObjectFieldsEdit(info);
		isCompleted = true;
	}

	if (!isCompleted) {

		Scene->ReadCommand(CreateObject);

		if (command.Description.size() != 0)
			info.Message = command.Description;
		if (info.Message.size() == 0 && command.ValueS.size() != 0)
			info.Message = command.ValueS;

		//Create object
		string objName = "Obj" + info.Message;
		vec3 color = vec3(0.156, 0.784, 0.301);
		shared_ptr<ObjectData> objCreate = Scene->Storage->AddObjectDefault(objName, typeObj, info.Pos, color);
		isCompleted = true;
	}

	if (isCompleted) {
		Scene->Storage->UpdateObjectsOrders();
		m_startContructing = false;
	}
}

void AspectFactoryObjects::CreateEditBox(CreateInfo info) {
	string caption = "?";
	string childModel;
	string objName;

	shared_ptr<ObjectData> objCreateButton;
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
	shared_ptr<ObjectData> objBackGUI = Scene->ObjectCurrent;
	
	objCreateButton = Scene->Storage->AddChildObject(objBackGUI, caption, childModel, objName, pos, size, Button, vec3(1));
	objCreateButton->ComponentButton->IsToogleButon = true;

		// ---- Object Edit box create	
		Scene->Storage->ControlConstruct(objCreateButton, caption, EditBox);

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

	shared_ptr<ObjectData> objBackGUI = Scene->ObjectCurrent;

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
	shared_ptr<ObjectData> objCreateButton;

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
	shared_ptr<ObjectData> objBackGUI = Scene->ObjectCurrent;
	
	objCreateButton = Scene->Storage->AddChildObject(objBackGUI, caption, childModel, objName, pos, size, Button, vec3(1));

		objCreateButton->ComponentButton->IsToogleButon = false;
		objCreateButton->SceneCommand->CommandType = TypeCommand::None;
	
		auto objCreateTextBox_Data = Scene->Storage->ControlConstruct(objCreateButton, caption, Button);
	
	if (!m_startContructing)
		Scene->AddCommand(ObjectCreated, - 1, objCreateButton->Index);

	if (m_startContructing)
		m_lastObjectCreated = objBackGUI;
}


void AspectFactoryObjects::CreateListBox(string nameListCommand, TypeObject typeListBox, CreateInfo info) {
	string caption;
	string childModel;
	string objName;
	vec3 color = vec3(0.3);

	shared_ptr<ObjectData> objCreateItem_Prev = nullptr;
	shared_ptr<ObjectData> objBaseFrame;
	
	shared_ptr<ObjectData> objCreate;
	//vec2 pos = vec2(1.); //---- Cursor
	float posZ = Scene->Storage->StartPosGUI_Z;

	// ---- background GUI
	shared_ptr<ObjectData> objBackGUI = Scene->ObjectCurrent;

	vector<int> listItemsIndex = vector<int>();
	vector<CommandPack> listCommand = Scene->GetListCommand(nameListCommand);

	//-- Frame list box
	objName = "BaseFrame_ListBox";
	caption = "BaseFrame_ListBox";
	childModel = "FrameModel"; 
	// -- Style
	float border = 0.003;
	float interligne = 0.003;
	float heightItem = 0.054;
	float widthFrame = 0.7;
	float heightFrame = (listCommand.size() * heightItem) + ((listCommand.size() -1) * interligne) + (border*2);
	vec2 sizeFrame = vec2(widthFrame, heightFrame);
	
	vec3 posFrame = vec3(info.Pos.x, info.Pos.y, posZ);

	objBaseFrame = Scene->Storage->AddChildObject(objBackGUI, caption, childModel, objName, posFrame, sizeFrame, ListBox, vec3(1));
	objBaseFrame->IsFocusable = false;
	
	CreateInfo infoItem;
	infoItem.Init = true;
	infoItem.Pos = vec3(posFrame.x + border, posFrame.y + border, posZ);
	infoItem.Size = vec2(sizeFrame.x - (border * 2), heightItem);

	for (CommandPack commItem : listCommand)
	{
		if (typeListBox == ListEditBox)
			infoItem.Message = commItem.ValueS;
		else
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

			objCreate->ComponentButton->IsToogleButon = true;

			auto objCreateEditBox_Data = Scene->Storage->ControlConstruct(objCreate, infoItem.Message, EditBox);

			//Command - create type Obj 
			objCreateEditBox_Data->SceneCommand->CommandType = commItem.CommandType;
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

			objCreate->ComponentButton->IsToogleButon = false;

			//-- create Text item 
			Scene->Storage->ControlConstruct(objCreate, infoItem.Message, Button);
			
			SetCommand(objCreate, commItem);
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
		m_lastObjectCreated = objBaseFrame;
}


void AspectFactoryObjects::CreateObjectFieldsEdit(CreateInfo info) {

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

	m_startContructing = true;

	//CreateInfo info;
	//info.Pos = ""
	float border = 0.01;
	if (!info.Init) {
		info.Pos.x = 1.2;
		info.Pos.y = .1;
	}

	CreateListBox(Scene->CommandsAttribute.ObjectFieldsListCommand, ListTextBox, info);

	info.Pos.x += m_lastObjectCreated->SizePanel.x + border;

	CreateListBox(Scene->CommandsAttribute.ObjectFieldsListCommand, ListEditBox, info);

	//--- save system info
	Scene->Storage->SceneData->IndexObjectFieldsEdit = m_lastObjectCreated->Index;
	m_lastObjectCreated->Name = Scene->Storage->SceneData->NameSystemObjectFieldsEdit;

	m_startContructing = false;

	//m_lastShellCreated
}
