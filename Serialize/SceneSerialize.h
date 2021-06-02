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
using glm::vec2;

class ModelData;
class ObjectData;

enum TypeValue { String, Int, Float, Vec2, Vec3, Vec4 };


struct ObjectFileds {
	string Name = "Name:";
	string Type = "Type:";
	string Model = "Model:";
	string Postranslate = "Pos:";
	vec3 PostranslateValue;
	string Target = "Target:";
	vec3 TargetValue;
	string ActionObjectCurrent = "Action:";
	string IndexObjectOwner = "IndexObjectOwner:";
	string Color = "Color:";
	vec3 ColorValue;

	map<string, string> OtherFields;
};


struct ModelFileds {
	string Name = "Name:";
	string PathShaderVertex = "PathShaderVertex:";
	string PathShaderFrag = "PathShaderFrag:";
	string PathTexture = "PathTexture:";
	string PathModel3D = "PathModel3D:";
	string RadiusCollider = "RadiusCollider:";
	string IsCubeModel = "IsCubeModel:";
};

struct ObjectFiledsSpecific {
	string FieldName;
	string Value;
};

class SceneSerialize
{
private:

	string m_dataObjects;
	string m_dataModels;
	string m_stringSeparator;
	string m_specificFields;

	map<ActionObject, string> m_mapNamesTypesActions;
	map<string, ActionObject> m_mapTypesActions;

	map<TypeObject, string> m_mapNamesTypesObjects;
	map<string, TypeObject> m_mapTypesObjects;

public:
	vector<shared_ptr<ObjectFileds>> FiledsObjects;
	vector<vector<ObjectFiledsSpecific>> FiledsObjectsSpecific; //#SaveFieldSpecific
	vector<shared_ptr<ModelFileds>> FiledsModels;
	

	SceneSerialize();
	
	virtual ~SceneSerialize();

	void Save(shared_ptr<ObjectData> obj, bool isSpecificExist);

	void Save(shared_ptr<ModelData> model);
	
	//#SaveFieldSpecific
	void  SaveSpecific(vector<ObjectFiledsSpecific>& otherFields);
	//-------------
	void Save();

	void Load(bool isOnlyObjects = false);
	//-------------

	//string GetValue(string nameField, TypeValue value);

	string Vec3Str(vec3 vec);

	string Vec2Str(vec2 vec);

	vec3 StrToVec3(string& value);

	vec2 StrToVec2(string& value);

	string GetNameType(TypeObject typeObj);
	TypeObject GetTypeObject(string name);
	void AddNextType(TypeObject typeObj, string Name);

	string GetNameType(ActionObject typeObj);
	ActionObject GetTypeAction(string name);
	void AddNextType(ActionObject typeObj, string Name);
};

