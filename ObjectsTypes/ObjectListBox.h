#pragma once

#include "./ObjectGUI.h"
#include "../CoreSettings.h"

//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/random.hpp>
//
//#include <fstream>

//using glm::vec4;
//using glm::vec3;
//using glm::vec2;
//using std::map;
//using std::string;
//using std::vector;
//using std::shared_ptr;
//using std::stringstream;

class ModelData;
class ObjectGUI;

class ObjectListBox : public ObjectGUI
{
public:
	ObjectListBox(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj,
		vec3 p_pos = vec3(0))
		: ObjectGUI(p_index,
			p_model,
			p_typeObj,
			p_pos) 
	{
	};


};

