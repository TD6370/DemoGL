#pragma once
#include "ObjectData.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

using glm::vec4;
using glm::vec3;
using glm::vec2;


class ObjectPhysic :
	public ObjectData
{

public:

	ObjectPhysic(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = NPC,
		vec3 p_pos = vec3(0))
		: ObjectData(p_index,
			p_model,
			p_typeObj,
			p_pos) {};

	bool IsContactWorldBorder(vec3 pos);

	bool virtual CheckIsLock();

	void RunAction();

	void CheckStartPosition();

	void virtual LockResult();

	void virtual LockPolygonResult();

	bool CheckIsPolygon();

	bool IsCollisionObject(int index, int& indexObjHit, bool isNewPosition);

	void SaveClusterObject(int index);
};

