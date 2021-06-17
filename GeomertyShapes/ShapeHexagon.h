#pragma once
#include "ShapeBase.h"
#include "../ObjectsTypes/ObjectPhysic.h"

//#include "../CoreSettings.h"
//#include "../ModelData.h"
//#include "../GeometryLib.h"
//#include "../ObjectsTypes/ObjectData.h"
//#include "../CreatorModelData.h"
////#include "../WorldCluster.h"
////----
//#include "../WorldCollision.h" //--<< #include "CreatorModelData.h"  
////#include "../TransformModel.h"
////#include "..\ConfigUniformArg.h"
//#include "../ObjectsTypes/ObjectPhysic.h"
//
//class ObjectPhysic;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

class ShapeHexagon : public ShapeBase
{
private:

public:
	float StartLenghtWall = 1;

	ShapeHexagon() :ShapeBase() {};

	~ShapeHexagon();
	
	void SelectVertexBlock(ObjectPhysic* obj);

	void ResizeTextureUV(ObjectPhysic* obj);

	void ResizeVerticaleWall(ObjectPhysic* obj, int keyUpTopVertex, int keyDownTopVertex);

	void CalculateTextureUV(ObjectPhysic* obj, bool isInit);

};

