#include "RoomSerializeScene.h"

//#include "..\SceneConstruction.h"
//#include "..\GeometryLib.h"
#include "..\ObjectsTypes\ObjectData.h"
#include "..\CreatorModelData.h"
#include "..\ModelData.h"

#include "..\SceneSerialize.h"

//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>


void RoomSerializeScene::Config() {

}

void RoomSerializeScene::Init() {

	_serializer = new SceneSerialize();
}

void RoomSerializeScene::Work() {

	//------ Serialization

	if (Scene->Storage->Inputs->Key == _saveKey &&
		Scene->Storage->Inputs->Action == GLFW_PRESS) {
		
		_serializer->Save(Scene->ObjectCurrent);
		if (Scene->IsLastCurrentObject)
			_serializer->Save();
	}

}