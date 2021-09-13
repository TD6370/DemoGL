#pragma once

#include "../CoreSettings.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

class ObjectData;
//class ObjectGUI;
struct ObjectFiledsSpecific;

class ButtonComponent
{
private:
	ObjectData* m_obj;

	vec3 m_color_work = vec3(0, 1, 0); //green

public:
	bool IsToogleButon = false;

	ButtonComponent() : m_obj(nullptr) {}

	//void InitData();

	void Init(ObjectData* obj);

	void ActionWork();

	void DefaultView();

	/*

		

		void RunAction();

		void DefaultView();

		void Click();

		void Refresh();*/

	//-------------

	vector<ObjectFiledsSpecific> GetSpecificFiels();
	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);

};

