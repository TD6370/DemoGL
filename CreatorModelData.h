#pragma once

#ifndef CreatorModelData_H
#define CreatorModelData_H

//#include "ConfigUniformArg.h"
#include "CoreSettings.h"
#include "OperationString.h"
//#include "SceneLayer.h"

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
class BaseShell;
class SceneLayer;
struct ControllerInput;
struct ObjectFileds;
struct ModelFileds; //#Error 3
struct ShellFileds;
struct ObjectFiledsSpecific;
//enum TypeObject;

class CreatorModelData {

private:
	vector<shared_ptr<ObjectData>> SceneObjects;
	//vector<ObjectData> SceneObjectsV;

public:
	std::shared_ptr<ObjectData> CurrentPolygonObject;

	int LimitSceneObjects = 100;
	int SceneObjectsLastIndex = -1;

	vector<shared_ptr<ModelData>> Models;
	vector<shared_ptr<BaseShell>> ObjectsShells;

	map<string, int> MapModels;
	map<string, int> MapSceneObjects;

	WorldCluster* Clusters;
	SceneLayer* LayerScene;
	map<string, GLuint> ShaderPrograms;

	CoreMVP* ConfigMVP;
	ControllerInput* Inputs;
	Operator* Oper;
	Camera* Cam;
	SceneParam* SceneData;

	CreatorModelData();
	~CreatorModelData();

	void AddModel(shared_ptr<ModelData> newModel, string name);

	shared_ptr<ObjectData> AddObject(string name, std::shared_ptr<ModelData> modelPtr, TypeObject p_typeObj, vec3 p_pos = vec3(0), vec3 p_color = vec3(0), int p_index = -1);

	shared_ptr<ObjectData> GetObjectPrt(int index);
	shared_ptr<ObjectData> GetObjectPrt(string key);

	shared_ptr<ModelData> GetModelPrt(int index);
	shared_ptr<ModelData> GetModelPrt(string key);

	shared_ptr<BaseShell> AddShell(string name, int rootIndex, int captionIndex = -1);
	shared_ptr<BaseShell> GetObjectShellPrt(int index);
	void FillShellComponents();
	void FillShellComponents(int indObj);

	void ObjectAction(int index);

	void SaveObject(ObjectData* objData);

	void LoadModels();

	void LoadObjects();

	void LoadModels(vector<shared_ptr<ModelFileds>> filedsModels);

	void LoadObjects(vector<shared_ptr<ObjectFileds>> filedsObjects, vector<vector<ObjectFiledsSpecific>> objectsDataOthers);

	void LoadShells(vector<shared_ptr<ShellFileds>> filedsShells);

	void LoadObjectsGUI();

	void LoadClusters();

	void Load();

	void ClearObjects();

	void ClearModels();

	void LoadShells();

	bool IsExistObjectByName(string key);

	bool IsExistObjectByType(TypeObject type);

	bool IsExistObject(TypeObject typeObj);

	int GetIndexObjBySortInd(int indObj);

	int SceneObjectsSize();

	//shared_ptr<ObjectData> AddChildObject(shared_ptr<ObjectGUI> ownerObj, string caption, string nameModel, string nameObject, vec3 position, vec2 size, TypeObject p_typeObj = TypeObject::GUI, vec3 color = vec3(0));
	shared_ptr<ObjectData> AddChildObject(shared_ptr<ObjectData> ownerObj, string caption, string nameModel, string nameObject, vec3 position, vec2 size, TypeObject p_typeObj = TypeObject::GUI, vec3 color = vec3(0));

	//void ControlConstruct(shared_ptr<ObjectGUI> obj, string caption);
	shared_ptr<ObjectData> ControlConstruct(shared_ptr<ObjectData> obj, string caption, TypeObject p_typeObj, string nameObj = std::string());
		
};


#endif

