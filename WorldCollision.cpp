//@@@
#include "WorldCollision.h"
#include "ModelData.h"
#include "ObjectsTypes/ObjectPhysic.h"
#include "ObjectsTypes/ObjectBlock.h"
#include "ObjectsTypes/ObjectData.h"
#include "GeometryLib.h"
#include "CreatorModelData.h"  //------<<<		//##$$ 4.
#include "./GeomertyShapes/ShapeBase.h"			//##$$ 4.

//----------------------

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <sstream>

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

using std::string;
using std::vector;
using glm::vec4;
using glm::vec3;
using glm::vec2;

vec3 color_red = vec3(1, 0, 0);
vec3 color_blue = vec3(0, 0, 1);
vec3 color_green = vec3(0, 1, 0);
vec3 color_violet = vec3(1, 0, 1);
vec3 color_yelow = vec3(0, 1, 1);

WorldCluster::WorldCluster() {

	default_Sectors = new WorldSectors();
	default_Sectors->IsEmpty = true;
	Init();
}

void WorldCluster::Init() {
	
	//Sectors = new WorldSectors();
	Sectors = map<string, WorldSectors*>{};
}

void WorldCluster::SectorsPlaneClear()
{
	for (auto itemKeys : Sectors) {
		Sectors[itemKeys.first]->SectorsPlane.clear();
	}
}

WorldSectors* WorldCluster::GetSectors(string key) {
	
	if (Sectors.find(key) == Sectors.end()) {
		return default_Sectors;
	}
	return Sectors[key];
}

void WorldCluster::InitPlaneSectors(string key, int index) {
	
	if (Sectors.find(key) != Sectors.end()) {
		return;
	}

	Sectors.insert({ key, new WorldSectors() });
	ClasterPlanes.insert({ key , index });
}

shared_ptr<ObjectData> WorldCluster::GetSectorPolygon(string key) {

	if (ClasterPlanes.find(key) == ClasterPlanes.end()) {
		return nullptr;
	}

	return Storage->GetObjectPrt(ClasterPlanes[key]);
}

//---- v.1 
/*
void WorldCluster::PlaneClusterization()
{

	Sectors->SectorsPlane.clear();

	shared_ptr<ObjectData> object = Storage->CurrentPolygonObject;
	shared_ptr<ModelData> model = object->ModelPtr;
	vector<shared_ptr<Plane>> planes = object->Shape->Planes;

	for (shared_ptr<Plane> plane : planes)
	{
		vec3 posWorldA = object->Shape->ToWorldPosition(plane->V0);
		vec3 posWorldB = object->Shape->ToWorldPosition(plane->V1);
		vec3 posWorldC = object->Shape->ToWorldPosition(plane->V2);

		vec3 planeArr[3] = { posWorldA , posWorldB  , posWorldC };

		for (int indVert = 0; indVert < planeArr->length(); indVert++)
		{
			glm::vec3 vecItem = planeArr[indVert];

			int x_sector = vecItem.x / SectorSizePlane;
			int z_sector = vecItem.z / SectorSizePlane;
			string keyPosSectorStr = std::to_string(x_sector) + "_" + std::to_string(z_sector);

			if (Sectors->SectorsPlane.find(keyPosSectorStr) == Sectors->SectorsPlane.end())
			{
				vector<int> value_planeIndexs = vector<int>();
				value_planeIndexs.push_back(plane->Index);
				Sectors->SectorsPlane.insert(std::pair<string, vector<int>>(keyPosSectorStr, value_planeIndexs));
			}
			else {
				vector<int> value_planeIndexs = Sectors->SectorsPlane[keyPosSectorStr];

				//----check --  value_planeIndexs

				bool isExist = std::find(value_planeIndexs.begin(), value_planeIndexs.end(), plane->Index) != value_planeIndexs.end();
				if (isExist)
					continue;
				value_planeIndexs.push_back(plane->Index);

				Sectors->SectorsPlane[keyPosSectorStr] = value_planeIndexs;
			}
		}
	}
	
}
*/

