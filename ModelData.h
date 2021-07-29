#pragma once

#include "ConfigBuffers.h"
#include "Shaders.h"
#include "LoaderModelObj.h"
#include "LoadBmp.h"
#include "CoreSettings.h"
#include "OperationString.h"

#include <string>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

//OpenGL Mathematics
#include <glm/glm.hpp>

using std::vector;
using glm::vec2;
using glm::vec3;
using std::string;
using std::shared_ptr;
using std::map;

class Plane;
class ModelFrame; //!!!
class ConfigUniform;
class RenderComponent;

class ModelData {

public:
	
	string Name;
	string TypeName;
	string PathShaderFolder = "./Shaders/";
	
	//---- Material Data
	Material MaterialData;

	//---- Mesh data
	Mesh MeshData;

	//---- Render component
	RenderComponent* Render;

	bool IsDebug = false;
	bool IsLoadedIntoMem_Buffer = false;
	//------------------------

	ModelData() {};

	void Init(map<string, GLuint>& shaderPrograms);

	/*void DebugUV(vector<vec2> list_uv);
	void DebugVec3(vector<vec3> list_v, string name);*/
	
};



