#pragma once
//#include "SceneLayer.h"

//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "CoreSettings.h"

#include <vector>
#include <string>
#include <iostream>
#include <map>

using std::vector;
using std::map;

using std::shared_ptr;
using std::string;
//using glm::vec3;

class ObjectData;

class CreatorModelData;

class SceneLayer
{
private:
	CreatorModelData* m_storage = nullptr;

public:
	map<TypeObject, int> MapSceneObjectsTypeOffset;
	vector<TypeObject> SortTypeObjects;
	vector<int> SortObjectIndex;
	

	SceneLayer(CreatorModelData* storage) : MapSceneObjectsTypeOffset(), SortObjectIndex(), SortTypeObjects(), m_storage(storage) {
		FillSortTypesObjects();
	};

	/*SceneLayer() {
		MapSceneObjectsTypeOffset = map<TypeObject, int>();
		SortObjectIndex = vector<int>();
		SortTypeObjects = vector<TypeObject>();
		FillSortTypesObjects();
	};*/
	~SceneLayer() {};

	void SaveOrderIndex(shared_ptr<ObjectData> obj);

	void FillSortTypesObjects();

	void Clear();

	int GetSortIndexByType(TypeObject value);

	bool IsNeedSort(int indObj);
};

