#pragma once


//#include "Controllers.h"	//##$$
//#include "ConfigBuffers.h" //##$$
//#include "ConfigUniformArg.h" //##$$ 3.

#include "CoreSettings.h"
//#include "ConfigUniformArg.cpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

vec3 GetVectorForwardFace(CoreMVP* ConfigMVP, GLfloat lenght, Operator* operatorG);

vec3 GetVectorForwardFaceOffset(CoreMVP* ConfigMVP, GLfloat lenght, Operator* operatorG, vec3 posOffset);

vec3 GetVectorForward(CoreMVP* ConfigMVP, GLfloat lenght, Operator* operatorG);

glm::mat4 Transform(unsigned int index, int paramCase, bool IsNoTranf,
	glm::mat4 trans,
	glm::vec3 p_postranslate,
	glm::vec3 p_angle,
	glm::vec3 p_size);


class TransformModel {

public:
	
	void GenMVP(
		int m_widthWindow,
		int m_heightWindow,
		Operator* p_operator,
		Camera* p_camera,
		CoreMVP* ConfigMVP);

};