string WorldCluster::GetKeyPlaneSector(vec3 pos, bool isOffset)
{
	if (isOffset) {
		pos = vec3(pos.x - m_WorldSetting.StartPlaneOffset.x, 0, pos.z - m_WorldSetting.StartPlaneOffset.z);
		pos.x /= m_WorldSetting.PlaneRadius;
		pos.z /= m_WorldSetting.PlaneRadius;
	}
	else {
		if (int(pos.x) < m_WorldSetting.PlaneRadius50)
		{
			pos.x /= m_WorldSetting.PlaneRadius50;
		}
		else
		{
			if (pos.x > 0)
				pos.x += m_WorldSetting.PlaneRadius50;
			else
				pos.x -= m_WorldSetting.PlaneRadius50;

			pos.x /= m_WorldSetting.PlaneRadius;
		}
		if (int(pos.z) < m_WorldSetting.PlaneRadius50)
		{
			pos.z /= m_WorldSetting.PlaneRadius50;
		}
		else
		{
			if (pos.z > 0)
				pos.z += m_WorldSetting.PlaneRadius50;
			else
				pos.z -= m_WorldSetting.PlaneRadius50;

			pos.z /= m_WorldSetting.PlaneRadius;
		}
	}
	return std::to_string(int(pos.x)) + "_" + std::to_string(int(pos.z));
}

void WorldCluster::PlaneClusterization()
{
	SectorsPlaneClear();
	
	string keyP;
	ClasterPlanes = map<string, int>();

	shared_ptr<ObjectData> object;
	int countObjects = Storage->SceneObjectsLastIndex;
	
	for (int i = 0; i < countObjects + 1; i++)
	{
		object = Storage->GetObjectPrt(i);
		if (object->TypeObj != TypeObject::Polygon)
			continue;

		keyP = GetKeyPlaneSector(object->Postranslate, true);
		InitPlaneSectors(keyP, object->Index);

		WorldSectors* Sectors = GetSectors(keyP);
		if (Sectors->IsEmpty) {
			return;
		}

		for (Plane plane : object->Shape->Planes)
		{
			vec3 posWorldA = object->Shape->ToWorldPosition(plane.V0);
			vec3 posWorldB = object->Shape->ToWorldPosition(plane.V1);
			vec3 posWorldC = object->Shape->ToWorldPosition(plane.V2);

			vec3 planeArr[3] = { posWorldA , posWorldB  , posWorldC };

			for (int indVert = 0; indVert < planeArr->length(); indVert++)
			{
				glm::vec3 vecItem = planeArr[indVert];

				int x_sector = vecItem.x / SectorSizePlane;
				int z_sector = vecItem.z / SectorSizePlane;
				string keyPosSectorStr = std::to_string(x_sector) + "_" + std::to_string(z_sector);

				if (Sectors->SectorsPlane.find(keyPosSectorStr) == Sectors->SectorsPlane.end())
				{
					vector<int> value_planeIndexs = vector<int>();
					value_planeIndexs.push_back(plane.Index);
					Sectors->SectorsPlane.insert(std::pair<string, vector<int>>(keyPosSectorStr, value_planeIndexs));
				}
				else {
					vector<int> value_planeIndexs = Sectors->SectorsPlane[keyPosSectorStr];

					//----check --  value_planeIndexs

					bool isExist = std::find(value_planeIndexs.begin(), value_planeIndexs.end(), plane.Index) != value_planeIndexs.end();
					if (isExist)
						continue;
					value_planeIndexs.push_back(plane.Index);

					Sectors->SectorsPlane[keyPosSectorStr] = value_planeIndexs;
				}
			}
		}
	}

}

