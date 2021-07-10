#include "AspectDispatcherCommands.h"
#include "..\SceneConstruction.h"
#include "../Serialize/SceneSerialize.h"

//#include "..\GeometryLib.h"
//#include "..\SceneConstruction.h"
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

	if (Scene->IsLastCurrentObject)
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

			//----- Clear not working command -----
			if (m_commandPassCount > m_commandPassLimit)
			{
				m_commandPassCount = 0;
				//return;
				SceneSerialize* serializer = new SceneSerialize();
				std::cout << "Command not work: " << serializer->GetNameType(Scene->CurrentSceneCommand.CommandType) << "\n";
				Scene->CurrentSceneCommand.Enable = false;
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

void AspectDispatcherCommands::AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex,
	int valueI , float valueF, vec4 valueV4, string valueS)
{

	CommandPack command = CommandPack();
	command.Enable = true;
	command.CommandType = commandType;
	command.SourceIndex = sourceIndex;
	command.TargetIndex = targetIndex;
	command.ValueS = valueS;
	command.ValueI = valueI;
	command.ValueF = valueF;
	command.ValueV4 = valueV4;
	
	ActiveCommands.push_back(command);
}
//void AspectDispatcherCommands::SetCommand(shared_ptr<ObjectData> obj, TypeCommand commandType, int targetIndex, int sourceIndex, string keyOptions, int valueOptions) {
void SetCommand(shared_ptr<ObjectData> obj, TypeCommand commandType, int sourceIndex, int targetIndex, string keyOptions, int valueOptions) {

	obj->SceneCommand->CommandType = commandType;
	obj->SceneCommand->SourceIndex = sourceIndex;
	obj->SceneCommand->TargetIndex = targetIndex;
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




