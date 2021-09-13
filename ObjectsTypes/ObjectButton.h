#pragma once

#include "./ObjectGUI.h"
#include "../CoreSettings.h"

using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::map;
using std::string;
using std::vector;
using std::shared_ptr;
using std::stringstream;

struct AnimationParamGUI;
class ModelData;
struct ObjectFiledsSpecific;

class ObjectButton :
	public ObjectGUI
{
private:
protected:
public:
	bool IsToogleButon = false;
	//bool IsChecked = false;
	

	ObjectButton(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		//TypeObject p_typeObj = TypeObject::Button,
		TypeObject p_typeObj,
		vec3 p_pos = vec3(0))
		: ObjectGUI(p_index,
			p_model,
			p_typeObj,
			p_pos) {
	};

	~ObjectButton();

	void InitData();

	void ActionWork();

	void RunAction();

	void DefaultView();

	void Click();
	//-------------

	vector<ObjectFiledsSpecific> GetSpecificFiels();
	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);
};

