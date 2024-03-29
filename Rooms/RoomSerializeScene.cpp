#include "RoomSerializeScene.h"

//#include "../SceneConstructor.h"
//#include "../GeometryLib.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../CreatorModelData.h"
#include "../ModelData.h"

#include "../Serialize/SceneSerialize.h"
#include "../Controllers.h"
//#include "SceneSerialize.h"

#include "../SceneConstructor.h"//##$$

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
	
	Scene->Storage->Load(true);
	IsOnceComplete = true;
	//-------

	/*_serializer->Load();
	Scene->Storage->LoadObjects(_serializer->FiledsObjects, _serializer->FiledsObjectsSpecific);
	Scene->Storage->LoadShells(_serializer->FiledsShells);
	Scene->RefreshGUI();
	IsOnceComplete = true;
	*/
}

void RoomSerializeScene::LoadObjects() {
	_serializer->Load(true);
	//#SaveFieldSpecific
	Scene->Storage->LoadObjects(_serializer->FiledsObjects, _serializer->FiledsObjectsSpecific);

	Scene->RefreshGUI();
	IsOnceComplete = true;
}

bool RoomSerializeScene::IsValidSave(shared_ptr<ObjectData> object) 
{
	/*if ((Scene->ObjectCurrent->TypeObj == TypeObject::CursorRay))
		return false;*/
	return true;
}

void RoomSerializeScene::Save() {

	int indO = 0;
	while (indO <= Scene->Storage->SceneObjectsLastIndex)
	{
		auto obj = Scene->Storage->GetObjectPrt(indO++);
		if (IsValidSave(obj)) {
				
			vector<ObjectFiledsSpecific> specificFiels = obj->GetSpecificFiels();
			bool isSpecificExist = specificFiels.size() != 0;

			_serializer->Save(obj, isSpecificExist);
				
			_serializer->SaveSpecific(specificFiels);
		}
	}

	for (auto model : Scene->Storage->Models)
	{
		_serializer->Save(model);
	}

	for (auto shell : Scene->Storage->ObjectsShells)
	{
		_serializer->Save(shell);
	}

	_serializer->Save();

	IsOnceComplete = false;

}

void RoomSerializeScene::Work() {};

void RoomSerializeScene::LastWork() {

	//------ Serialization
	if (Scene->Storage->Inputs->Key == m_saveKey &&
		Scene->Storage->Inputs->Action == GLFW_PRESS) {
		Save();
	}

	//------ Deserialization
	if (Scene->Storage->Inputs->Key == m_loadKey &&
		Scene->Storage->Inputs->Action == GLFW_PRESS) {

		//if (!IsOnceComplete)
			Load();
	}
}