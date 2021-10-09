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
struct ObjectFileds;
struct ModelFileds; //#Error 3
struct ShellFileds;
struct ObjectFiledsSpecific;

class CreatorModelData {

private:

	World m_WorldSetting;
	vector<shared_ptr<ObjectData>> SceneObjects;
	map<int, string> m_defaultModelNames;
	ModelNames m_namesModels;

public:
	std::shared_ptr<ObjectData> CurrentPolygonObject;

	int LimitSceneObjects = 100;
	int SceneObjectsLastIndex = -1;
	float StartPosGUI_Z = 0.002;
	float StartPosSubGUI_ADD_Z = 0.001;
	float StartPosSystemGUI_Z = 0.005;

	vector<shared_ptr<ModelData>> Models;
	vector<shared_ptr<BaseShell>> ObjectsShells;

	map<string, int> MapModels;
	map<string, int> MapSceneObjects;

	WorldCluster* Clusters;
	SceneLayer* LayerScene;
	map<string, GLuint> ShaderPrograms;

	//TODO: EngineData
	CoreMVP* ConfigMVP;
	ControllerInput* Inputs;
	Operator* Oper;
	Camera* Cam;
	SceneParam* SceneData;

	CreatorModelData();
	~CreatorModelData();

	shared_ptr<ModelData> CreateNextModel();

	void AddModel(shared_ptr<ModelData> newModel, string name);

	shared_ptr<ObjectData> AddObject(const string& name, std::shared_ptr<ModelData> modelPtr, TypeObject p_typeObj,
		vec3 p_pos = vec3(0), vec3 p_color = vec3(0), int p_index = -1, TypeLayer Layer = TypeLayer::LayerNone, bool isLoading = false);

	shared_ptr<ObjectData> AddObjectDefault(string name, TypeObject p_typeObj,
		vec3 p_pos = vec3(0), vec3 p_color = vec3(0), int p_index = -1, TypeLayer Layer = TypeLayer::LayerNone, bool isLoading = false);

	shared_ptr<ObjectData> GetObjectPrt(int index);
	shared_ptr<ObjectData> GetObjectPrt(const string& key);

	shared_ptr<ModelData> GetModelPrt(int index);
	shared_ptr<ModelData> GetModelPrt(const string& key);

	shared_ptr<BaseShell> AddShell(string name, int rootIndex, int captionIndex = -1, bool isLoading = false, vector<int> items = vector<int>());

	shared_ptr<BaseShell> GetObjectShellPrt(int index);

	void FillShellComponents();
	void FillShellComponents(int indObj);

	void ObjectAction(int index);

	void SaveObject(ObjectData* objData);

	void LoadModels();

	void LoadObjects();

	void LoadPlanes();

	void LoadModels(vector<shared_ptr<ModelFileds>> filedsModels);

	void LoadObjects(vector<shared_ptr<ObjectFileds>> filedsObjects, vector<vector<ObjectFiledsSpecific>> objectsDataOthers);

	void SaveAndInitObject(ObjectFileds* objectData, vector<ObjectFiledsSpecific> objectDataSpecific, shared_ptr<ObjectData> newObj = nullptr);

	void LoadShells(vector<shared_ptr<ShellFileds>> filedsShells);

	void LoadObjectsGUI();

	void LoadClusters();

	void Load(bool isFast = false);

	void ClearObjects();

	void ClearModels();

	void LoadShells();
	void LoadShellsLinks();

	bool IsExistObjectByName(const string& key);

	bool IsExistObjectByType(TypeObject type);

	bool IsExistObject(TypeObject typeObj);

	int GetIndexObjBySortInd(int indObj);

	int SceneObjectsSize();

	void UpdateObjectsOrders();

	void UpdateFamilBonds();

	shared_ptr<ObjectData> AddChildObject(shared_ptr<ObjectData> ownerObj, 
		const string& caption,
		const string& nameModel,
		const string& nameObject,
		vec3 position, vec2 size, TypeObject p_typeObj = TypeObject::GUI, vec3 color = vec3(0), TypeLayer p_layer = LayerNone);

	shared_ptr<ObjectData> ControlConstruct(shared_ptr<ObjectData> obj, 
		string caption, TypeObject p_typeObj,
		string nameObj = std::string(), TypeLayer p_layer = LayerNone);
	
	shared_ptr<ObjectData> CreateObjectNull();
};


#endif

