#pragma once

#include "ObjectDynamic.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

using glm::vec4;
using glm::vec3;
using glm::vec2;


class ObjectCursorRay :
	public ObjectDynamic
{
private:
	ActionObject m_preSelectedObjectAction;

public:
	GLfloat StartLenght = 0.1f;
	GLfloat EndLenght = 200.f;
	int KeyPush = GLFW_MOUSE_BUTTON_2;
	int KeyClear = GLFW_MOUSE_BUTTON_1;

	int SelectedObjIndex = -1;
	int PrevousSelectedObjIndex = -1;

	ObjectCursorRay(
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

	void LockPolygonResult();

	void LockResult();

	void TargetCompleted();

	void Click();

	void ObjectSelected(int index);

	void ClearSelected();
	
	void ClearPrevousSelected();

	void SelectPositionOnPolygon();

	void CusrsorAction();

	void SelectedCompleted();
};