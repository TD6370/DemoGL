#pragma once
#include "ObjectPhysic.h"
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


struct ObjectFiledsSpecific;
class ObjectData;

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
	AnimationParamGUI* AnimationParam; //TODO: in static param

	ObjectGUI(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj,
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
		Layer = TypeLayer::LayerBase;
	};

	void InitData();

	void SetDataToShader();

	void RunAction();

	bool GetVisible();

	//void virtual Click();
	void Click();
	
	//void virtual Work();
	void ActionWork();


	void ActionMoving();

	void DefaultView();

	float GetTopLayer();

	float GetZ();

	void SetZ(float z);

	string GetInfo();
	//-------------

	vector<ObjectFiledsSpecific> GetSpecificFiels();

	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);

	void virtual SizeControlUpdate();

};