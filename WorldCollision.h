#pragma once

#ifndef WorldCollision_H
#define WorldCollision_H

#include "CoreSettings.h"

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
class CreatorModelData;
class ObjectData;

using std::string;
using std::vector;
using std::map;
using glm::vec4;

class WorldCluster
{
public:
	//TODO: static !!!!
	int SectorSizePlane = 5; // 5;// 10;// 100;

	int SectorSize = 10;// 100;
	CreatorModelData* Storage;
	WorldSectors* Sectors;

	WorldCluster();
	
	void PlaneClusterization();

	void SaveClusterObject(int indexObj);
	void SaveClusterObject(ObjectData* obj);

	void SaveClusterDynamicColiderObject(int indexObj);
	void SaveClusterDynamicColiderObject(ObjectData* object);

	void SaveClusterBlockObject(int indexObj);
	void SaveClusterBlockObject(ObjectData* object);

	vector<int> GetSectorObjects(int indexObj, bool isNewPosition, TypeObject typeObj);
	vector<int> GetSectorObjects(ObjectData* object, bool isNewPosition, TypeObject typeObj);

	vector<int> GetVertexPolygonFromObject(int indexObj, vector<string>& checkedZona);
	
	vector<int> GetIndexPlanePolygonFromObject(int indexObj, vector<string>& checkedZona);
	vector<int> GetIndexPlanePolygonFromObject(ObjectData* obj, vector<string>& checkedZona);

	bool IsCollisionCircle(int indObjMe, int indObj2, bool isNewPosition = false);
	bool IsCollisionCircle(ObjectData* objectMe, ObjectData* objOther, bool isNewPosition);

	bool IsCollisionLineCircle(float x1, float y1, float x2, float y2,
		float xC, float yC, float R);

	bool IsCollisionObject(int indexObjMe, int& indexObjHit, bool isNewPosition = false);
	bool IsCollisionObject(ObjectData* objectMe, int& indexObjHit, bool isNewPosition = false);

	bool IsCollisionDynamicObject(int indexObjMe, int& indexObjHit, bool isNewPosition = false);
	bool IsCollisionDynamicObject(ObjectData* objectMe, int& indexObjHit, bool isNewPosition = false);

	bool IsCollisionBlocks(int indexObjMe, int& indexObjHit, bool isNewPosition = false);
	bool IsCollisionBlocks(ObjectData* objectMe, int& indexObjHit, bool isNewPosition = false);

	bool IsCollisionObjectToBlock(int indObjMe, int indBlock, bool isNewPosition);
	bool IsCollisionObjectToBlock(ObjectData* obj, ObjectData* objBlock, bool isNewPosition);
	
	//std::vector<glm::vec3> GetPosVertextPolygon(int ind, int x_sector, int z_sector);

	ColliseState IsCollisionPolygon(int indexObj, Plane* plane, vec4& vertex);
	ColliseState IsCollisionPolygon(ObjectData* obj, Plane* plane, vec4& vertex);

	ColliseState IsCollisionPolygonP2(int indexObj, Plane* plane, vec4& vertex);
};

#endif

