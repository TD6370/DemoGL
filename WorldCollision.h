#pragma once

#ifndef WorldCollision_H
#define WorldCollision_H

#include "CoreSettings.h"
#include "CreatorModelData.h"  //------<<<
#include "./GeomertyShapes/ShapeBase.h"

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

class Plane;
class ShapeBase;

using std::string;
using std::vector;
using std::map;
using glm::vec4;

class WorldCluster
{
public:
	int SectorSizePlane = 5; // 5;// 10;// 100;
	int SectorSize = 10;// 100;
	CreatorModelData* Storage;
	WorldSectors* Sectors;
	ShapeBase* Shape;
	
	WorldCluster();
	
	void PlaneClusterization();

	void SaveClusterObject(int indexObj);

	void SaveClusterDynamicColiderObject(int indexObj);

	void SaveClusterBlockObject(int indexObj);

	vector<int> GetSectorObjects(int indexObj, bool isNewPosition, TypeObject typeObj);

	vector<int> GetVertexPolygonFromObject(int indexObj, vector<string>& checkedZona);

	vector<int> GetIndexPlanePolygonFromObject(int indexObj, vector<string>& checkedZona);

	bool IsCollisionCircle(int indObjMe, int indObj2, bool isNewPosition = false);

	bool IsCollisionLineCircle(float x1, float y1, float x2, float y2,
		float xC, float yC, float R);

	bool IsCollisionObject(int indexObjMe, int& indexObjHit, bool isNewPosition = false);

	bool IsCollisionDynamicObject(int indexObjMe, int& indexObjHit, bool isNewPosition = false);

	bool IsCollisionBlocks(int indexObjMe, int& indexObjHit, bool isNewPosition = false);

	bool IsCollisionObjectToBlock(int indObjMe, int indBlock, bool isNewPosition);
	
	
	//std::vector<glm::vec3> GetPosVertextPolygon(int ind, int x_sector, int z_sector);

	ColliseState IsCollisionPolygon(int indexObj, Plane* plane, vec4& vertex);

	ColliseState IsCollisionPolygonP2(int indexObj, Plane* plane, vec4& vertex);
};

#endif

