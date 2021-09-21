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

using std::shared_ptr;
using std::string;
using std::vector;
using std::map;
using glm::vec4;

class WorldCluster
{
private:
		World m_WorldSetting;
		WorldSectors* default_Sectors;
		map<string, int> ClasterPlanes;

public:
	//TODO: static !!!!
	int SectorSizePlane = 5; // 5;// 10;// 100;

	int SectorSize = 10;// 100;
	CreatorModelData* Storage;
	//WorldSectors* Sectors;
	map<string, WorldSectors*> Sectors;

	WorldCluster();

	void Init();
		
	void PlaneClusterization();

	void SaveClusterObject(ObjectData* obj);

	void SaveClusterDynamicColiderObject(ObjectData* object);

	void SaveClusterBlockObject(ObjectData* object);

	vector<int> GetSectorObjects(ObjectData* object, bool isNewPosition, TypeObject typeObj);

	vector<int> GetVertexPolygonFromObject(int indexObj, vector<string>& checkedZona);

	vector<int> GetIndexPlanePolygonFromObject(ObjectData* obj, vector<string>& checkedZona);

	bool IsCollisionCircle(ObjectData* objectMe, ObjectData* objOther, bool isNewPosition);

	bool IsCollisionLineCircle(float x1, float y1, float x2, float y2,
		float xC, float yC, float R);

	bool IsCollisionObject(ObjectData* objectMe, int& indexObjHit, bool isNewPosition = false);

	bool IsCollisionDynamicObject(ObjectData* objectMe, int& indexObjHit, bool isNewPosition = false);

	bool IsCollisionBlocks(ObjectData* objectMe, int& indexObjHit, bool isNewPosition = false);

	bool IsCollisionObjectToBlock(ObjectData* obj, ObjectData* objBlock, bool isNewPosition);

	ColliseState IsCollisionPolygon(ObjectData* obj, Plane* plane, vec4& vertex);

	ColliseState IsCollisionPolygonP2(int indexObj, Plane* plane, vec4& vertex);

	string GetKeyPlaneSector(vec3 pos, bool isOffset = false);

	void SectorsPlaneClear();

	WorldSectors* GetSectors(string key);

	void InitPlaneSectors(string key, int index);
	
	shared_ptr<ObjectData> GetSectorPolygon(string key);
};

#endif

