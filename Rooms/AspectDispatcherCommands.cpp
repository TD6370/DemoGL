#include "AspectDispatcherCommands.h"
#include "..\SceneConstructor.h"
#include "../Serialize/SceneSerialize.h"

//#include "..\GeometryLib.h"
//#include "..\SceneConstructor.h"
#include "..\ObjectsTypes\ObjectData.h"
#include "..\ObjectsTypes\ObjectButton.h"
#include "..\ObjectsTypes\ObjectEditBox.h"
#include "..\ObjectsTypes\ObjectTextBox.h"
#include "..\ObjectsTypes\ObjectGUI.h"
#include "..\CreatorModelData.h"
#include "..\ModelData.h"

AspectDispatcherCommands::~AspectDispatcherCommands(){}

void AspectDispatcherCommands::Config() {

}

void AspectDispatcherCommands::Init() {
	ActiveCommands = vector<CommandPack>();

	LoadStaticCommandList();
}

void AspectDispatcherCommands::LoadStaticCommandList() {

	int sourceIndex = -1;
	int targetIndex = -1;

	CommandPack commandCreateBlock = CommandPack();
	commandCreateBlock.Description = "создать блок";
	commandCreateBlock.CommandType = TypeCommand::CreateObject;
	commandCreateBlock.Options.insert({ Scene->CommandsAttribute.TypeObjectAttr , (int)TypeObject::Block });

	//CommandPack commandT1 = CommandPack();
	//commandT1.Description = "тест";
	//commandT1.CommandType = TypeCommand::None;

	CommandPack commandDeleteObject = CommandPack();
	commandDeleteObject.Description = "удалить";
	commandDeleteObject.CommandType = TypeCommand::DeleteObject;

	vector<CommandPack> baseListCommand = vector<CommandPack>{
		commandCreateBlock, commandDeleteObject
	};
		
	StaticListCommand.insert({ Scene->CommandsAttribute.BaseListCommand, baseListCommand });

	//---- ListBox Types object
	vector<CommandPack> listTypeObjects = vector<CommandPack>();
	CommandPack commandNextType;
	SceneSerialize* serializer = new SceneSerialize();

	map<TypeObject, string> mapTypesNamesObj = serializer->GetNamesTypesObjects();

	for (auto pairTypeName : mapTypesNamesObj) {
		commandNextType = CommandPack();
		commandNextType.SourceIndex = TypeCommand::CreateObject;
		commandNextType.Description = pairTypeName.second;
		commandNextType.CommandType = TypeCommand::SelectItemValue;
		commandNextType.ValueI = pairTypeName.first;
		listTypeObjects.push_back(commandNextType);
	}

	StaticListCommand.insert({ Scene->CommandsAttribute.TypesObjectListCommand, listTypeObjects });

	//--- ListBox Object Fields Edit
	CreateCommandList(nullptr);
}

void AspectDispatcherCommands::CreateCommandList(shared_ptr<ObjectData> obj) {
	vector<CommandPack> listObjectFiels = vector<CommandPack>();
	ObjectFileds fielsInfo = ObjectFileds();
	auto typeComm = TypeCommand::SelectItemValue;
	int sourceIndex = TypeCommand::EditObjectCommand;
	int valueI = -1;

	if (obj == nullptr)
	{
		ModelData model = ModelData();
		shared_ptr<ModelData> modelPrt = std::make_unique<ModelData>(model);
		modelPrt->InitNull();
		ObjectData objNull(-1, modelPrt, TypeObject::Solid, vec3(0));
		objNull.InitData();
		obj = std::make_unique<ObjectData>(objNull);
	}

	map<string, string> listObjFieldsValue = Scene->GetObjectListFieldValue(obj);
	vector<string> listObjFields = Scene->GetObjectListFields();
	string fieldValue = "";

	for (auto fieldName : listObjFields) {
		fieldValue = listObjFieldsValue[fieldName];
		AddCommandInList(listObjectFiels, fieldName, typeComm, sourceIndex, -1, -1, -1, vec4(-1), fieldValue);
	}

	string keyList = Scene->CommandsAttribute.ObjectFieldsListCommand;
	if (StaticListCommand.find(keyList) != StaticListCommand.end())
		StaticListCommand[keyList] = listObjectFiels;
	else
		StaticListCommand.insert({ keyList, listObjectFiels });
}

void AddCommandInList(vector<CommandPack>& listObjectFiels, string description, TypeCommand  commandType, int SourceIndex, int TargetIndex, int ValueI, float valueF, vec4 valueV4, string valueS)
{
	CommandPack commandNextType = CommandPack();
	commandNextType.Description = description;
	commandNextType.SourceIndex = SourceIndex;
	commandNextType.ValueS = valueS; // description;
	commandNextType.CommandType = TypeCommand::SelectItemValue;
	listObjectFiels.push_back(commandNextType);
}

