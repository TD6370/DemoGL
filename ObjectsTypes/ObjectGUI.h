#pragma once
#include "ObjectData.h"
#include "ObjectPhysic.h"

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
	//public ObjectData
{

public:
	vec3 StartPos = vec3(0);
	vec2 Size = vec2(0);

	ObjectGUI(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = NPC,
		vec3 p_pos = vec3(0))
		: ObjectPhysic(p_index,
			p_model,
			p_typeObj,
			p_pos) {};

	void InitData();

	void RunAction();

	void UpdateState();

	bool IsShow();

	void ConfigInterface(string caption, string nameModel, string nameObject, vec3 position, vec2 size, vec3 color = vec3(0));

	bool IsCubeModel();

	void SetSizeControl(vec3 size);

	//#FieldSpecific

	vector<ObjectFiledsSpecific> virtual GetSpecificFiels();

	void virtual SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);
};