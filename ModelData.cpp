//@@@
#include "ModelData.h"
//#include "BindHeader.h"

#include "ConfigBuffers.h"
#include "Shaders.h"
#include "ConfigUniformArg.h"
#include "LoaderModelObj.h"
#include "LoadBmp.h"
#include "GeometryLib.h"
#include "FileReader.h"
#include "./Components/RenderComponent.h"

#include <string>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

//OpenGL Mathematics
#include <glm/glm.hpp>

using std::vector;
using std::map;
using glm::vec3;
using glm::vec2;

//#using <system.dll>
//using namespace System::Diagnostics;


void ModelData::InitNull() {
	TypeName = FormatTypeName(typeid(this).name());
	Render = new RenderComponent(MaterialData, MeshData);
};


void ModelData::Init(map<string, GLuint>& shaderPrograms) {

	TypeName = FormatTypeName(typeid(this).name());

	Render = new RenderComponent(MaterialData, MeshData);
	Render->Init(shaderPrograms);

}

