#include "AspectFamilyBonds.h"

//
//#include "RoomMarkerPlane.h"
//#include "..\GeometryLib.h"
#include "../SceneConstructor.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../ObjectsTypes/ObjectPhysic.h"
#include "../ObjectsTypes/ObjectCursorRay.h"

#include "../CreatorModelData.h"
#include "../CoreSettings.h"
#include "../GeomertyShapes/ShapeBase.h"	
#include "../GeomertyShapes/ShapeSquare.h"	

//#include "..\ModelData.h"

#include <string>
#include <iostream>

//using std::vector;
//using glm::vec2;
//using glm::vec3;
//using std::string;
using std::shared_ptr;

void AspectFamilyBonds::Config() {

}

void AspectFamilyBonds::Init() {

}

void AspectFamilyBonds::Work() {
	
	if (Scene->IsBreakUpdate())
		return;

	CommandPack* command = &Scene->CurrentSceneCommand;


	
	if (Scene->ObjectCurrent->TypeObj == TypeObject::CursorRay ||
			Scene->ObjectCurrent->TypeObj == TypeObject::BulletHero) {

		if (Scene->Storage->Inputs->MBT != -1) {
			Scene->ObjectCurrent->Click();
		}
	}

	if (!Scene->Storage->SceneData->IsGUI && command->Enable && command->CommandType == EditObjectCommand)
	{
		if (Scene->ObjectCurrent->Index == command->TargetIndex)
		{
			if (command->ValueI == Woking) {

				Scene->ReadCommand(EditObjectCommand);

				//Select object cursor ray
				Scene->ObjectCurrent->MaterialData.Color = vec3(0, 1, 0);
				Scene->ObjectCurrent->IsSelected = true;
				Scene->ObjectCurrent->Target = m_posCursor;
				Scene->ObjectCurrent->SelectedEvent();

				if (command->SourceIndex != -1) {
					//Clear prevous selected Object
					Scene->AddCommand(EditObjectCommand, -1, command->SourceIndex, (int)Clearing);
				}
			}
			if (command->ValueI == Clearing) {

				Scene->ReadCommand(EditObjectCommand);

				//Unselect prevous object
				Scene->ObjectCurrent->IsSelected = false;
				Scene->ObjectCurrent->UnselectedEvent();
			}
			if (command->ValueI == Transforming) {

				Scene->ReadCommand(EditObjectCommand);
				//Transform to new posotion
				Scene->ObjectCurrent->Target = m_posCursor;
				Scene->ObjectCurrent->MeshTransform();
			}
		}
	}
		

	if (Scene->ObjectCurrent->Index == Scene->Storage->SceneData->IndexCursorRayObj)
	{
		m_posCursor = Scene->ObjectCurrent->Postranslate;
	}

	if (Scene->Storage->SceneData->IsGUI && Scene->ObjectCurrent->IsGUI)
	{

		if (m_sizeBackground.x == -1 &&
			Scene->ObjectCurrent->Index == Scene->Storage->SceneData->IndexBackgroundGUIObj)
		{
			//auto  objPh = std::dynamic_pointer_cast<ObjectPhysic>(Scene->ObjectCurrent);
			//m_sizeBackground.x = objPh->Shape->GetLineLenght(0);
			//m_sizeBackground.y = objPh->Shape->GetLineVertLenght(0);
			m_sizeBackground.x = Scene->ObjectCurrent->Shape->GetLineLenght(0);
			m_sizeBackground.y = Scene->ObjectCurrent->Shape->GetLineVertLenght(0);

			Scene->Storage->SceneData->SizeBackgroungGUI = m_sizeBackground;
		}

		if (Scene->ObjectCurrent->IndexObjectOwner != -1)
		{
			auto  objOwnerGUI = Scene->ObjectCurrent->OwnerObj;
			auto  shapeSquare = (ShapeSquare*)Scene->ObjectCurrent->Shape;
			shapeSquare->SetOwnerPosition(m_sizeBackground, objOwnerGUI->StartPos);
		}
	}
}