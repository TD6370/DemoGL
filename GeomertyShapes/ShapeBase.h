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

	std::vector<shared_ptr<Plane>> Planes;

	ShapeBase();

	~ShapeBase();

	vec3 GetVertexWorldPosition(ObjectData* obj, int indVertex);

	vec3 ToWorldPosition(ObjectData* obj, vec3 pos);

	string GetKeySectorPolygon(ObjectData* obj, bool isNewPosition);

	void FillPlanes(ObjectData* obj);

	void FillVertextBox(ObjectPhysic* obj);
};