#pragma once

#include "../ModelData.h"
#include "./ObjectData.h"
#include "./ObjectGUI.h"
#include "./ObjectPhysic.h"
#include "../CoreSettings.h"
#include "../CreatorModelData.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::map;
using std::string;
using std::vector;
using std::shared_ptr;

class ObjectCursorGUI :
	public ObjectGUI
{
protected:
public:

	ObjectCursorGUI(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = TypeObject::TextBlock,
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
		
		//ActionObjectCurrent = ActionObject::Woking;
	};

	~ObjectCursorGUI();

	/*void CreateMessage();
	void MeshTransform();*/

	void InitData();
	
	//void RunAction();
	//void virtual DefaultSate();
	//void virtual Click();

	void ActionMoving();
	void CalculateNextPosition();
	void SaveNewPosition();

	void Work();

	//-------------

	//vector<ObjectFiledsSpecific> GetSpecificFiels();
	//void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);
};

