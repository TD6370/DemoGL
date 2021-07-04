#pragma once

#include "../ModelData.h"
#include "./ObjectData.h"
#include "./ObjectGUI.h"
#include "../CoreSettings.h"
#include "../CreatorModelData.h"


using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::map;
using std::string;
using std::vector;
using std::shared_ptr;
using std::stringstream;

struct AnimationParamGUI;

class ObjectButton :
	public ObjectGUI
{
private:



protected:
public:
	bool IsToogleButon = false;
	bool IsToogleButonOn = false;
	

	ObjectButton(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = TypeObject::Button,
		vec3 p_pos = vec3(0))
		: ObjectGUI(p_index,
			p_model,
			p_typeObj,
			p_pos) {

		/*IsAbsolutePosition = false;
		IsFocusable = true;
		IsTransformable = true;
		IsUsable = true;
		IsSquareModel = true;*/
		//ActionObjectCurrent = ActionObject::Stay;
		//ActionObjectCurrent = ActionObject::Moving;

		/*IsGUI = true;
		IsFocusable = true;
		IsTransformable = true;
		IsUsable = true;
		IsAbsolutePosition = false;
		IsSquareModel = true;
		ActionObjectCurrent = ActionObject::Stay;*/
	};

	~ObjectButton();

	void InitData();

	void Work();

	void RunAction();

	void UpdateState();

	void DefaultColor();

	void Click();

	//ParamCaseDefault = -1;
	//-------------

	vector<ObjectFiledsSpecific> GetSpecificFiels();
	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);
};

