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
protected:
	void FillPlanesCube();
	
public:
	std::map<int, vec3> TopVectors;
	std::map<int, vec3> BottomVectors;

	int IndexVertexTransform = -1;

	ObjectPhysic(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = NPC,
		vec3 p_pos = vec3(0))
		: ObjectData(p_index,
			p_model,
			p_typeObj,
			p_pos) {
		IsCubeModel = false;
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
	void virtual CalculateNextPosition();
	void virtual SaveNewPosition();

	//void TestGravity();
	void SelectedEvent();
	void UnselectedEvent();

	void SaveToCluster();

	void virtual FillPlanes();
	

	bool GetVisible();

	//-----------

	std::vector< glm::vec3 > GetVertices();

	void SetMesh();

	vec3 GetBottom(int index);
	vec3 GetBottomFirst();
	vec3 GetBottomLast();

	vec3 GetTop(int index);
	vec3 GetTopFirst();
	vec3 GetTopLast();

	void SetBottom(int index, vec3 value);

	void SetTop(int index, vec3 value);

	vec4 GetLine(int index);

	float GetLineLenght(int index);

	void virtual GetPositRect(vec2& startPos, vec2& endPos, float& zOrder);
	//----------

	//bool virtual IsCubeModel();

};

