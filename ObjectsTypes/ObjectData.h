#pragma once

#ifndef ObjectData_H
#define ObjectData_H

#include "..\CoreSettings.h"
#include "..\ConfigUniformArg.h"
#include "..\TransformModel.h"

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
	GLfloat Speed = 0.5f;
	int IndexObjectOwner = -1;
	bool IsSelected = false;

	//Options
	bool IsVisible = true;
	bool IsGravity = false;
	bool IsGUI = false;
	bool IsTextureRepeat = false;
	bool IsNPC = false;
	bool IsBlock = false;
	bool IsCubeModel = false;
	bool IsAbsolutePosition = true;
	bool IsFocusable;
	bool IsTransformable;
	bool IsUsable;
	
	CreatorModelData* Storage;

	TypeObject TypeObj;
	ColliseState CollisionPolygonState;
	std::string Name;
	ActionObject ActionObjectCurrent;

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
	std::vector<shared_ptr<Plane>> Planes;
	std::vector<string>  ChildObjects;

	ObjectData();

	ObjectData(int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = NPC,
		vec3 p_pos = vec3(0));

	~ObjectData();

	void virtual InitData();

	void GenStartPosition();

	void virtual CheckStartPosition();

	void virtual RunAction();

	void ActionBase();

	void RunTransform();

	void Action();

	//---------------
	
	vec3 GetVertexPosition(int indVertex); //------- geometric ???

	vec3 ToWorldPosition(vec3 pos); //------- geometric ???

	string GetKeySectorPolygon(bool isNewPosition = true); //------- geometric ???

	std::shared_ptr<Plane> GetPlaneFromVertIndex(int indexVertPlane); //------- geometric ???

	std::shared_ptr<Plane> GetPlanePrt(int indexPlane); //------- geometric ???

	void virtual SelectedEvent();
	void virtual UnselectedEvent();

	std::vector< glm::vec3 > virtual GetVertices();
	std::vector< glm::vec2 > virtual GetUV();
	std::vector< glm::vec3 > virtual GetNormals();

	void virtual SetMesh();

	void virtual MeshTransform(); //------- geometric ???

	void virtual ControlsEvents();

	void virtual Billboard(); //------- geometric ???

	bool virtual GetVisible();

	void virtual InitChildObjects();

	void AddChild(string key);
		
	std::shared_ptr<ObjectData> GetChild(string key);
	
	

	void UpdateTextureUV(); //------- geometric ???

	//-------- SaveFieldSpecific

	vector<ObjectFiledsSpecific> virtual GetSpecificFiels();

	void virtual SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);

	// ---- geometric Box

	//void UpdateState();
	//bool IsCubeModel();
	//void FillPlanes(); //TEST
	//void ResizeTextureUV();
	//---------------------

};

#endif




