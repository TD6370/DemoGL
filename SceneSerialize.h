#pragma once

#include "CoreSettings.h"

#include <sstream>
#include <iostream>

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
	string Postranslate = "Postranslate: ";
	string Target = "Target: ";
	string ActionObjectCurrent = "ActionObjectCurrent: ";
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
	std::stringstream m_streamObjects;
	std::stringstream m_streamModels;

public:
	//CreatorModelData* Storage;

	SceneSerialize();
	
	virtual ~SceneSerialize();

	void Save(ObjectData obj);

	void Save(shared_ptr<ObjectData> obj);
	
	void Save(ModelData model);

	void Save(shared_ptr<ModelData> model);
	

	void Save();

	//void Load(string path, ObjectData& obj, TypeObject type);
	void Load();

	//-------------

	string Vec3Str(vec3 vec);
};

