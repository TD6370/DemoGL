#pragma once

#include "ConfigBuffers.h"
#include "ConfigUniformArg.h"
#include "CoreSettings.h"
//#include "ConfigUniformArg.cpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

//glm::mat4 Transform(GLuint shaderProgram, unsigned int index, int paramCase, ConfigUniform confUniform, bool IsNoTranf,
//	glm::mat4 trans,
//	glm::vec3 p_postranslate,
//	GLfloat	 p_angle);
glm::mat4 Transform(unsigned int index, int paramCase, bool IsNoTranf,
	glm::mat4 trans,
	glm::vec3 p_postranslate,
	GLfloat	 p_angle);

/*
void GenMVP(GLuint shaderProgram,
	ConfigUniform confUniform,
	int m_widthWindow,
	int m_heightWindow,
	Operator* p_operator,
	Camera p_camera);
	*/
void GenMVP(
	int m_widthWindow,
	int m_heightWindow,
	Operator* p_operator,
	Camera* p_camera,
	CoreMVP* ConfigMVP);

glm::vec3 GetVectorForwardFace(CoreMVP* ConfigMVP, GLfloat lenght, Operator* operatorG);

glm::vec3 GetVectorForward(CoreMVP* ConfigMVP, GLfloat lenght, Operator* operatorG);

//enum TramsformType { RotateX, RotateY, RotateZ };
//enum TramsformType;


