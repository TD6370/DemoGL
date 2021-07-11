#include "SceneLayer.h"
#include "ObjectsTypes/ObjectData.h"


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
	int IndexInsert = 0;
	int index = obj->Index;
	map <TypeObject, int> ::iterator it;

	for (TypeObject typeObj : SortTypeObjects)
	{
		it = MapSceneObjectsTypeOffset.find(typeObj);
		if (it != MapSceneObjectsTypeOffset.end())
			IndexInsert += MapSceneObjectsTypeOffset[typeObj];
		if (typeObj == p_typeObj)
			break;
	}

	it = MapSceneObjectsTypeOffset.find(p_typeObj);
	if (it != MapSceneObjectsTypeOffset.end()) {
		MapSceneObjectsTypeOffset[p_typeObj]++;
	}
	else {
		MapSceneObjectsTypeOffset.insert(std::pair<TypeObject, int>(p_typeObj, 1));
	}

	//==========  TEST Sort
	/*for (auto indNext : SortObjectIndex) {

		auto currType =  SceneObjects[indNext]->TypeObj;
		SceneSerialize* serializer = new SceneSerialize();
		std::cout << "next Index Type: " << serializer->GetNameType(currType) << "\n";
	}
	std::cout << "================================\n";*/

	SortObjectIndex.insert(SortObjectIndex.begin() + IndexInsert, index);
}

void SceneLayer::Clear() {
	if (MapSceneObjectsTypeOffset.size() != 0)
		MapSceneObjectsTypeOffset.clear();
	if (SortObjectIndex.size() != 0)
		SortObjectIndex.clear();
}