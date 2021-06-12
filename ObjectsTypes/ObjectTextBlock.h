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
using std::map;
using std::string;
using std::vector;
using std::shared_ptr;


class ObjectTextBlock :
	public ObjectGUI
{
protected:
	map<string, int> mapAlphabet;
	vector<int> MessageCode;

public:
	string Message = std::string();

	ObjectTextBlock(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = TypeObject::TextBlock,
		vec3 p_pos = vec3(0))
		: ObjectGUI(p_index,
			p_model,
			p_typeObj,
			p_pos) {
		
		IsAbsolutePosition = true;
		//IsFocused = false;

		//--- TEST
		IsFocused = true;
	};


	~ObjectTextBlock();

	void InitData();

	void RunAction();

	void UpdateState();

	void CreateMessage();

	void MeshTransform();

	bool IsCubeModel();

	void FillPlanes();

	//void ResizeTextureUV();

	//void virtual Click();

	//void virtual Work();

	//void virtual DefaultSate();

	//-------------

	//vector<ObjectFiledsSpecific> GetSpecificFiels();

	//void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);
};