void AspectDispatcherCommands::Work() {

	if (Scene->IsBreakUpdate())
		return;

	CommandPack* commandCurrObj = Scene->ObjectCurrent->SceneCommand;
	if (commandCurrObj->Enable) {
		CommandPack commandSet = *commandCurrObj;
		ActiveCommands.push_back(commandSet);
		commandCurrObj->Enable = false;
		//Scene->Debug("");
	}

	if (Scene->IsLastCurrentObject || 
		Scene->IsFirstCurrentObject)
	{
		if (Scene->CurrentSceneCommand.Enable == false) {
			if (ActiveCommands.size() > 0) {
				//---- Next Command -----------
				Scene->CurrentSceneCommand = ActiveCommands[0];
				Scene->CurrentSceneCommand.Enable = true;
				ActiveCommands.erase(ActiveCommands.begin());

				DebugReadCommand(&Scene->CurrentSceneCommand, "CurrentSceneCommand");
			}
		}
		else
		{
			if (Scene->CurrentSceneCommand.CommandType == TypeCommand::None) {
				Scene->CurrentSceneCommand.Enable = false;
				return;
			}

			if (m_commandLast != Scene->CurrentSceneCommand.CommandType) {
				m_commandLast = Scene->CurrentSceneCommand.CommandType;
				m_commandPassCount = 0;
				return;
			}

			/*if (Scene->CurrentSceneCommand.IsLongCommand)
				return;*/

			//----- Clear not working command -----
			if (m_commandPassCount > m_commandPassLimit)
			{
				m_commandPassCount = 0;
				
				if (Scene->CurrentSceneCommand.IsLongCommand) {
				
					if (ActiveCommands.size() != -0)
					{
						AddCommand(Scene->CurrentSceneCommand);
						Scene->CurrentSceneCommand.Enable = false;
					}
				}
				else {

					SceneSerialize* serializer = new SceneSerialize();
					std::cout << "Command not work: " << serializer->GetNameType(Scene->CurrentSceneCommand.CommandType) << "\n" << 
						"---------------------------------------------------\n";;
					Scene->CurrentSceneCommand.Enable = false;
				}
			}
			m_commandPassCount++;
		}
	}
}


void AspectDispatcherCommands::AddCommand(CommandPack command) {

	if (command.Enable) {
		ActiveCommands.push_back(command);
	}
}

void AspectDispatcherCommands::AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex, string keyOptions, int valueOptions) {

	CommandPack command = CommandPack();
	command.Enable = true;
	command.CommandType = commandType;
	command.SourceIndex = sourceIndex;
	command.TargetIndex = targetIndex;
	if (keyOptions.size() != 0) {
		command.Options.clear();
		command.Options.insert(std::pair<string, int>(keyOptions, valueOptions));
	}
	ActiveCommands.push_back(command);
}

void AspectDispatcherCommands::AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex, vector<string> keyOptions, vector<int> valueOptions, 
	int valueI, float valueF, vec4 valueV4, string valueS, string description, bool isLong)
{
	CommandPack command = CommandPack();
	command.Enable = true;
	command.Description = description;
	command.CommandType = commandType;
	command.SourceIndex = sourceIndex;
	command.TargetIndex = targetIndex;
	command.ValueI = valueI;
	command.ValueF = valueF;
	command.ValueV4 = valueV4;
	command.ValueS = valueS;
	for(int i=0; i< keyOptions.size();i++)
	{
		command.Options.insert(std::pair<string, int>(keyOptions[i], valueOptions[i]));
	}
	ActiveCommands.push_back(command);
}

void AspectDispatcherCommands::AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex,
	int valueI , float valueF, vec4 valueV4, string valueS, string description, bool isLong)
{

	CommandPack command = CommandPack();
	command.Enable = true;
	command.Description = description;
	command.CommandType = commandType;
	command.SourceIndex = sourceIndex;
	command.TargetIndex = targetIndex;
	command.ValueS = valueS;
	command.ValueI = valueI;
	command.ValueF = valueF;
	command.ValueV4 = valueV4;
	command.IsLongCommand = isLong;
	
	ActiveCommands.push_back(command);
}

void SetCommand(shared_ptr<ObjectData> obj, CommandPack command) {

	obj->SceneCommand->CommandType = command.CommandType;
	obj->SceneCommand->SourceIndex = command.SourceIndex;
	obj->SceneCommand->TargetIndex = command.TargetIndex;
	obj->SceneCommand->Description = command.Description;

	obj->SceneCommand->ValueI = command.ValueI;
	obj->SceneCommand->ValueF = command.ValueF;
	obj->SceneCommand->ValueV4 = command.ValueV4;
	obj->SceneCommand->ValueS = command.ValueS;
	obj->SceneCommand->IsLongCommand = command.IsLongCommand;

	obj->SceneCommand->Options.clear();

	for (auto item : command.Options)
	{
		obj->SceneCommand->Options.insert(item);
	}
}

