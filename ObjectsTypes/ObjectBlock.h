
#pragma once

#include "ObjectPhysic.h"

using std::string;

struct BlockFileds {
	string Vertex = "Vertex:";
	string UV = "UV:";
};


class ObjectBlock :
	public ObjectPhysic
{
public:
	int KeyUpTopVertex = GLFW_KEY_KP_ADD;
	int KeyDownTopVertex = GLFW_KEY_KP_SUBTRACT;
	float StartLenghtWall = 1;
			
	ObjectBlock(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj,
		vec3 p_pos = vec3(0))
		: ObjectPhysic(p_index,
			p_model,
			p_typeObj,
			p_pos) {
		IsHexagonModel = true;
		IsSquareModel = true;
		IsTransformable = true;
	};

	void InitData();
	void LockPolygonResult();
	void LockResult();

	void RunAction();
	
	void TestGravity();

	void SelectedEvent();

	void SaveNewPosition();

	void MeshTransform();

	void ControlsEvents();

	//----------------------

	vector<ObjectFiledsSpecific> GetSpecificFiels();

	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);
};
