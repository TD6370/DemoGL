#pragma once

#include "ObjectDynamic.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

using glm::vec4;
using glm::vec3;
using glm::vec2;


class ObjectBullet :
	public ObjectDynamic
{
public:
	GLfloat StartLenght = 5.f;
	GLfloat EndLenght = 100.f;
	int KeyPush = GLFW_MOUSE_BUTTON_1;

	ObjectBullet(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj,
		vec3 p_pos = vec3(0))
		: ObjectDynamic(p_index,
			p_model,
			p_typeObj,
			p_pos) {};

	void InitData();

	void RunAction();

	bool CheckIsLock();

	void TargetCompleted();

	void Click();
};