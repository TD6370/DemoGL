#pragma once

#ifndef ObjectData_H
#define ObjectData_H

#include "CoreSettings.h"
#include "ConfigUniformArg.h"
#include "TransformModel.h"

class ModelData;
//class ObjectData;
class CreatorModelData;

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::linearRand;
using std::string;
using std::shared_ptr;

class Plane;

class ObjectData
{
private:
	
	GLfloat m_speedRotate = 0.1f;
	GLfloat m_nextWayTime = 0;
	GLfloat m_stepMove = 0;
	GLfloat m_angleModel = 0.0;
	std::string m_keyPosSectorStr;
public:
	GLfloat Speed = 0.5f;

	CreatorModelData* Storage;

	TypeObject TypeObj;
	ColliseState CollisionPolygonState;
	std::string Name;

	int Index = 0;
	//int RadiusCollider = 5;

	std::shared_ptr<ModelData> ModelPtr;

	vec3 Color = vec3(0);

	GLfloat TranslateAngle = 0;
	GLfloat TargetAngle = 0;
	vec3 Postranslate = vec3(0);
	vec3 NewPostranslate = vec3(0);
	vec3 Move = vec3(0);
	vec3 Target = vec3(0);
	std::vector<int> tramsformTypes;
	ActionObject ActionObjectCurrent;


	int minDist = 5;
	int DistanceTarget = 20;
	//int DistanceTarget = 10;

	string ZonaPolygon;
	GLfloat PolygonPointY = 0;
	int PlaneDownIndex = -1;
	vec3 PlaneDownPosition = vec3(0);
	std::vector<vec3> TempVectors;
	//std::vector<Plane>* Planes;
	std::vector<shared_ptr<Plane>> Planes;
	//shared_ptr<ObjectData>

	ObjectData(int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = NPC,
		vec3 p_pos = vec3(0));
	
	~ObjectData();

	void GenStartPosition();

	void ChekNextWay();

	bool IsContactWorldBorder(vec3 pos);

	void ActionMoving();
	
	void ActionLook();

	void ActionSearch();

	void RunAction();

	bool CheckIsLock();

	bool CheckIsPolygon();

	void RunTransform();

	//void Action(Camera* camera, CoreMVP* p_mvp);
	void Action();
	
	vec3 GetVertexPosition(int indVertex);

	vec3 ToWorldPosition(vec3 pos);
	//bool IsCheckLock();

	string GetKeySectorPolygon(bool isNewPosition = true);


	//-------------

	void FillPlanes();

	//Plane* GetPlaneFromVertIndex(int indexVertPlane);
	std::shared_ptr<Plane> GetPlaneFromVertIndex(int indexVertPlane);

	//Plane* GetPlanePrt(int indexPlane);
	std::shared_ptr<Plane> GetPlanePrt(int indexPlane);
};

#endif




