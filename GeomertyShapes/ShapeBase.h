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
	ObjectPhysic* m_obj;

	ShapeBase();

	~ShapeBase();

	vec3 GetVertexWorldPosition(ObjectData* obj, int indVertex);

	vec3 ToWorldPosition(ObjectData* obj, vec3 pos);

	string GetKeySectorPolygon(ObjectData* obj, bool isNewPosition);

	void FillPlanes(ObjectData* obj);

	void FillVertextBox(ObjectPhysic* obj);

	//===========

	vec3 GetBottom(int index);
	vec3 GetBottomFirst();
	vec3 GetBottomLast();

	vec3 GetTop(int index);
	vec3 GetTopFirst();
	vec3 GetTopLast();

	void SetBottom(ObjectPhysic* obj, int index, vec3 value);

	void SetTop(ObjectPhysic* obj, int index, vec3 value);

	vec4 GetLine(ObjectPhysic* obj, int index);

	float GetLineLenght(ObjectPhysic* obj, int index);

	void GetPositRect(ObjectPhysic* obj, vec2& startPos, vec2& endPos, float& zOrder);

	vec2 GetStartPositWorld(ObjectPhysic* obj);

	void Billboard(ObjectPhysic* obj);
};