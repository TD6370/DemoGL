
#pragma once

#include "ObjectPhysic.h"
//
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/random.hpp>
//
//#include <fstream>
//
//using glm::vec4;
//using glm::vec3;
//using glm::vec2;

class ObjectBlock :
	public ObjectPhysic
{
public:

	//std::vector<vec3> BottomVectors;
	//std::vector<vec3> TopVectors;
	std::map<int, vec3> TopVectors;
	std::map<int, vec3> BottomVectors;
	//std::vector<vec3> CubeVectors;
	
	ObjectBlock(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = NPC,
		vec3 p_pos = vec3(0))
		: ObjectPhysic(p_index,
			p_model,
			p_typeObj,
			p_pos) {};

	void InitData();
	void LockPolygonResult();
	void LockResult();

	void RunAction();
	void TestGravity();
	void EventChange();

	std::vector< glm::vec3 > GetVertices();

	void SetMesh();

	void SaveNewPosition();

	void MeshTransform();

	void FillPlanes();

	vec3 GetBottom(int index);

	vec3 GetTop(int index);

	void SetBottom(int index, vec3 value);

	void SetTop(int index, vec3 value);
};
