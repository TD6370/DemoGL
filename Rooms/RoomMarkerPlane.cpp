#include "RoomMarkerPlane.h"
#include "..\GeometryLib.h"
#include "..\SceneConstructor.h"
#include "..\ObjectsTypes\ObjectData.h"
#include "..\CreatorModelData.h"
#include "..\ModelData.h"


void RoomMarkerPlane::Config() {

}

void RoomMarkerPlane::Init() {

}

void RoomMarkerPlane::Work() {

	if (Scene->IsBreakUpdate())
		return;

	string name = Scene->ObjectCurrent->Name;

	if (name == "Plane") {
		vec3 vectorsParams[10];

		Scene->ObjectCurrent->SetParamCase(55);

		std::shared_ptr <ObjectData> objectObserver = Scene->Storage->GetObjectPrt("Mon");
		vector<string> checkedZona;

		if (objectObserver->PlaneDownIndex != -1)
		{
			shared_ptr<Plane> planeV = Scene->ObjectCurrent->GetPlanePrt(objectObserver->PlaneDownIndex);
			vectorsParams[0] = planeV->V0;
			vectorsParams[1] = planeV->V1;
			vectorsParams[1] = planeV->V2;
			Scene->ObjectCurrent->SetVectorsParams(vectorsParams);
		}
	}


	if (name == "M_V_1") {
		std::shared_ptr <ObjectData> objectObserver = Scene->Storage->GetObjectPrt("Mon");
		if (objectObserver->TempVectors.size() > 0)
			Scene->ObjectCurrent->Postranslate = objectObserver->TempVectors[0];
	}
	if (name == "M_V_2") {
		std::shared_ptr <ObjectData> objectObserver = Scene->Storage->GetObjectPrt("Mon");
		if (objectObserver->TempVectors.size() > 1)
			Scene->ObjectCurrent->Postranslate = objectObserver->TempVectors[1];
	}
	if (name == "M_V_3") {
		std::shared_ptr <ObjectData> objectObserver = Scene->Storage->GetObjectPrt("Mon");
		if (objectObserver->TempVectors.size() > 2)
			Scene->ObjectCurrent->Postranslate = objectObserver->TempVectors[2];
	}
	if (name == "M_C_2") {
		std::shared_ptr <ObjectData> objectObserver = Scene->Storage->GetObjectPrt("Mon");
		Scene->ObjectCurrent->Postranslate = objectObserver->PlaneDownPosition;
	}

	if (name == "M_C_1") {
		std::shared_ptr <ObjectData> objectObserver = Scene->Storage->GetObjectPrt("Mon");
		Scene->ObjectCurrent->Postranslate = objectObserver->Postranslate; //nearestPolygonIntersectionPoint - blue
	}
}