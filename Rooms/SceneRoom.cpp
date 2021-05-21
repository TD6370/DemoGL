#include "SceneRoom.h"
#include "..\SceneConstruction.h"

#include "..\GeometryLib.h"
#include "..\ObjectsTypes\ObjectData.h"
#include "..\CreatorModelData.h"
#include "..\ModelData.h"

#include "..\SceneSerialize.h"


SceneRoom::SceneRoom(std::string name, SceneConstruction* sceneConstructor) {
	Name = name;
	Scene = sceneConstructor;

	//Scene->Cam->camX;
	Init();
}


void SceneRoom::Config() {

}

void SceneRoom::Init() {
	
}

void SceneRoom::Work() {

	//====================== WorkingRooms

	//--- Target - cursor
	/*if (object->Name == "Mon") {
		std::shared_ptr <ObjectData> objectObserver = Storage.GetObjectPrt("Box");
		vec3 target = objectObserver->Postranslate;
		object->Target = vec3(target.x, object->Postranslate.y, target.z);
	}*/

	////--- Object Mon - cursor -- !!!!!!
	//if (ObjectCurrent->Name == "Mon" && Inputs->ParamCase == 2) {
	//	vec4 vecPos = glm::inverse(ConfigMVP->View) * vec4(1);
	//	//vec3 posCursorObject = vec3(vecPos.x, vecPos.y, vecPos.z) + m_OperatorG.m_direction * 25.f;
	//	vec3 posCursorObject = vec3(vecPos.x, ObjectCurrent->Postranslate.y, vecPos.z) + Oper->m_direction * 20.f;
	//	ObjectCurrent->Postranslate = posCursorObject;
	//}

	//--- Mouse camera object
	/*if (object->Name == "Box") {
		vec4 vecPos = glm::inverse(ConfigMVP.View) * vec4(1);
		vec3 posCursorObject = vec3(vecPos.x, vecPos.y, vecPos.z) + m_OperatorG.m_direction * 25.f;
		object->Postranslate = posCursorObject;
	}*/

	//--- Target
	/*if (object->Name == "Box2") {
		std::shared_ptr <ObjectData> objectObserver = Storage.GetObjectPrt("Mon");
		object->Postranslate = objectObserver->Target;
	}*/

	//object->TempVectors.push_back(nearestSphereIntersectionPoint);
	//object->TempVectors.push_back(nearestPolygonIntersectionPoint);
	//if (object->Name == "M_P_1") {
	//	std::shared_ptr <ObjectData> objectObserver = Storage.GetObjectPrt("Mon");
	//	if(objectObserver->TempVectors.size() > 1)
	//		object->Postranslate = objectObserver->TempVectors[0]; //nearestSphereIntersectionPoint - green
	//}

	//----- Light position
	std::shared_ptr <ObjectData> objectLight =  Scene->Storage->GetObjectPrt("Mon"); //Box2
	Scene->Light->positionLight = vec3(objectLight->Postranslate.x, objectLight->Postranslate.y + 15.f, objectLight->Postranslate.z);
	Scene->ModelCurrent->ConfUniform.SetPositionLight(Scene->Light->positionLight);

	//------ Set Mouse position
	Scene->ModelCurrent->ConfUniform.SetPositionMouse(Scene->Storage->Oper->PositionCursorModel);

	//------ Serialization
	int F2_Key = 000;
	if (Scene->Storage->Inputs->Key == F2_Key) {

		SceneSerialize* serializer = new SceneSerialize();
		serializer->Save(Scene->ObjectCurrent);
		//serializer->Save();
		//serializer->Save(Scene->ModelCurrent);
		//serializer->Save(Scene->ObjectCurrent->ModelPtr);
	}

	//=================== end WorkingRooms

}