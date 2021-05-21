#pragma once

#ifndef CreatorModelData_H
#define CreatorModelData_H

#include "ConfigUniformArg.h"
#include "CoreSettings.h"

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
class ControllerInput;

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
	WorldCluster* Clusters;
		
	CoreMVP* ConfigMVP;
	ControllerInput* Inputs;
	Operator* Oper;
	Camera* Cam;
	SceneParam* SceneData;

	CreatorModelData();
	~CreatorModelData();

	void AddModel(ModelData* newModel, string name);

	std::shared_ptr<ObjectData> AddObject(string name, std::shared_ptr<ModelData> modelPtr, TypeObject p_typeObj, vec3 p_pos = vec3(0), vec3 p_color = vec3(0));

	void GenerateObjects();

	std::shared_ptr<ObjectData> GetObjectPrt(int index);
	std::shared_ptr<ObjectData> GetObjectPrt(string key);

	std::shared_ptr<ModelData> GetModelPrt(int index);
	std::shared_ptr<ModelData> GetModelPrt(string key);

	void ObjectAction(int index);

	void SaveObject(ObjectData* objData);

	void LoadModels();

	void LoadObjects();

	void LoadObjectsGUI();

	void LoadClusters();

	void Load();
};


#endif