ColliseState WorldCluster::IsCollisionPolygonP2(int indexObj, Plane* plane, vec4& vertexOut)
{
	return COLLISE_DOWN;
	/*
	ColliseState stateResult = COLLISE_DOWN;
	std::shared_ptr<ObjectData> polygon = Storage->CurrentPolygonObject;
	std::shared_ptr<ObjectData> object = Storage->GetObjectPrt(indexObj);

	vector<string> checkedZona = vector<string>();
	vector<int> indexesPlane = GetIndexPlanePolygonFromObject(indexObj, checkedZona);

	shared_ptr<Plane> planePrt;

	float radius = object->ModelPtr->MeshData.RadiusCollider;
	vec3 pos = object->Postranslate;
	pos = vec3(pos.x, pos.y - radius, pos.z);
	bool foundCollision = false;
	float nearestDistance = 1000000;
	vec3  nearestSphereIntersectionPoint = vec3(-50000);
	vec3 nearestPolygonIntersectionPoint = vec3(-50000);
	int indPlaneA = -1;
	int indPlaneB = -1;
	int indPlaneC = -1;

	object->PlaneDownIndex = -1;

	vec3 color_red = vec3(1, 0, 0);
	vec3 color_blue = vec3(0, 0, 1);
	vec3 color_green = vec3(0, 1, 0);
	vec3 color_violet = vec3(1, 0, 1);
	vec3 color_yelow = vec3(0, 1, 1);

	object->MaterialData.Color = vec3(1, 1, 1);

	object->TempVectors = vector<vec3>();
	float testMinDist = 50000;

	for (const int indPlane : indexesPlane)
	{
		planePrt = polygon->GetPlanePrt(indPlane);
		planePrt->SetInspectPoint(pos, radius);
		planePrt->CalculatePlaneIntersectionPoint();
		int type = planePrt->ClassifyPointType;

		if (type == BEHIND)
		{
			//planePrt->CalculatePolygonIntersectionPoint();

			indPlaneC = planePrt->Index;
			if (indPlaneB == -1) {
				nearestPolygonIntersectionPoint = planePrt->P_Point;
				object->PlaneDownIndex = planePrt->Index;
			}

			// any chance of hit ?
			if ((planePrt->DistToSphereIntersection > 0) && (planePrt->DistToSphereIntersection <= planePrt->Velocity.length())) {

				//object->Color = color_red;
				indPlaneB = planePrt->Index;
				if (indPlaneA == -1) {
					//nearestPolygonIntersectionPoint = planePrt->P_Point;
					object->PlaneDownIndex = planePrt->Index;
				}

				// if first hit, or closest hit so far
				if ((foundCollision == false) || (planePrt->DistToSphereIntersection < nearestDistance)) {

					nearestDistance = planePrt->DistToSphereIntersection;
					nearestSphereIntersectionPoint = planePrt->E_Point;
					//nearestPolygonIntersectionPoint = planePrt->P_Point;
					foundCollision = true;

					indPlaneA = planePrt->Index;
					object->PlaneDownIndex = planePrt->Index;
				}
			}
		}
	
	}

	if (nearestPolygonIntersectionPoint != vec3(-50000))
		object->TempVectors.push_back(nearestPolygonIntersectionPoint);

	return stateResult;
	*/

}

