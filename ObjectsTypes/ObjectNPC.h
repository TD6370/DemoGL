#pragma once

#include "ObjectDynamic.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

using glm::vec4;
using glm::vec3;
using glm::vec2;


class ObjectNPC :
	public ObjectDynamic
{
public:

	ObjectNPC(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = NPC,
		vec3 p_pos = vec3(0))
		: ObjectDynamic(p_index,
			p_model,
			p_typeObj,
			p_pos) {};

	void InitData();

	//void LockObjectResult();

	//void LockPolygonResult();

	void Pathfinding();

	bool CalculateTatget(vec3& resultTarget);

	//void CalculateNextPosition();
};