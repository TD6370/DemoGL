//@@@
#include "ModelData.h"
//#include "BindHeader.h"

#include "ConfigBuffers.h"
#include "Shaders.h"
#include "ConfigUniformArg.h"
#include "LoaderModelObj.h"
#include "LoadBmp.h"
#include "GeometryLib.h"

#include <string>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

//OpenGL Mathematics
#include <glm/glm.hpp>

using std::vector;
using glm::vec3;

ModelData::ModelData() {

}

void ModelData::Init() {
	//Load vertex shader
	GLuint vertShader = GenShader(PathShaderVertex, true);

	//Load fragment shader
	GLuint fragShader = GenShader(PathShaderFrag, false);
	//----------------------------------------------------
	ShaderProgram = ProgramConfig(vertShader, fragShader);

	//------------------------------- Load texture
	DataImage = LoadBmp(PathTexture, &WidthImage, &HeightImage);
	if (DataImage == NULL)
	{
		std::cerr << "Could not read image file " << PathTexture << ". File does not exist." << std::endl;
		return;
	}
	//---------------------------------------- Load mesh

	bool isGetIndices = false;
	bool result = false;
	result = LoadModelObj(PathModel3D,
		Vertices,
		UV,
		Normals,
		Indices,
		isGetIndices);

	if (!result)
	{
		fprintf(stderr, "Error load Model.obj");
	}
	TrianglesCount = Vertices.size();
	IndicesSize = Indices.size();
	//-----------------------------

	VAO = InitVAO();
	VBO = InitVBO();

	SetVAO();

	if (VAO == -1)
	{
		fprintf(stderr, "Error load VAO");
	}

	Texture_ID = InitImage();
	BufferUV_ID = InitUV();
	BufferNormal_ID = InitNormals();

	SetModelInBuffer();

	ConfUniform = ConfigUniform(ShaderProgram);

	//FillPlanes();
}

void ModelData::SetVAO() {
	GenVertexArrayObject(IsIndex,
		Vertices,
		Indices,
		VAO, VBO);
}

void ModelData::SetModelInBuffer(bool isUpdate)
{
	SetImage(DataImage, WidthImage, HeightImage, Texture_ID);
	SetBufferUV(UV, BufferUV_ID);
	SetNormals(Normals, BufferNormal_ID);
}