ColliseState WorldCluster::IsCollisionPolygon(ObjectData* object, Plane* plane, vec4& vertexOut)
{
	int indexObj = object->Index;
	ColliseState stateResult = COLLISE_DOWN;

	vector<string> checkedZona = vector<string>();
	vector<int> indexesPlane = GetIndexPlanePolygonFromObject(object, checkedZona);

	float radius = object->ModelPtr->MeshData.RadiusCollider;
	vec3 pos = object->Postranslate;
	pos = vec3(pos.x, pos.y - radius, pos.z);

	object->PlaneDownIndex = -1;
	object->MaterialData.Color = vec3(1, 1, 1);
	object->TempVectors = vector<vec3>();
	object->PlaneDownPosition = vec3(0);

	float libra = 2;
	float y, y1, y2;
	vec3 posCalc = vec3(pos.x, 0, pos.z);
	int step = 1 / object->Speed;
	//----
	if (object->IsGravity)
		stateResult = COLLISE_NORMAL;

	vec3 V0_W, V1_W, V2_W, posA, posB, posC;
	int planeDownIndex = object->PlaneDownIndex;
	vec3 planeDownPosition = object->PlaneDownPosition;
	vec3 newPostranslate = object->NewPostranslate;
	bool isGravity = object->IsGravity;
	bool isHero = object->TypeObj == Hero;
	bool isTestVertex = object->Name == "Mon";


	string keyP = GetKeyPlaneSector(object->Postranslate);
	//----------- POLYGON
	shared_ptr<ObjectData> polygon = GetSectorPolygon(keyP);
	if (polygon == nullptr)
	{
		return stateResult;
	}

	for (const int indPlane : indexesPlane)
	{
		std::tie(V0_W, V1_W, V2_W) = polygon->GetCase_V_W(indPlane);
		posA = vec3(V0_W.x, 0, V0_W.z);
		posB = vec3(V1_W.x, 0, V1_W.z);
		posC = vec3(V2_W.x, 0, V2_W.z);

		if (CheckPointInTriangle(posCalc, posA, posB, posC)) {

			planeDownIndex = indPlane;

			//-- TEST - collision
			if (isTestVertex) {
				object->TempVectors.push_back(V0_W);
				object->TempVectors.push_back(V1_W);
				object->TempVectors.push_back(V2_W);
			}

			float distA = glm::distance(posCalc, posA);
			float distB = glm::distance(posCalc, posB);
			float distC = glm::distance(posCalc, posC);

			if (distA < distB && distA < distC)
			{
				float factorAB = distA / distB / libra;
				float factorAC = distA / distC / libra;
				float vertAB = V0_W.y - V1_W.y;
				float vertAC = V0_W.y - V2_W.y;

				y1 = V0_W.y - (vertAB * factorAB);
				y2 = V0_W.y - (vertAC * factorAC);
			}
			else if (distB < distA && distB < distC)
			{
				float factorBA = distB / distA / libra;
				float factorBC = distB / distC / libra;

				float vertBA = V1_W.y - V0_W.y;
				float vertBC = V1_W.y - V2_W.y;

				y1 = V1_W.y - (vertBA * factorBA);
				y2 = V1_W.y - (vertBC * factorBC);
			}
			else
			{
				float factorCA = distC / distA / libra;
				float factorCB = distC / distB / libra;

				float vertCA = V2_W.y - V0_W.y;
				float vertCB = V2_W.y - V1_W.y;
				y1 = V2_W.y - (vertCA * factorCA);
				y2 = V2_W.y - (vertCB * factorCB);
			}
			y = (y1 + y2) / 2;
			planeDownPosition = vec3(pos.x, y, pos.z);

			int vers = 3;

			if (!isGravity)
				vers = 2;

			if (isHero)
				vers = 1;

			//--- version 1
			if (vers == 1) {
				newPostranslate.y = planeDownPosition.y + radius;
			}
			//--- version 2
			if (vers == 2) {
				if (pos.y < planeDownPosition.y)
					stateResult = COLLISE_UP;
			}
			//--- version 3
			if (vers == 3) {
				float speed = (newPostranslate.y - (radius * 2) - planeDownPosition.y) / step;
				newPostranslate.y -= speed;
			}
			//---------------------------
			break;
		}
	}

	object->PlaneDownIndex = planeDownIndex;
	object->PlaneDownPosition = planeDownPosition;
	object->NewPostranslate = newPostranslate;

	return stateResult;
}


vector<int> WorldCluster::GetIndexPlanePolygonFromObject(ObjectData* object, vector<string>& checkedZona)
{
	int indexObj = object->Index;
	//std::shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);
	int radius = object->ModelPtr->MeshData.RadiusCollider;
	vector<int> resultPlaneIndexes = vector<int>();

	vec3 pos;
	vec3 posStart = object->Postranslate;

	int x_sector;
	int z_sector;
	string keyPosSectorStr;
	bool isExist;
	vector<int> indexesPlane;

	string keyP = GetKeyPlaneSector(posStart);
	WorldSectors* Sectors = GetSectors(keyP);
	if (Sectors->IsEmpty) {
		return resultPlaneIndexes;
	}

	//clear old  position in zona
	for (int i = 0; i <= 4; i++)
	{
		pos = posStart;
		if (i == 0)	
		{
			pos.x -= radius;
			pos.z -= radius;
		}
		if (i == 1)
		{
			pos.x -= radius;
			pos.z += radius;
		}
		if (i == 2)
		{
			pos.x += radius;
			pos.z -= radius;
		}
		if (i == 3)
		{
			pos.x += radius;
			pos.z += radius;
		}
		x_sector = pos.x / SectorSizePlane; 
		z_sector = pos.z / SectorSizePlane; 
		keyPosSectorStr = std::to_string(x_sector) + "_" + std::to_string(z_sector);

		isExist = std::find(checkedZona.begin(), checkedZona.end(), keyPosSectorStr) != checkedZona.end();
		if (isExist)
			continue;
		checkedZona.push_back(keyPosSectorStr);

		if (Sectors->SectorsPlane.find(keyPosSectorStr) != Sectors->SectorsPlane.end())
		{
			indexesPlane = Sectors->SectorsPlane[keyPosSectorStr];
			//--------------
			for (const int indPlane : indexesPlane) {
				isExist = std::find(resultPlaneIndexes.begin(), resultPlaneIndexes.end(), indPlane) != resultPlaneIndexes.end();
				if (isExist)
					continue;
				resultPlaneIndexes.push_back(indPlane);
			}
		}
	}
	return resultPlaneIndexes;
}


