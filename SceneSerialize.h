#pragma once

#include "CoreSettings.h"

#include <sstream>
#include <iostream>
//#include <vector>
#include <map>

using std::map;
using std::string;
using std::vector;
using std::shared_ptr;
using glm::vec3;

class ModelData;
class ObjectData;

struct ObjectFileds {
	string Name = "Name: ";
	string Type = "Type: ";
	string Model = "Model: ";
	string Postranslate = "Pos: ";
	string Target = "Target: ";
	string ActionObjectCurrent = "Action: ";
};

//const GLchar* PathShaderVertex = "basic.vert";
//const GLchar* PathShaderFrag = "basic.frag";
//const char* PathTexture = "./Textures/testTexture.bmp";
//const char* PathModel3D = "./Models3D/monkey.obj";
//
//ConfigUniform ConfUniform;
//
//GLint TrianglesCount = 0;
//GLint IndicesSize = 0;
//GLuint VAO = 0;
//GLuint VBO = 0;
//GLuint ShaderProgram = 0;
//bool IsIndex = false;
//bool IsDebug = false;
////-------------------
//std::vector< glm::vec2 > UV;
////std::vector< glm::vec2 > StartUV;
//std::vector< glm::vec3 > Normals;
//std::vector< glm::vec3 > Vertices;
//std::vector<unsigned int> Indices;
////std::vector<Plane>* Planes;
//
//unsigned int WidthImage;
//unsigned int HeightImage;

struct ModelFileds {
	string Name = "Name: ";
	
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

	SceneSerialize();
	
	virtual ~SceneSerialize();

	//void Save(ObjectData obj);

	void Save(shared_ptr<ObjectData> obj);
	
	//void Save(ModelData model);

	void Save(shared_ptr<ModelData> model);
	
	//-------------
	void Save();

	void Load();
	//-------------

	string Vec3Str(vec3 vec);

	string GetNameType(TypeObject typeObj);
	TypeObject GetTypeObject(string name);
	void AddNextType(TypeObject typeObj, string Name);

	string GetNameType(ActionObject typeObj);
	ActionObject GetTypeAction(string name);
	void AddNextType(ActionObject typeObj, string Name);
};

