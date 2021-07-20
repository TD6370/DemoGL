#include "SceneLayer.h"
#include "ObjectsTypes/ObjectData.h"
#include "CreatorModelData.h"
#include "Serialize/SceneSerialize.h"
#include "ObjectsTypes\ObjectGUI.h"

void SceneLayer::FillSortTypesObjects() {

	//enum TypeObject { Polygon, Solid, Block, Tree, Terra, NPC, Bullet, Hero, BulletHero, CursorRay, GUI, TextBox, CursorGUI, Button
	SortTypeObjects.push_back(Polygon);
	SortTypeObjects.push_back(Hero);
	SortTypeObjects.push_back(CursorRay);
	SortTypeObjects.push_back(BulletHero);
	SortTypeObjects.push_back(Solid);
	SortTypeObjects.push_back(Block);
	SortTypeObjects.push_back(Terra);
	SortTypeObjects.push_back(NPC);
	SortTypeObjects.push_back(Bullet);
	SortTypeObjects.push_back(Tree);
	SortTypeObjects.push_back(Terra);
	SortTypeObjects.push_back(GUI);
	SortTypeObjects.push_back(Button);
	SortTypeObjects.push_back(TextBox);
	SortTypeObjects.push_back(EditBox);
	SortTypeObjects.push_back(CursorGUI);
	//SortTypeObjects.push_back();
}

void SceneLayer::SaveOrderIndex(shared_ptr<ObjectData> obj) {

	//--------------------------------- Save Order Index 
	TypeObject p_typeObj = obj->TypeObj;
	int indexInsert = 0;
	int conutItemsCurrType = 0;
	int index = obj->Index;
	map <TypeObject, int> ::iterator it;
	

	//---------------------- ver 0.
	/*for (TypeObject typeObj : SortTypeObjects)
	{
		it = MapSceneObjectsTypeOffset.find(typeObj);
		if (it != MapSceneObjectsTypeOffset.end())
			indexInsert += MapSceneObjectsTypeOffset[typeObj];
		if (typeObj == p_typeObj)
			break;
	}*/

	//---------------------- ver 1.
	for (TypeObject typeObj : SortTypeObjects)
	{
		it = MapSceneObjectsTypeOffset.find(typeObj);
		conutItemsCurrType = 0;
		if (it != MapSceneObjectsTypeOffset.end())
			conutItemsCurrType = MapSceneObjectsTypeOffset[typeObj];
		if (typeObj == p_typeObj)
			break;
		else
			indexInsert += conutItemsCurrType;
	}

	int countStack = indexInsert + conutItemsCurrType;
	float lastZ = -1;

	if (obj->IsGUI)
	{
		int indNN;
		for (indNN = indexInsert; indNN < countStack; indNN++) {
			int indNext = SortObjectIndex[indNN];
			auto nextObj = m_storage->GetObjectPrt(indNext);

			if (nextObj->Layer > obj->Layer) 
				break;

			lastZ = nextObj->GetZ();

			if (nextObj->Layer < obj->Layer)
				continue;

			if (obj->ShellIndex != -1 && nextObj->ShellIndex == obj->ShellIndex)
			{
				int sortIndObj = GetSortIndexByType(obj->TypeObj);
				int sortIndNextObj = GetSortIndexByType(nextObj->TypeObj);
				if(sortIndNextObj > sortIndObj)
					break;
				if (sortIndNextObj < sortIndObj)
					continue;
			}
			else {
				if (nextObj->GetTopLayer() > obj->GetTopLayer())
				{
					break;
				}
			}
		}
		indexInsert = indNN;
	}
	//------------------------

	//============ Update Z  -- //TEST Z
	if (obj->IsGUI && obj->Layer == TypeLayer::LayerGUI) {
		
		float myZ = obj->GetZ();
		if (lastZ >= myZ) {
			float add = 0.0001;
			add = 0.0002;
			obj->SetZ(lastZ + add);
		}
	}
	//===============

	it = MapSceneObjectsTypeOffset.find(p_typeObj);
	if (it != MapSceneObjectsTypeOffset.end()) {
		MapSceneObjectsTypeOffset[p_typeObj]++;
	}
	else {
		MapSceneObjectsTypeOffset.insert(std::pair<TypeObject, int>(p_typeObj, 1));
	}

	SortObjectIndex.insert(SortObjectIndex.begin() + indexInsert, index);

	//==========  TEST Sort
	if (obj->IsGUI)
	{
		int pp = 0;
		std::cout << "Sort:\n";
		for (auto indNext : SortObjectIndex) {
			auto objDebug = m_storage->GetObjectPrt(indNext);
			std::cout << objDebug->GetInfo() << "\n";
		}
		std::cout << "================================\n";
		auto v = "";
	}
	//========================
}

void SceneLayer::Clear() {
	if (MapSceneObjectsTypeOffset.size() != 0)
		MapSceneObjectsTypeOffset.clear();
	if (SortObjectIndex.size() != 0)
		SortObjectIndex.clear();
}

int SceneLayer::GetSortIndexByType(TypeObject value)
{
	vector<TypeObject> list = SortTypeObjects;
	auto it = find(list.begin(), list.end(), value);
	if (it != list.end())
	{
		int index = it - list.begin();
		return index;
	}
	return -1;
}

bool SceneLayer::IsNeedSort(int index)
{
	auto list = SortObjectIndex;
	auto it = find(list.begin(), list.end(), index);

	return it == list.end();
}