vector<int> WorldCluster::GetVertexPolygonFromObject(int indexObj, vector<string>& checkedZona)
{
	std::shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);
	int radius = object->ModelPtr->MeshData.RadiusCollider;
	vector<int> resultVertex = vector<int>();
	//vector<string> checkedZona = vector<string>();

	string keyP = GetKeyPlaneSector(object->Postranslate);
	WorldSectors* Sectors = GetSectors(keyP);

	if (Sectors->IsEmpty) {
		return resultVertex;
	}

	glm::vec3 pos;
	//clear old  position in zona
	for (int i = 0; i <= 4; i++)
	{
		pos = object->Postranslate;
		if (i == 0)
		{
			pos.x -= radius;
			pos.z -= radius;
		}
		if (i == 1)
		{
			pos.x -= radius;
			pos.z += radius;
		}
		if (i == 2)
		{
			pos.x += radius;
			pos.z -= radius;
		}
		if (i == 3)
		{
			pos.x += radius;
			pos.z += radius;
		}


		//int x_sector = pos.x / SectorSize; // SectorSizePlane;
		//int z_sector = pos.z / SectorSize; // SectorSizePlane;
		int x_sector = pos.x / SectorSizePlane; // SectorSizePlane;
		int z_sector = pos.z / SectorSizePlane; // SectorSizePlane;
		string keyPosSectorStr = std::to_string(x_sector) + "_" + std::to_string(z_sector);

		bool isExist = std::find(checkedZona.begin(), checkedZona.end(), keyPosSectorStr) != checkedZona.end();
		if (isExist)
			continue;
		checkedZona.push_back(keyPosSectorStr);

		if (Sectors->SectorsPlane.find(keyPosSectorStr) != Sectors->SectorsPlane.end())
		{
			vector<int> vertexPlane = Sectors->SectorsPlane[keyPosSectorStr];
			resultVertex.insert(std::end(resultVertex), std::begin(vertexPlane), std::end(vertexPlane));
		}
	}
	return resultVertex;
}

