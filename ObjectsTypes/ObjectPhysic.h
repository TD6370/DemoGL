#pragma once
#include "ObjectData.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

using glm::vec4;
using glm::vec3;
using glm::vec2;

class ShapeHexagon;
class ShapeSquare;

class ObjectPhysic :
	public ObjectData
{
	
public:

	int IndexVertexTransform = -1;

	ObjectPhysic(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj,
		vec3 p_pos = vec3(0))
		: ObjectData(p_index,
			p_model,
			p_typeObj,
			p_pos) {
		IsSquareModel = false;

		
	};

	void InitData();

	bool IsContactWorldBorder(vec3 pos);

	bool virtual CheckIsLock();

	void RunAction();

	void CheckStartPosition();

	void virtual LockResult();

	void virtual LockPolygonResult();

	bool CheckIsPolygon();

	bool IsCollisionObject(int index, int& indexObjHit, bool isNewPosition);

	void virtual ActionMoving(); 
	void virtual ActionTransforming();
	void virtual CalculateNextPosition();
	void virtual SaveNewPosition();

	//void TestGravity();
	void SelectedEvent();
	void UnselectedEvent();

	void SaveToCluster();

	void virtual FillPlanes();

	bool GetVisible();

	//-----------
	/*ShapeHexagon* GetShapeHexagon();

	ShapeSquare* GetShapeSquare();*/

	std::vector< glm::vec3 > GetVertices();

	void SetMesh();
	
};

