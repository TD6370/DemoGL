#include "RoomSerializeScene.h"

//#include "../SceneConstruction.h"
//#include "../GeometryLib.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../CreatorModelData.h"
#include "../ModelData.h"

#include "../Serialize/SceneSerialize.h"
//#include "SceneSerialize.h"

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
	Scene->Storage->LoadObjects(_serializer->FiledsObjects, _serializer->FiledsObjectsSpecific);
	Scene->Storage->LoadModels(_serializer->FiledsModels);

	IsOnceComplete = true;
}

void RoomSerializeScene::LoadObjects() {
	_serializer->Load(true);
	//#SaveFieldSpecific
	Scene->Storage->LoadObjects(_serializer->FiledsObjects, _serializer->FiledsObjectsSpecific);

	IsOnceComplete = true;
}

bool RoomSerializeScene::IsValidSave(shared_ptr<ObjectData> object) 
{
	if ((Scene->ObjectCurrent->TypeObj == TypeObject::CursorRay))
		return false;
	return true;
}

void RoomSerializeScene::Save() {

	if (Scene->IsBreakUpdate())
		return;

	if (IsValidSave(Scene->ObjectCurrent)) {
		
		//TEST
		//auto str = Scene->ObjectCurrent->Name;
		//auto str2 = Scene->ObjectCurrent->TypeObj;

		//#SaveFieldSpecific
		vector<ObjectFiledsSpecific> specificFiels = Scene->ObjectCurrent->GetSpecificFiels();
		bool isSpecificExist = specificFiels.size() != 0;
		
		_serializer->Save(Scene->ObjectCurrent, isSpecificExist);

		
		//vector<ObjectFiledsSpecific> specificFiels = Scene->ObjectCurrent->GetSpecificFiels();
		_serializer->SaveSpecific(specificFiels);
	}

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