vector<int> WorldCluster::GetSectorObjects(ObjectData* object, bool isNewPosition, TypeObject typeObj)
{
	int indexObj = object->Index;
	int indObjOwner = object->IndexObjectOwner;
	int radius = object->ModelPtr->MeshData.RadiusCollider;
	vector<string> checkedZona = vector<string>();
	vector<int> resultIndexObjects = vector<int>();

	string keyP = GetKeyPlaneSector(object->Postranslate);
	WorldSectors* Sectors = GetSectors(keyP);
	if (Sectors->IsEmpty) {
		return resultIndexObjects;
	}

	std::map <std::string, std::vector<int>> sectorsObjects;
	switch (typeObj)
	{
	case TypeObject::Polygon:
		sectorsObjects = Sectors->SectorsPlane;
		break;
	case TypeObject::Block:
		sectorsObjects = Sectors->SectorsBlocks;
		break;
	case TypeObject::NPC:
		sectorsObjects = Sectors->SectorsObjects;
		break;
	default:
		sectorsObjects = Sectors->SectorsObjects;
		break;
	}

	glm::vec3 pos;
	for (int i = 0; i <= 5; i++)
	{
		if (isNewPosition)
			pos = object->NewPostranslate;
		else
			pos = object->Postranslate;
		if (i == 0)
		{
			pos.x -= radius;
			pos.z -= radius;
		}
		if (i == 1)
		{
			pos.x -= radius;
			pos.z += radius;
		}
		if (i == 2)
		{
			pos.x += radius;
			pos.z -= radius;
		}
		if (i == 3)
		{
			pos.x += radius;
			pos.z += radius;
		}

		int x_sector = pos.x / SectorSize;
		int z_sector = pos.z / SectorSize;
		string keyPosSectorStr = std::to_string(x_sector) + "_" + std::to_string(z_sector);

		bool isExist = std::find(checkedZona.begin(), checkedZona.end(), keyPosSectorStr) != checkedZona.end();
		if (isExist)
			continue;
		checkedZona.push_back(keyPosSectorStr);

		if (sectorsObjects.find(keyPosSectorStr) != sectorsObjects.end())
		{
			vector<int> indObjsInSecor = sectorsObjects[keyPosSectorStr];
			for (const auto& nextIndObj : indObjsInSecor) {
				if (nextIndObj == indexObj || nextIndObj == indObjOwner)
					continue;
				resultIndexObjects.push_back(nextIndObj);
			}
		}
	}
	return resultIndexObjects;
}


void WorldCluster::SaveClusterObject(ObjectData* object)
{
	//if (object->TypeObj == Block) {
	if (object->IsHexagonModel) {
		SaveClusterBlockObject(object);
	}
	else {
		SaveClusterDynamicColiderObject(object);
	}
}

void WorldCluster::SaveClusterBlockObject(ObjectData* object) {

	if (object->TypeObj != Block) {
		return;
	}

	int indexObj = object->Index;

	string keyP = GetKeyPlaneSector(object->Postranslate);
	WorldSectors* Sectors = GetSectors(keyP);
	if (Sectors->IsEmpty) {
		return;
	}

	map<int, vec3> blockVectors = object->Shape->BottomVectors;
	vec3 leftTop = blockVectors[0];
	vec3 rightBottom = blockVectors[0];
	vec3 vertexNext;
	vec3 blockPos = object->Postranslate;

	//clear old  position in zona
	for (std::map<string, vector<int>>::iterator it = Sectors->SectorsBlocks.begin(); it != Sectors->SectorsBlocks.end(); ++it)
	{
		string keyPosSectorStr = it->first;
		vector<int> value_objectIndexs = Sectors->SectorsBlocks[keyPosSectorStr];
		for (int ind = value_objectIndexs.size() - 1; ind >= 0; ind--)
		{
			if (value_objectIndexs[ind] == indexObj) {
				value_objectIndexs.erase(value_objectIndexs.begin() + ind);
				Sectors->SectorsBlocks[keyPosSectorStr] = value_objectIndexs;
			}
		}
	}

	//save new position in zona
	for (std::map<int, vec3>::iterator it = blockVectors.begin(); it != blockVectors.end(); ++it)
	{
		vertexNext = it->second;
		if (vertexNext.x < leftTop.x)
			leftTop.x = vertexNext.x;
		if (vertexNext.z < leftTop.z)
			leftTop.z = vertexNext.z;
		if (vertexNext.x > rightBottom.x)
			rightBottom.x = vertexNext.x;
		if (vertexNext.z > rightBottom.z)
			rightBottom.z = vertexNext.z;
	}

	int minX = (leftTop.x + blockPos.x) / SectorSize;
	int maxX = (rightBottom.x + blockPos.x) / SectorSize;
	int minZ = (leftTop.z + blockPos.z) / SectorSize;
	int maxZ = (rightBottom.z + blockPos.z) / SectorSize;

	for (int x_sector = minX; x_sector <= maxX; x_sector++) {
		for (int z_sector = minZ; z_sector <= maxZ; z_sector++) {
			string keyPosSectorStr = std::to_string(x_sector) + "_" + std::to_string(z_sector);

			if (Sectors->SectorsBlocks.find(keyPosSectorStr) == Sectors->SectorsBlocks.end())
			{
				vector<int> value_objectIndexs = vector<int>();
				value_objectIndexs.push_back(indexObj);
				Sectors->SectorsBlocks.insert(std::pair<string, vector<int>>(keyPosSectorStr, value_objectIndexs));
			}
			else {
				vector<int> value_objectIndexs = Sectors->SectorsBlocks[keyPosSectorStr];
				value_objectIndexs.push_back(indexObj);
				Sectors->SectorsBlocks[keyPosSectorStr] = value_objectIndexs;
			}
		}
	}
}

