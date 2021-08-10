#include "ObjectListBox.h"
#include "ObjectGUI.h"

void ObjectListBox::InitData()
{
	ObjectGUI::InitData();

	if (Name == EngineData->SceneData->NameSystemObjectFieldsEdit)
	{
		EngineData->SceneData->IndexObjectFieldsEdit = Index;
	}
}