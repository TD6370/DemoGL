#pragma once

#include "../CoreSettings.h"
#include "../ModelData.h"
#include "../GeometryLib.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../CreatorModelData.h"
//#include "../WorldCluster.h"
//----
#include "../WorldCollision.h" //--<< #include "CreatorModelData.h"  
//#include "../TransformModel.h"
//#include "..\ConfigUniformArg.h"
#include "../ObjectsTypes/ObjectPhysic.h"

//#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>
#include <map>

using glm::vec4;
using glm::vec3;
using glm::vec2;
//using glm::linearRand;
using glm::mat4;
using std::string;
using std::shared_ptr;
using std::map;


class Plane;
//class ModelData;
class ObjectData;
class ObjectPhysic;

class ShapeBase
{
public:
public:
	std::map<int, vec3> TopVectors;
	std::map<int, vec3> BottomVectors;
	std::vector<shared_ptr<Plane>> Planes;
	//ObjectPhysic* m_obj;
	ObjectData* m_obj;
	ObjectPhysic* m_objPhysic;
	bool IsContainObject = true;

	ShapeBase();

	~ShapeBase();

	void UpdateShapeInfo(ObjectPhysic* m_obj);
	void UpdateShapeInfo(ObjectData* m_obj);

	//--------------------------------------

	vec3 GetVertexWorldPosition(int indVertex);

	vec3 ToWorldPosition(vec3 pos);

	string GetKeySectorPolygon(bool isNewPosition);

	void FillPlanes();

	void FillVertextBox();

	//===========

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

	void GetPositRect(vec2& startPos, vec2& endPos, float& zOrder);

	vec2 GetStartPositWorld();

	void Billboard();
};
