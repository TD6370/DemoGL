
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

	int KeyUpTopVertex = GLFW_KEY_KP_ADD;
	int KeyDownTopVertex = GLFW_KEY_KP_SUBTRACT;

	//std::map<int, vec3> TopVectors;
	//std::map<int, vec3> BottomVectors;
	//int IndexVertexTransform = -1;

	float StartLenghtWall = 1;
			
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

	void SelectedEvent();

	void SaveNewPosition();

	void MeshTransform();

	//------------------
	void SelectVertexBlock();
	
	bool IsCubeModel();

	//----------------------
		
	void UpdateTextureUV();

	void CalculateTextureUV(bool isInit = false);

	void ControlsEvents();
};
