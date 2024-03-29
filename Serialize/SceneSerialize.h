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
class BaseShell;

enum TypeValue { String, Int, Float, Vec2, Vec3, Vec4 };

struct OptionsObject {
	string IsVisible = "IsVisible:";
	string IsGravity = "IsGravity:";
	string IsGUI = "IsGUI:";
	string IsTextureRepeat = "IsTextureRepeat:";
	string IsNPC = "IsNPC:";
	string IsHexagonModel = "IsHexagonModel:";
	string IsSquareModel = "IsSquareModel:";
	string IsAbsolutePosition = "IsAbsolutePosition:";
	string IsFocusable = "IsFocusable:";
	string IsTransformable = "IsTransformable:";
	string IsUsable = "IsUsable:";
	string IsChecked = "IsChecked:";
	string IsTextBoxComponent = "IsTextBoxComponent:";
	bool Value;
};

struct ObjectFileds {
	string Name = "Name:";
	string Type = "Type:";
	string Model = "Model:";
	string Index = "Index:";
	string Layer = "Layer:";
	string Postranslate = "Pos:";
	vec3 PostranslateValue;
	string Target = "Target:";
	vec3 TargetValue;
	string ActionObjectCurrent = "Action:";
	string IndexObjectOwner = "IndexObjectOwner:";
		string ShellIndex = "ShellIndex:";
		string NextItemShellIndex = "NextItemShellIndex:";
	string Color = "Color:";
	vec3 ColorValue;
	OptionsObject Options;
	string Command = "Command:";
	string CommandSourceIndex = "CommandSourceIndex:";
	string CommandTargetIndex = "CommandTargetIndex:";
	string CommandValueI = "CommandValueI:";
	string CommandValueF = "CommandValueF:";
	string CommandValueS = "CommandValueS:";
	string CommandDescription = "CommandDescription:";

	map<string, string> OtherFields;
};


struct ModelFileds {
	string Name = "Name:";
	string PathShaderVertex = "PathShaderVertex:";
	string PathShaderFrag = "PathShaderFrag:";
	string PathTexture = "PathTexture:";
	string PathTextureAtlas = "PathTextureAtlas:";
	string PathModel3D = "PathModel3D:";
	string RadiusCollider = "RadiusCollider:";
	string IsSquareModel = "IsSquareModel:";
	string TypeName = "TypeName:";
};

struct ShellFileds {
	string Name = "Name:";
	string Index = "Index:";
	string RootObjIndex = "RootObjIndex:";
	string CaptionObjIndex = "CaptionObjIndex:";
	string HeadIndexList = "HeadIndexList:";
};

struct ObjectFiledsSpecific {
	string FieldName;
	string Value;
};

string FormatEmptyValue(string value);
string FormatFieldName(string value);

class SceneSerialize
{
private:
	string m_dataObjects;
	string m_dataModels;
	string m_dataShells;
	string m_stringSeparator;
	string m_specificFields;

	map<ActionObject, string> m_mapNamesTypesActions;
	map<string, ActionObject> m_mapTypesActions;

	map<TypeObject, string> m_mapNamesTypesObjects;
	map<string, TypeObject> m_mapTypesObjects;

	map<TypeCommand, string> m_mapNamesTypesCommands;
	map<string, TypeCommand> m_mapTypesCommands;

	map<TypeLayer, string> m_mapNamesTypesLayers;
	map<string, TypeLayer> m_mapTypesLayers;

	map<string, string> m_mapObjectFieldsNameAndValue;
	vector<string> m_mapObjectFieldsNames;

	void AddFieldNameAndValue(std::stringstream& stringStream);
	
public:
	vector<shared_ptr<ObjectFileds>> FiledsObjects;
	vector<vector<ObjectFiledsSpecific>> FiledsObjectsSpecific; //#SaveFieldSpecific
	vector<shared_ptr<ModelFileds>> FiledsModels;
	vector<shared_ptr<ShellFileds>> FiledsShells;

	SceneSerialize();
	
	virtual ~SceneSerialize();

	void Save(shared_ptr<ObjectData> obj, bool isSpecificExist, bool isGet = false);

	void Save(shared_ptr<ModelData> model);

	void Save(shared_ptr<BaseShell> shell);
	
	//#SaveFieldSpecific
	void  SaveSpecific(vector<ObjectFiledsSpecific>& otherFields);
	//-------------
	void Save();

	void CreateStructFileds(std::stringstream& in, ObjectFileds& filedsObj);
	

	void Load(bool isOnlyObjects = false);
	//-------------

	string Vec3Str(vec3 vec);

	string Vec2Str(vec2 vec);

	vec3 StrToVec3(string& value);

	vec2 StrToVec2(string& value);

	string GetStrValue(const string& value);

	const char* GetStrValueToChar(const string& value);

	void SetStrValue(std::ifstream& in, string& value);
	void SetStrValue(std::stringstream& in, string& value);

	string GetNameType(TypeObject typeObj);
	TypeObject GetTypeObject(string name);
	void AddNextType(TypeObject typeObj, string Name);

	string GetNameType(ActionObject typeObj);
	ActionObject GetTypeAction(string name);
	void AddNextType(ActionObject typeObj, string Name);

	string GetNameType(TypeCommand typeObj);
	TypeCommand GetTypeCommands(string name);
	void AddNextType(TypeCommand typeObj, string Name);

	string GetNameType(TypeLayer typeObj);
	TypeLayer GetTypeLayer(string name);
	void AddNextType(TypeLayer typeObj, string Name);

	map<TypeObject, string> GetNamesTypesObjects();

	//-------------- 
	string GetFieldValueByName(string fieldName);
	map<string, string> GetObjectListFieldValue();
	vector<string> GetObjectListFields();
};

