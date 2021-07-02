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
