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

void RoomSerializeScene::Load() {
	_serializer->Load();
	Scene->Storage->LoadObjects(_serializer->FiledsObjects);
	Scene->Storage->LoadModels(_serializer->FiledsModels);

	IsOnceComplete = true;
}

void RoomSerializeScene::LoadObjects() {
	_serializer->Load(true);
	Scene->Storage->LoadObjects(_serializer->FiledsObjects);

	IsOnceComplete = true;
}


void RoomSerializeScene::Save() {
	if (Scene->IsBreakUpdate())
		return;

	_serializer->Save(Scene->ObjectCurrent);
	//_serializer->Save(Scene->ObjectCurrent->ModelPtr);

	if (Scene->IsLastCurrentObject) {
		for (auto model : Scene->Storage->Models)
		{
			_serializer->Save(model);
		}
		_serializer->Save();
	}
}


void RoomSerializeScene::Work() {

	//------ Serialization

	if (Scene->Storage->Inputs->Key == m_saveKey &&
		Scene->Storage->Inputs->Action == GLFW_PRESS) {
		
		Save();
	}

	//------ Deserialization

	if (Scene->Storage->Inputs->Key == m_loadKey &&
		Scene->Storage->Inputs->Action == GLFW_PRESS) {

		if (!IsOnceComplete)
		{
			//Load();
			LoadObjects();
		}
	}

}