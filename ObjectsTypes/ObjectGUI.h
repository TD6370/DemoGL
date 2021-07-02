#pragma once
#include "ObjectData.h"
#include "ObjectPhysic.h"
//#include "ObjectTextBlock.h"
#include "../CoreSettings.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::map;
using std::string;
using std::shared_ptr;

class ObjectGUI :
	public ObjectPhysic
{
protected :
	vec3 m_color_work = vec3(0, 1, 0); //green
	vec3 m_color_transforming = vec3(0.1, 0.8, 0.1); //green
	string hyst_posit = "";
	vec3 tmp_posit = vec3();
	vec3 start_vertBottomLeft = vec3(0);
	vec3 start_vertBottomRight = vec3(0);
	vec3 start_vertTopLeft = vec3(0);

public:
	
	vec3 StartPos = vec3(0);
	vec2 SizePanel = vec2(0);
	float PanelDepth;
	AnimationParamGUI* AnimationParam;

	ObjectGUI(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = TypeObject::GUI,
		vec3 p_pos = vec3(0))
		: ObjectPhysic(p_index,
			p_model,
			p_typeObj,
			p_pos) 	{
		IsGUI = true;
		IsFocusable = true;
		IsTransformable = true;
		IsUsable = true;
		IsAbsolutePosition = false;
		IsSquareModel = true;
		ActionObjectCurrent = ActionObject::Stay;
	};

	void InitData();

	void SetDataToShader(bool isUpdate = false);

	void RunAction();

	bool GetVisible();

	shared_ptr<ObjectData> ConfigInterface(string caption, string nameModel, string nameObject, vec3 position, vec2 size, TypeObject p_typeObj = TypeObject::GUI, vec3 color = vec3(0));

	void ControlConstruct(shared_ptr<ObjectGUI> obj, string caption);

	void virtual Click();
	
	void virtual Work();

	void virtual DefaultSate();

	void ActionMoving();

	//-------------

	vector<ObjectFiledsSpecific> GetSpecificFiels();

	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);

	void virtual SizeControlUpdate();

};