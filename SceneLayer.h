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

class A
{
	A() {};

	void virtual CallTest() {
		std::cout << "CallTest A ";
	};
};

class B : A
{
	B();

	void CallTest() {
		std::cout << "CallTest B ";
	};
};

class C : B
{
	C();

	void CallTest() {
		std::cout << "CallTest C ";
	};
};

class SceneLayer
{
public:
	map<TypeObject, int> MapSceneObjectsTypeOffset;
	vector<TypeObject> SortTypeObjects;
	vector<int> SortObjectIndex;

	SceneLayer() : MapSceneObjectsTypeOffset(), SortObjectIndex(), SortTypeObjects() {
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

