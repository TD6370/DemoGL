#pragma once

#include "../CoreSettings.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

class ObjectData;
//class ObjectGUI;
struct ObjectFiledsSpecific;


class GUIComponent
{
private:
	ObjectData* m_obj;

	vec3 m_color_work = vec3(0, 1, 0); //green

protected:

	vec3 m_color_transforming = vec3(0.1, 0.8, 0.1); //green
	string hyst_posit = "";
	vec3 tmp_posit = vec3();
	vec3 start_vertBottomLeft = vec3(0);
	vec3 start_vertBottomRight = vec3(0);
	vec3 start_vertTopLeft = vec3(0);

public:

	GUIComponent() : m_obj(nullptr) {}

	float PanelDepth;

    void Init(ObjectData* obj);

	void RunAction();

	bool GetVisible();

	//void Click();

	void ActionWork();

	void ActionMoving();

	float GetTopLayer();

	float GetZ();

	void SetZ(float z);

	string GetInfo();

	vector<ObjectFiledsSpecific> GetSpecificFiels();
	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);
};

