#include "AspectDispatcherCommands.h"
#include "..\SceneConstruction.h"

//#include "..\GeometryLib.h"
//#include "..\SceneConstruction.h"
#include "..\ObjectsTypes\ObjectData.h"
#include "..\CreatorModelData.h"
#include "..\ModelData.h"


void AspectDispatcherCommands::Config() {

}

void AspectDispatcherCommands::Init() {
	SceneCommands = vector<CommandParams*>();
	//SceneCommands = vector<CommandParams>();
}

void AspectDispatcherCommands::Work() {

	if (Scene->IsBreakUpdate())
		return;

	if (Scene->CurrentSceneCommand->Enable == false) {
		if (SceneCommands.size() > 0) {
			//---- Next Command -----------
			//Scene->CurrentSceneCommand = &SceneCommands[0];
			Scene->CurrentSceneCommand = SceneCommands[0];
			SceneCommands.erase(SceneCommands.begin());
		}
	}

	if (!Scene->ObjectCurrent->SceneCommand->Enable)
		return;

	CommandParams* command = Scene->ObjectCurrent->SceneCommand;
	command->Enable = true;
	SceneCommands.push_back(command);

	//CommandParams* command = Scene->ObjectCurrent->SceneCommand;
	//command->Enable = true;
	//SceneCommands.push_back(*command);
		
	Scene->ObjectCurrent->SceneCommand->Enable = false;
}
