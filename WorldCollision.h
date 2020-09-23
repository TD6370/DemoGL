#pragma once

#ifndef WorldCollision_H
#define WorldCollision_H

#include "CoreSettings.h"
#include "CreatorModelData.h"  //------<<<

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

class Plane;

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
	
	WorldCluster();
	
	void PlaneClusterization();

	void SaveClusterObject(int indexObj);

	vector<int> GetSectorObjects(int indexObj, bool isNewPosition);

	vector<int> GetVertexPolygonFromObject(int indexObj, vector<string>& checkedZona);

	vector<int> GetIndexPlanePolygonFromObject(int indexObj, vector<string>& checkedZona);

	bool IsCollisionCircle(int indObjMe, int indObj2, bool isNewPosition = false);

	bool IsCollisionObject(int indexObjMe, bool isNewPosition = false);
	
	//std::vector<glm::vec3> GetPosVertextPolygon(int ind, int x_sector, int z_sector);

	ColliseState IsCollisionPolygon(int indexObj, Plane* plane, vec4& vertex);

	ColliseState IsCollisionPolygonP2(int indexObj, Plane* plane, vec4& vertex);
};

#endif

