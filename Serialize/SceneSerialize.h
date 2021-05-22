#pragma once

#include "../CoreSettings.h"

#include <sstream>
#include <iostream>
#include <vector>
#include <map>

using std::map;
using std::string;
using std::vector;
using std::shared_ptr;
using glm::vec3;

class ModelData;
class ObjectData;

enum TypeValue { String, Int, Float, Vec2, Vec3, Vec4 };

//struct FieldValue {
//	string Value;
//	TypeValue Type;
//};

struct ObjectFileds {
	string Name = "Name:";
	string Type = "Type:";
	string Model = "Model:";
	string Postranslate = "Pos:";
	vec3 PostranslateValue;
	string Target = "Target:";
	vec3 TargetValue;
	string ActionObjectCurrent = "Action:";

	map<string, string> OtherFields;
};

struct ModelFileds {
	string Name = "Name:";
	string PathShaderVertex = "PathShaderVertex:";
	string PathShaderFrag = "PathShaderFrag:";
	string PathTexture = "PathTexture:";
	string PathModel3D = "PathModel3D:";
};

class SceneSerialize
{
private:

	string m_dataObjects;
	string m_dataModels;
	string m_stringSeparator;

	map<ActionObject, string> m_mapNamesTypesActions;
	map<string, ActionObject> m_mapTypesActions;

	map<TypeObject, string> m_mapNamesTypesObjects;
	map<string, TypeObject> m_mapTypesObjects;

public:
	vector<shared_ptr<ObjectFileds>> FiledsObjects;
	vector<shared_ptr<ModelFileds>> FiledsModels;

	SceneSerialize();
	
	virtual ~SceneSerialize();

	void Save(shared_ptr<ObjectData> obj);

	void Save(shared_ptr<ModelData> model);
	
	void  SaveOthers(map<string, string> otherFields);
	//-------------
	void Save();

	void Load(bool isOnlyObjects = false);
	//-------------

	//string GetValue(string nameField, TypeValue value);

	string Vec3Str(vec3 vec);

	string GetNameType(TypeObject typeObj);
	TypeObject GetTypeObject(string name);
	void AddNextType(TypeObject typeObj, string Name);

	string GetNameType(ActionObject typeObj);
	ActionObject GetTypeAction(string name);
	void AddNextType(ActionObject typeObj, string Name);
};

