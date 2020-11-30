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

class ObjectGUI :
	public ObjectPhysic
	//public ObjectData
{

public:
	vec3 StartPos = vec3(0);
	//vec2 SizeControl = vec2(.1f,1.f);

	/*ObjectGUI(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = NPC,
		vec3 p_pos = vec3(0))
		: ObjectData(p_index,
			p_model,
			p_typeObj,
			p_pos) {};*/

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

	void ConfigInterface(string caption, string nameModel, string nameObject, vec3 position, vec2 size);

	bool IsCubeModel();

	void MeshTransform(vec3 size);
};