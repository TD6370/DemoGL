#include "RoomSerializeScene.h"

//#include "../SceneConstruction.h"
//#include "../GeometryLib.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../CreatorModelData.h"
#include "../ModelData.h"

#include "../Serialize/SceneSerialize.h"
#include "../Controllers.h"
//#include "SceneSerialize.h"

#include "../SceneConstruction.h"//##$$

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
	Scene->Storage->LoadShells(_serializer->FiledsShells);
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

	if (Scene->IsLastCurrentObject) {

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

	}
}

void RoomSerializeScene::Work() {

	//------ Serialization

	if (Scene->Storage->Inputs->Key == m_saveKey &&
		Scene->Storage->Inputs->Action == GLFW_PRESS) {
		
		/*if (!m_startSave) 
		{
			m_startSave = true;
			m_calculateSave = true;
		}*/
		Save();
	}

	/*if (m_startSave) {
		Save();
	}*/

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