void SetCommand(shared_ptr<ObjectData> obj, TypeCommand commandType, int sourceIndex, int targetIndex, string keyOptions, int valueOptions, 
	int valueI, float valueF, vec4 valueV4, string valueS, string description) {

	obj->SceneCommand->CommandType = commandType;
	obj->SceneCommand->SourceIndex = sourceIndex;
	obj->SceneCommand->TargetIndex = targetIndex;

	obj->SceneCommand->ValueI = valueI;
	obj->SceneCommand->ValueF = valueF;
	obj->SceneCommand->ValueV4 = valueV4;
	obj->SceneCommand->ValueS = valueS;
	obj->SceneCommand->Description;

	if(sourceIndex == -1)
		obj->SceneCommand->SourceIndex = obj->Index;
		
	if (keyOptions.size() != 0) {
		obj->SceneCommand->Description = obj->Name;
		obj->SceneCommand->Options.clear();
		obj->SceneCommand->Options.insert(std::pair<string, int>(keyOptions, valueOptions));
		obj->SceneCommand->ValueI = valueOptions;
	}
	
	TypeObject typeObj = obj->TypeObj;
	if (typeObj == TypeObject::Button) {
		auto objButton = std::dynamic_pointer_cast<ObjectButton>(obj);
		if (objButton != nullptr) {
			if(objButton->IsToogleButon)
			{
				//-- command tooble button
				objButton->SceneCommand->Options.clear();
				objButton->SceneCommand->Options.insert(std::pair<string, int>(objButton->Name, (int)objButton->IsChecked));
				objButton->SceneCommand->Description = objButton->Name;
				objButton->SceneCommand->ValueI = (int)objButton->IsChecked;
			}
		}
	}
}

void SetCommand(shared_ptr<ObjectData> obj, TypeCommand commandType, int sourceIndex, int targetIndex,
	int valueI, float valueF, vec4 valueV4, string valueS, string description) {
	SetCommand(obj, commandType, sourceIndex, targetIndex, "", -1,
		valueI, valueF, valueV4, valueS, description);
}

void SetCommand(shared_ptr<ObjectData> obj, TypeCommand commandType,
	int valueI, string valueS, float valueF, vec4 valueV4, string description) {
	SetCommand(obj, commandType, -1, -1, "", -1,
		valueI, valueF, valueV4, valueS, description);
}

void AspectDispatcherCommands::DebugReadCommand(CommandPack* command, string name) {

	SceneSerialize* serializer = new SceneSerialize();

	string typeComm = serializer->GetNameType(command->CommandType);
	string infoOpt = "";
	string infoObjS = "";
	string infoObjT = "";
	shared_ptr<ObjectData> obj;
	if (command->Options.size() != 0)
	{
		for (auto opt : command->Options)
		{
			infoOpt += opt.first + " : " + std::to_string(opt.second) + "\n";
		}
	}
	if (command->SourceIndex != -1) {
		obj = Scene->Storage->GetObjectPrt(command->SourceIndex);
		infoObjS = " > " + obj->Name + " : " + serializer->GetNameType(obj->TypeObj);
	}
	if (command->TargetIndex != -1) {
		obj = Scene->Storage->GetObjectPrt(command->TargetIndex);
		infoObjT = " > " + obj->Name + " : " + serializer->GetNameType(obj->TypeObj);
	}

	std::cout << name << " : " << typeComm << "\n" <<
		" D: " << command->Description << "\n" <<
		" S: " << command->SourceIndex << " > " << infoObjS << "\n" <<
		" T: " << command->TargetIndex << " > " << infoObjT << "\n" <<
		" I: " << command->ValueI << "\n" <<
		" S: " << command->ValueS << "\n" <<
		infoOpt << "\n" <<
		"---------------------------------------------------\n";
}

void AddCommandOptions(shared_ptr<ObjectData> obj, string keyOptions, int valueOptions) {

	TypeObject typeObj = obj->TypeObj;

	if (keyOptions.size() != 0) {
		obj->SceneCommand->Description = obj->Name;
		obj->SceneCommand->Options.insert(std::pair<string, int>(keyOptions, valueOptions));
		obj->SceneCommand->ValueI = valueOptions;
	}
}

void UpdateCommandOptions(shared_ptr<ObjectData> obj, string keyOptions, int valueOptions) {

	TypeObject typeObj = obj->TypeObj;

	if (keyOptions.size() != 0) {
		obj->SceneCommand->Description = obj->Name;
		obj->SceneCommand->Options[keyOptions] = valueOptions;
		obj->SceneCommand->ValueI = valueOptions;
	}
}

void UpdateCommandOptions(ObjectData* obj, string keyOptions, int valueOptions) {

	TypeObject typeObj = obj->TypeObj;

	if (keyOptions.size() != 0) {
		obj->SceneCommand->Description = obj->Name;
		obj->SceneCommand->Options[keyOptions] = valueOptions;
		obj->SceneCommand->ValueI = valueOptions;
	}
}




