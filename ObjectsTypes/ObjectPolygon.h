#pragma once

#include "ObjectPhysic.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

using glm::vec4;
using glm::vec3;
using glm::vec2;


class ObjectPolygon :
	public ObjectPhysic
{
public:

	ObjectPolygon(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj,
		vec3 p_pos = vec3(0))
		: ObjectPhysic(p_index,
			p_model,
			p_typeObj,
			p_pos) {};


	void FillPlanes();

	void InitData();
};
