#include "..\ModelData.h"
#include "..\GeometryLib.h"
#include "../GeomertyShapes/ShapeBase.h"

#include "ObjectPolygon.h"



void ObjectPolygon::FillPlanes()
{
	Shape->FillPlanes();
}


void ObjectPolygon::InitData()
{
	ObjectPhysic::InitData();  
	ActionObjectCurrent = Lock;
}