void WorldCluster::SaveClusterDynamicColiderObject(ObjectData* object) {

	int indexObj = object->Index;
	//std::shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);
	int radius = object->ModelPtr->MeshData.RadiusCollider;
	vector<string> checkedZona = vector<string>();
	glm::vec3 pos;

	string keyP = GetKeyPlaneSector(object->Postranslate);
	WorldSectors* Sectors = GetSectors(keyP);
	if (Sectors->IsEmpty) {
		return;
	}

	//clear old  position in zona
	for (int i = 0; i <= 5; i++)
	{
		pos = object->Postranslate;
		if (i == 0)
		{
			pos.x -= radius;
			pos.z -= radius;
		}
		if (i == 1)
		{
			pos.x -= radius;
			pos.z += radius;
		}
		if (i == 2)
		{
			pos.x += radius;
			pos.z -= radius;
		}
		if (i == 3)
		{
			pos.x += radius;
			pos.z += radius;
		}
		if (i == 4)
		{
		}

		int x_sector = pos.x / SectorSize;
		int z_sector = pos.z / SectorSize;
		string keyPosSectorStr = std::to_string(x_sector) + "_" + std::to_string(z_sector);

		bool isExist = std::find(checkedZona.begin(), checkedZona.end(), keyPosSectorStr) != checkedZona.end();
		if (isExist)
			continue;
		checkedZona.push_back(keyPosSectorStr);

		if (Sectors->SectorsObjects.find(keyPosSectorStr) != Sectors->SectorsObjects.end())
		{
			vector<int> value_objectIndexs = Sectors->SectorsObjects[keyPosSectorStr];
			for (int ind = value_objectIndexs.size() - 1; ind >= 0; ind--)
			{
				if (value_objectIndexs[ind] == indexObj)
					value_objectIndexs.erase(value_objectIndexs.begin() + ind);
			}
			Sectors->SectorsObjects[keyPosSectorStr] = value_objectIndexs;
		}
	}

	checkedZona.clear();

	//save new position in zona
	for (int i = 0; i < 5; i++)
	{
		pos = object->NewPostranslate;
		if (i == 0)
		{
			pos.x -= radius;
			pos.z -= radius;
		}
		if (i == 1)
		{
			pos.x -= radius;
			pos.z += radius;
		}
		if (i == 2)
		{
			pos.x += radius;
			pos.z -= radius;
		}
		if (i == 3)
		{
			pos.x += radius;
			pos.z += radius;
		}
		if (i == 4)
		{
		}

		int x_sector = pos.x / SectorSize;
		int z_sector = pos.z / SectorSize;
		string keyPosSectorStr = std::to_string(x_sector) + "_" + std::to_string(z_sector);

		bool isExist = std::find(checkedZona.begin(), checkedZona.end(), keyPosSectorStr) != checkedZona.end();
		if (isExist)
			continue;
		checkedZona.push_back(keyPosSectorStr);

		if (Sectors->SectorsObjects.find(keyPosSectorStr) == Sectors->SectorsObjects.end())
		{
			vector<int> value_objectIndexs = vector<int>();
			value_objectIndexs.push_back(indexObj);
			Sectors->SectorsObjects.insert(std::pair<string, vector<int>>(keyPosSectorStr, value_objectIndexs));
		}
		else {
			vector<int> value_objectIndexs = Sectors->SectorsObjects[keyPosSectorStr];
			value_objectIndexs.push_back(indexObj);
			Sectors->SectorsObjects[keyPosSectorStr] = value_objectIndexs;
		}
	}
}

