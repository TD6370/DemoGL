#pragma once

#include "./ObjectGUI.h"
#include "../CoreSettings.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>
#include <sstream>
#include <ostream>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::map;
using std::string;
using std::vector;
using std::shared_ptr;
using std::stringstream;

class ModelData;

class ObjectCursorGUI :
	public ObjectGUI
{
private :

protected:
public:

	ObjectCursorGUI(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = TypeObject::TextBox,
		vec3 p_pos = vec3(0))
		: ObjectGUI(p_index,
			p_model,
			p_typeObj,
			p_pos) {

		IsAbsolutePosition = true;
		IsFocusable = false;
		IsTransformable = false;
		IsUsable = false;
		//TEST
		//IsFocusable = true;
		//IsUsable = true;
		ActionObjectCurrent = ActionObject::Moving;
	};

	~ObjectCursorGUI();

	/*void CreateMessage();
	void MeshTransform();*/

	void InitData();
	
	//void RunAction();
	//void virtual DefaultSate();
	//void virtual Click();

	void ActionMoving();
	void SaveNewPosition();
	void Work();
	//-------------

	//vector<ObjectFiledsSpecific> GetSpecificFiels();
	//void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);
};

