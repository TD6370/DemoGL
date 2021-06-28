#include "AspectDispatcherCommands.h"
#include "..\SceneConstruction.h"
#include "../Serialize/SceneSerialize.h"

//#include "..\GeometryLib.h"
//#include "..\SceneConstruction.h"
#include "..\ObjectsTypes\ObjectData.h"
#include "..\CreatorModelData.h"
#include "..\ModelData.h"


void AspectDispatcherCommands::Config() {

}

void AspectDispatcherCommands::Init() {
	ActiveCommands = vector<CommandPack>();
}

void AspectDispatcherCommands::Work() {

	if (Scene->IsBreakUpdate())
		return;

	CommandPack* command = Scene->ObjectCurrent->SceneCommand;
	if (command->Enable) {
		CommandPack commandSet = *command;
		ActiveCommands.push_back(commandSet);
		command->Enable = false;
	}

	if (Scene->IsLastCurrentObject)
	{
		if (Scene->CurrentSceneCommand.Enable == false) {
			if (ActiveCommands.size() > 0) {
				//---- Next Command -----------
				Scene->CurrentSceneCommand = ActiveCommands[0];
				Scene->CurrentSceneCommand.Enable = true;
				ActiveCommands.erase(ActiveCommands.begin());
			}
		}
		else
		{
			//----- Clear not working command -----
			if (m_commandPassCount > m_commandPassLimit)
			{
				m_commandPassCount = 0;
				SceneSerialize* serializer = new SceneSerialize();
				std::cout << "Command not work: " << serializer->GetNameType(Scene->CurrentSceneCommand.CommandType) << "\n";
				Scene->CurrentSceneCommand.Enable = false;
			}
			m_commandPassCount++;
		}
	}
}