bool WorldCluster::IsCollisionLineCircle(float x1, float y1, float x2, float y2,
	float xC, float yC, float R)
{
	x1 -= xC;
	y1 -= yC;
	x2 -= xC;
	y2 -= yC;

	float dx = x2 - x1;
	float dy = y2 - y1;

	float a = dx * dx + dy * dy;
	float b = 2. * (x1 * dx + y1 * dy);
	float c = x1 * x1 + y1 * y1 - R * R;

	if (-b < 0)
		return (c < 0);
	if (-b < (2. * a))
		return ((4. * a * c - b * b) < 0);

	return (a + b + c < 0);
}

bool WorldCluster::IsCollisionObjectToBlock(ObjectData* objectMe, ObjectData* objBlock, bool isNewPosition)
{
	float x1, y1, x2, y2, xC, yC, r;
	bool isCollision = false;

	if (isNewPosition) {
		xC = objectMe->NewPostranslate.x;
		yC = objectMe->NewPostranslate.z;
	}
	else {
		xC = objectMe->Postranslate.x;
		yC = objectMe->Postranslate.z;
	}
	r = objectMe->ModelPtr->MeshData.RadiusCollider;

	for (int indLine = 0; indLine < 4; indLine++) {
		vec4 line = objBlock->Shape->GetLine(indLine);
		x1 = line.x;
		y1 = line.y;
		x2 = line.z;
		y2 = line.w;

		if (IsCollisionLineCircle(x1, y1, x2, y2, xC, yC, r)) {
			isCollision = true;
			break;
		}
	}
	return isCollision;
}

bool WorldCluster::IsCollisionCircle(ObjectData* objectMe, ObjectData* objOther, bool isNewPosition)
{
	double x1, x2, y1, y2, r1, r2;
	
	if (isNewPosition) {
		x1 = objectMe->NewPostranslate.x;
		y1 = objectMe->NewPostranslate.z;
	}
	else {
		x1 = objectMe->Postranslate.x;
		y1 = objectMe->Postranslate.z;
	}
	r1 = objectMe->ModelPtr->MeshData.RadiusCollider;

	x2 = objOther->Postranslate.x;
	y2 = objOther->Postranslate.z;
	r2 = objOther->ModelPtr->MeshData.RadiusCollider;

	double r = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	if (r == 0 && r1 == r2)
		return true;

	if (r1 + r2 >= r && r1 + r >= r2 && r2 + r >= r1)
		return true;
	else
		return false;
}

bool WorldCluster::IsCollisionObject(ObjectData* objectMe, int& indexObjHit, bool isNewPosition)
{
	indexObjHit = -1;
	bool result = false;
	result = IsCollisionDynamicObject(objectMe, indexObjHit, isNewPosition);
	if (!result)
		result = IsCollisionBlocks(objectMe, indexObjHit, isNewPosition);

	return result;
}

bool WorldCluster::IsCollisionDynamicObject(ObjectData* objectMe, int& indexObjHit, bool isNewPosition)
{
	vector<int> indObjsInSecor = GetSectorObjects(objectMe, isNewPosition, NPC);
	std::shared_ptr <ObjectData> objNext;
	for (const auto& nextIndObj : indObjsInSecor)
	{
		if (nextIndObj == objectMe->Index)
			continue;
		objNext = Storage->GetObjectPrt(nextIndObj);
		if (IsCollisionCircle(objectMe, objNext.get(), isNewPosition)) {
			indexObjHit = nextIndObj;
			return true;
		}
	}
	return false;
}

bool WorldCluster::IsCollisionBlocks(ObjectData* objectMe, int& indexObjHit, bool isNewPosition)
{
	if (objectMe->TypeObj == Block)
		return false;

	vector<int> indObjsInSecor = GetSectorObjects(objectMe, isNewPosition, Block);
	std::shared_ptr <ObjectData> objNext;
	for (const auto& nextIndObj : indObjsInSecor)
	{
		if (nextIndObj == objectMe->Index)
			continue;
		objNext = Storage->GetObjectPrt(nextIndObj);
		if (IsCollisionObjectToBlock(objectMe, objNext.get(), isNewPosition)) {
			indexObjHit = nextIndObj;
			return true;
		}
	}
	return false;
}


