#pragma once

#include "ObjectData.h"
#include "ObjectGUI.h"
#include "ObjectPhysic.h"
#include "../CoreSettings.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

using glm::vec4;
using glm::vec3;
using glm::vec2;
//using std::map;
using std::string;
using std::shared_ptr;


class ObjectTextBlock :
	public ObjectGUI
{

public:
	string Message = std::string();

	//ObjectTextBlock() {};

	ObjectTextBlock(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = TypeObject::TextBlock,
		vec3 p_pos = vec3(0))
		: ObjectGUI(p_index,
			p_model,
			p_typeObj,
			p_pos) {
		IsFocused = false;
	};


	~ObjectTextBlock();

	void InitData();

	void RunAction();

	void UpdateState();

	void SetSizeControl(vec3 size);

	void GetPositRect(vec2& startPos, vec2& endPos, float& zOrder);

	//void ResizeTextureUV();

	//void virtual Click();

	//void virtual Work();

	//void virtual DefaultSate();

	//-------------

	//vector<ObjectFiledsSpecific> GetSpecificFiels();

	//void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);
};
