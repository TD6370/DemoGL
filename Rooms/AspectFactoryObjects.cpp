#include "AspectFactoryObjects.h"
#include "AspectDispatcherCommands.h"

//#include "RoomMarkerPlane.h"
//#include "..\GeometryLib.h"
#include "..\SceneConstruction.h"
//#include "..\ObjectsTypes\ObjectData.h"
//#include "..\CreatorModelData.h"
//#include "..\ModelData.h"


void AspectFactoryObjects::Config() {

}

void AspectFactoryObjects::Init() {

}

void AspectFactoryObjects::Work() {

	if (Scene->IsBreakUpdate())
		return;

	if (Scene->CurrentSceneCommand->Enable == false)
		return;

	CommandParams* command = Scene->CurrentSceneCommand;

	//enum SceneCommands { CreateObject, DeleteObject };
	if (command->SceneCommand == CreateObject) {

		Scene->CurrentSceneCommand->Enable = false;
		return;
	}
}
