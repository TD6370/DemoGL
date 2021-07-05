#pragma once

#ifndef CreatorModelData_H
#define CreatorModelData_H

//#include "ConfigUniformArg.h"
#include "CoreSettings.h"
#include "OperationString.h"
//#include "SceneSerialize.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::vector;
using std::shared_ptr;
using std::map;
using std::string;

class ModelData;
class ObjectData;
class WorldCluster;
struct ControllerInput;
struct ObjectFileds;
struct ModelFileds; //#Error 3
struct ObjectFiledsSpecific;
//enum TypeObject;

class CreatorModelData {
protected:
	

public:
	std::shared_ptr<ObjectData> CurrentPolygonObject;

	int LimitSceneObjects = 100;
	int SceneObjectsLastIndex = -1;
	vector<shared_ptr<ObjectData>> SceneObjects;
	vector<shared_ptr<ModelData>> Models;
	map<string, int> MapModels;
	map<string, int> MapSceneObjects;
	map<TypeObject, int> MapSceneObjectsTypeOffset;
	vector<TypeObject> SortTypeObjects;
	vector<int> SortObjectIndex;
	WorldCluster* Clusters;
	map<string, GLuint> ShaderPrograms;

	CoreMVP* ConfigMVP;
	ControllerInput* Inputs;
	Operator* Oper;
	Camera* Cam;
	SceneParam* SceneData;

	CreatorModelData();
	~CreatorModelData();

	void AddModel(shared_ptr<ModelData> newModel, string name);

	std::shared_ptr<ObjectData> AddObject(string name, std::shared_ptr<ModelData> modelPtr, TypeObject p_typeObj, vec3 p_pos = vec3(0), vec3 p_color = vec3(0), int p_index = -1);

	void GenerateObjects();

	std::shared_ptr<ObjectData> GetObjectPrt(int index);
	std::shared_ptr<ObjectData> GetObjectPrt(string key);

	std::shared_ptr<ModelData> GetModelPrt(int index);
	std::shared_ptr<ModelData> GetModelPrt(string key);

	void ObjectAction(int index);

	void SaveObject(ObjectData* objData);

	void LoadModels();

	void LoadObjects();

	void LoadModels(vector<shared_ptr<ModelFileds>> filedsModels);

	void LoadObjects(vector<shared_ptr<ObjectFileds>> filedsObjects, vector<vector<ObjectFiledsSpecific>> objectsDataOthers);

	void LoadObjectsGUI();

	void LoadClusters();

	void Load();

	void FillSortTypesObjects();

	void ClearObjects();

	void ClearModels();

	bool IsExistObjectByName(string key);

	bool IsExistObjectByType(TypeObject type);

	bool IsExistObject(TypeObject typeObj);

	//shared_ptr<ObjectData> AddChildObject(shared_ptr<ObjectGUI> ownerObj, string caption, string nameModel, string nameObject, vec3 position, vec2 size, TypeObject p_typeObj = TypeObject::GUI, vec3 color = vec3(0));
	shared_ptr<ObjectData> AddChildObject(shared_ptr<ObjectData> ownerObj, string caption, string nameModel, string nameObject, vec3 position, vec2 size, TypeObject p_typeObj = TypeObject::GUI, vec3 color = vec3(0));

	//void ControlConstruct(shared_ptr<ObjectGUI> obj, string caption);
	shared_ptr<ObjectData> ControlConstruct(shared_ptr<ObjectData> obj, string caption, TypeObject p_typeObj);


};


#endif

