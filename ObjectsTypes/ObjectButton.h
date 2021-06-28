#pragma once

#include "../ModelData.h"
#include "./ObjectData.h"
#include "./ObjectGUI.h"
//#include "./ObjectPhysic.h"
#include "../CoreSettings.h"
#include "../CreatorModelData.h"

//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/random.hpp>

//#include <fstream>
//#include <sstream>
//#include <ostream>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::map;
using std::string;
using std::vector;
using std::shared_ptr;
using std::stringstream;

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

	void Work();

	//-------------

	vector<ObjectFiledsSpecific> GetSpecificFiels();
	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);
};
