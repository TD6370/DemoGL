#pragma once

#ifndef ObjectData_H
#define ObjectData_H

#include "..\CoreSettings.h"
#include "..\ConfigUniformArg.h"
#include "..\TransformModel.h"
//#include "../GeomertyShapes/ShapeBase.h"

//#include "ObjectPhysic.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>
#include <map>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::linearRand;
using glm::mat4;
using std::string;
using std::shared_ptr;
using std::map;

class ShapeBase;
class Plane;
class ModelData;
class CreatorModelData;
struct ObjectFiledsSpecific;

class ObjectData
{
protected:
	
	GLfloat m_speedRotate = 0.1f;
	GLfloat m_nextWayTime = 0;
	GLfloat m_stepMove = 0;
	GLfloat m_angleModel = 0.0;
	std::string m_keyPosSectorStr;
public:
	string Name;
	TypeObject TypeObj;
	ActionObject ActionObjectCurrent;

	GLfloat Speed = 0.5f;
	int IndexObjectOwner = -1;
	bool IsSelected = false;

	// -----------  Options ---------------------
	bool IsGUI = false;
	bool IsNPC = false;
	bool IsHexagonModel = false;
	bool IsSquareModel = false;

	bool IsVisible = true;
	bool IsFocusable;
	bool IsTransformable;
	bool IsUsable;

	bool IsAbsolutePosition = true;
	bool IsTextureRepeat = false;
	bool IsGravity = false;
	//----------------------------------------
	
	CreatorModelData* Storage;
	ShapeBase* Shape;
	ColliseState CollisionPolygonState;
	
	int Index = 0;
	//int RadiusCollider = 5;

	shared_ptr<ModelData> ModelPtr;
	vector <vec3> Vertices;
	vector <vec3> Normals;
	vector <vec2> TextureUV;
	vector <vec3> Buffer;

	float TextureRepeat = 1;
	GLint TrianglesCount = 0;
	vec3 Color = vec3(0);

	//GLfloat TranslateAngle = 0;
	vec3 TranslateAngle = vec3(0);
	vec3 TargetAngle = vec3(0);
	vec3 Size = vec3(0);
	vec3 Postranslate = vec3(0);
	vec3 NewPostranslate = vec3(0);
	vec3 Move = vec3(0);
	vec3 Target = vec3(0);
	std::vector<int> tramsformTypes;
	//ActionObject ActionObjectCurrent;
	mat4 TransformResult;

	int minDist = 5;
	int DistanceTarget = 20;

	string ZonaPolygon;
	GLfloat PolygonPointY = 0;
	int PlaneDownIndex = -1;
	vec3 PlaneDownPosition = vec3(0);
	std::vector<vec3> TempVectors;
	//std::vector<shared_ptr<Plane>> Planes;		//-----(#3.))
	std::vector<string>  ChildObjects;

	ObjectData();

	ObjectData(int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = NPC,
		vec3 p_pos = vec3(0));

	~ObjectData();

	void virtual InitData();

	void virtual UpdateState(); //after fill options

	void GenStartPosition();

	void virtual CheckStartPosition();

	void virtual RunAction();

	void ActionBase();

	void RunTransform();

	void Action();

	//---------------
	std::shared_ptr<Plane> GetPlaneFromVertIndex(int indexVertPlane);

	std::shared_ptr<Plane> GetPlanePrt(int indexPlane);

	void virtual SelectedEvent();
	void virtual UnselectedEvent();

	std::vector< glm::vec3 > virtual GetVertices();
	std::vector< glm::vec2 > virtual GetUV();
	std::vector< glm::vec3 > virtual GetNormals();

	void virtual SetMesh();

	void UpdateTextureUV();
	
	void virtual ControlsEvents();

	bool virtual GetVisible();

	void virtual InitChildObjects();

	void AddChild(string key);
		
	std::shared_ptr<ObjectData> GetChild(string key);
	
	string GetInfo();

	//-----------------------------
	
	void virtual MeshTransform(); //------- geometric ???

	//-------- SaveFieldSpecific

	vector<ObjectFiledsSpecific> virtual GetSpecificFiels();

	void virtual SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);

};

#endif




