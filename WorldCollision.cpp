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

vec3 color_red = vec3(1, 0, 0);
vec3 color_blue = vec3(0, 0, 1);
vec3 color_green = vec3(0, 1, 0);
vec3 color_violet = vec3(1, 0, 1);
vec3 color_yelow = vec3(0, 1, 1);

WorldCluster::WorldCluster() {
	Sectors = new WorldSectors();
}

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


ColliseState WorldCluster::IsCollisionPolygonP2(int indexObj, Plane* plane, vec4& vertexOut)
{
	ColliseState stateResult = COLLISE_DOWN;
	std::shared_ptr<ObjectData> polygon = Storage->CurrentPolygonObject;
	std::shared_ptr<ObjectData> object = Storage->GetObjectPrt(indexObj);

	vector<string> checkedZona = vector<string>();
	vector<int> indexesPlane = GetIndexPlanePolygonFromObject(indexObj, checkedZona);

	shared_ptr<Plane> planePrt;

	float radius = object->ModelPtr->MeshData.RadiusCollider;
	vec3 pos = object->Postranslate;
	pos = vec3(pos.x, pos.y - radius, pos.z);
	//vec3 posDown = vec3(pos.x, pos.y - radius, pos.z);
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

	//----
	float testMinDist = 50000;


	for (const int indPlane : indexesPlane)
	{
		planePrt = polygon->GetPlanePrt(indPlane);
		planePrt->SetInspectPoint(pos, radius);
		planePrt->CalculatePlaneIntersectionPoint();
		int type = planePrt->ClassifyPointType;

		/*planePrt->CalculatePolygonIntersectionPoint();
		nearestPolygonIntersectionPoint = planePrt->P_Point;*/

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
	//if (nearestSphereIntersectionPoint != vec3(-50000))
	//	object->TempVectors.push_back(nearestSphereIntersectionPoint);

	return stateResult;
}


ColliseState WorldCluster::IsCollisionPolygon(int indexObj, Plane * plane, vec4 & vertexOut)
{
	ColliseState stateResult = COLLISE_DOWN;
	std::shared_ptr<ObjectData> polygon = Storage->CurrentPolygonObject;
	std::shared_ptr<ObjectData> object = Storage->GetObjectPrt(indexObj);

	vector<string> checkedZona = vector<string>();
	vector<int> indexesPlane = GetIndexPlanePolygonFromObject(indexObj, checkedZona);

	shared_ptr<Plane> planePrt;

	float radius = object->ModelPtr->MeshData.RadiusCollider;
	vec3 pos = object->Postranslate;
	pos = vec3(pos.x, pos.y - radius, pos.z);

	object->PlaneDownIndex = -1;
	object->MaterialData.Color = vec3(1, 1, 1);
	object->TempVectors = vector<vec3>();
	object->PlaneDownPosition = vec3(0);

	float libra = 2;
	float y;
	float y1;
	float y2;
	vec3 posCalc = vec3(pos.x, 0, pos.z);
	int step = 1 / object->Speed;
	//----
	if (object->IsGravity)
		stateResult = COLLISE_NORMAL;

	for (const int indPlane : indexesPlane)
	{
		planePrt = polygon->GetPlanePrt(indPlane);
		vec3 posA = vec3(planePrt->V0_W.x, 0, planePrt->V0_W.z);
		vec3 posB = vec3(planePrt->V1_W.x, 0, planePrt->V1_W.z);
		vec3 posC = vec3(planePrt->V2_W.x, 0, planePrt->V2_W.z);

		if (CheckPointInTriangle(posCalc, posA, posB, posC)) {

			//debug
			object->PlaneDownIndex = planePrt->Index;
			object->TempVectors.push_back(planePrt->V0_W);
			object->TempVectors.push_back(planePrt->V1_W);
			object->TempVectors.push_back(planePrt->V2_W);

			float distA = glm::distance(posCalc, posA);
			float distB = glm::distance(posCalc, posB);
			float distC = glm::distance(posCalc, posC);
			
			if (distA < distB && distA < distC)
			{
				float factorAB = distA / distB / libra;
				float factorAC = distA / distC / libra;
				float vertAB = planePrt->V0_W.y - planePrt->V1_W.y;
				float vertAC = planePrt->V0_W.y - planePrt->V2_W.y;
				
				/*if (distB < distC)
					y = planePrt->V0_W.y - (vertAB * factorAB);
				else 
					y = planePrt->V0_W.y - (vertAC * factorAC);*/
				
				y1 = planePrt->V0_W.y - (vertAB * factorAB);
				y2 = planePrt->V0_W.y - (vertAC * factorAC);
			}
			else if (distB < distA && distB < distC)
			{
				float factorBA = distB / distA / libra;
				float factorBC = distB / distC / libra;

				float vertBA = planePrt->V1_W.y - planePrt->V0_W.y;
				float vertBC = planePrt->V1_W.y - planePrt->V2_W.y;
				/*if (distA < distC)
					y = planePrt->V1_W.y - (vertBA * factorBA);
				else
					y = planePrt->V1_W.y - (vertBC * factorBC);*/
				
				y1 = planePrt->V1_W.y - (vertBA * factorBA);
				y2 = planePrt->V1_W.y - (vertBC * factorBC);
			}
			else
			{
				float factorCA = distC / distA / libra;
				float factorCB = distC / distB / libra;

				float vertCA = planePrt->V2_W.y - planePrt->V0_W.y;
				float vertCB = planePrt->V2_W.y - planePrt->V1_W.y;
				/*if (distA < distB)
					y = planePrt->V2_W.y - (vertCA * factorCA);
				else
					y = planePrt->V2_W.y - (vertCB * factorCB);*/
				y1 = planePrt->V2_W.y - (vertCA * factorCA);
				y2 = planePrt->V2_W.y - (vertCB * factorCB);
			}
			y = (y1 + y2) / 2;
			object->PlaneDownPosition = vec3(pos.x, y, pos.z);

			int vers = 3;

			if (!object->IsGravity)
				vers = 2;
			
			if (object->TypeObj == Hero)
				vers = 1;

			//--- version 1
			if (vers == 1) {
				object->NewPostranslate.y = object->PlaneDownPosition.y + radius;
			}
			//--- version 2
			if (vers == 2) {
				if (pos.y < object->PlaneDownPosition.y)
					stateResult = COLLISE_UP;
				/*else if (pos.y > object->PlaneDownPosition.y + 0.5) {
					stateResult = COLLISE_DOWN;
				}*/
			}
			//--- version 3
			if (vers == 3) {
				float speed = (object->NewPostranslate.y - (radius*2) - object->PlaneDownPosition.y)/step;
				object->NewPostranslate.y -= speed;
			}
			//---------------------------
			break;
		}
	}
	return stateResult;
}

vector<int> WorldCluster::GetIndexPlanePolygonFromObject(int indexObj, vector<string>& checkedZona)
{
	std::shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);
	int radius = object->ModelPtr->MeshData.RadiusCollider;
	vector<int> resultPlaneIndexes = vector<int>();

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
		int x_sector = pos.x / SectorSizePlane; 
		int z_sector = pos.z / SectorSizePlane; 
		string keyPosSectorStr = std::to_string(x_sector) + "_" + std::to_string(z_sector);

		bool isExist = std::find(checkedZona.begin(), checkedZona.end(), keyPosSectorStr) != checkedZona.end();
		if (isExist)
			continue;
		checkedZona.push_back(keyPosSectorStr);

		if (Sectors->SectorsPlane.find(keyPosSectorStr) != Sectors->SectorsPlane.end())
		{
			vector<int> indexesPlane = Sectors->SectorsPlane[keyPosSectorStr];
			//--------------
			for (const int indPlane : indexesPlane) {
				bool isExist = std::find(resultPlaneIndexes.begin(), resultPlaneIndexes.end(), indPlane) != resultPlaneIndexes.end();
				if (isExist)
					continue;
				//resultPlaneIndexes.insert(std::end(resultPlaneIndexes), std::begin(indexesPlane), std::end(indexesPlane));
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

vector<int> WorldCluster::GetSectorObjects(int indexObj, bool isNewPosition, TypeObject typeObj)
{
	std::shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);
	int indObjOwner = object->IndexObjectOwner;
	int radius = object->ModelPtr->MeshData.RadiusCollider;
	vector<string> checkedZona = vector<string>();
	vector<int> resultIndexObjects = vector<int>();

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
		if(isNewPosition)
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

void WorldCluster::SaveClusterObject(int indexObj)
{
	shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);
	//if (object->TypeObj == Block) {
	if (object->IsHexagonModel) {
		SaveClusterBlockObject(indexObj);
	}
	else {
		SaveClusterDynamicColiderObject(indexObj);
	}
}

void WorldCluster::SaveClusterBlockObject(int indexObj) {

	shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);

	if (object->TypeObj != Block) {
		return;
	}
	
	//ObjectBlock* objP = dynamic_cast<ObjectBlock*>(od);
	shared_ptr <ObjectBlock> objectBlock = std::dynamic_pointer_cast<ObjectBlock>(object);
	map<int, vec3> blockVectors = objectBlock->Shape->BottomVectors;
	vec3 leftTop = blockVectors[0];
	vec3 rightBottom = blockVectors[0];
	vec3 vertexNext;
	vec3 blockPos = objectBlock->Postranslate;

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
	/*for (const auto& nextIndObj : indObjsInSecor){} */

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

void WorldCluster::SaveClusterDynamicColiderObject(int indexObj) {
	
	std::shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);
	int radius = object->ModelPtr->MeshData.RadiusCollider;
	vector<string> checkedZona = vector<string>();
	glm::vec3 pos;
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

	//составляем коэффициенты квадратного уравнения на пересечение прямой и окружности.
	//если на отрезке [0..1] есть отрицательные значения, значит отрезок пересекает окружность
	float a = dx * dx + dy * dy;
	float b = 2. * (x1 * dx + y1 * dy);
	float c = x1 * x1 + y1 * y1 - R * R;

	//а теперь проверяем, есть ли на отрезке [0..1] решения
	if (-b < 0)
		return (c < 0);
	if (-b < (2. * a))
		return ((4. * a * c - b * b) < 0);

	return (a + b + c < 0);
}

bool WorldCluster::IsCollisionObjectToBlock(int indObjMe, int indBlock, bool isNewPosition)
{
	std::shared_ptr <ObjectData> objectMe = Storage->GetObjectPrt(indObjMe);
	std::shared_ptr <ObjectData> objectBlock = Storage->GetObjectPrt(indBlock);
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

	shared_ptr <ObjectBlock> objBlock = std::dynamic_pointer_cast<ObjectBlock>(objectBlock);
	for (int indLine = 0; indLine < 4; indLine++) {
		vec4 line =  objBlock->Shape->GetLine(indLine);
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

bool WorldCluster::IsCollisionCircle(int indObjMe, int indObj2, bool isNewPosition)
{
	double x1, x2, y1, y2, r1, r2;
	std::shared_ptr <ObjectData> objectMe = Storage->GetObjectPrt(indObjMe);
	std::shared_ptr <ObjectData> object2 = Storage->GetObjectPrt(indObj2);

	if (isNewPosition) {
		x1 = objectMe->NewPostranslate.x;
		y1 = objectMe->NewPostranslate.z;
	}
	else{
		x1 = objectMe->Postranslate.x;
		y1 = objectMe->Postranslate.z;
	}
	r1 = objectMe->ModelPtr->MeshData.RadiusCollider;

	x2 = object2->Postranslate.x;
	y2 = object2->Postranslate.z;
	r2 = object2->ModelPtr->MeshData.RadiusCollider;

	double r = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); //расстояние между центрами
	// проверка на совпадение 2-ух окружностей
	if (r == 0 && r1 == r2)
		return true;

	if (r1 + r2 >= r && r1 + r >= r2 && r2 + r >= r1)
		return true;
	else
		return false;
}


bool WorldCluster::IsCollisionObject(int indexObjMe, int& indexObjHit, bool isNewPosition)
{
	indexObjHit = -1;
	bool result = false;
	result = IsCollisionDynamicObject(indexObjMe, indexObjHit, isNewPosition);
	if(!result)
		result = IsCollisionBlocks(indexObjMe, indexObjHit, isNewPosition);

	return result;
}

bool WorldCluster::IsCollisionDynamicObject(int indexObjMe, int& indexObjHit, bool isNewPosition)
{
	vector<int> indObjsInSecor = GetSectorObjects(indexObjMe, isNewPosition, NPC);
	std::shared_ptr <ObjectData> objNext;
	for (const auto& nextIndObj : indObjsInSecor)
	{
		if (nextIndObj == indexObjMe)
			continue;
		objNext = Storage->GetObjectPrt(nextIndObj);
		if (IsCollisionCircle(indexObjMe, objNext->Index, isNewPosition)) {
			indexObjHit = nextIndObj;
			return true;
		}
	}
	return false;
}

bool WorldCluster::IsCollisionBlocks(int indexObjMe, int& indexObjHit, bool isNewPosition)
{
	std::shared_ptr <ObjectData> objectMe = Storage->GetObjectPrt(indexObjMe);
	if (objectMe->TypeObj == Block)
		return false;

	vector<int> indObjsInSecor = GetSectorObjects(indexObjMe, isNewPosition, Block);
	std::shared_ptr <ObjectData> objNext;
	for (const auto& nextIndObj : indObjsInSecor)
	{
		if (nextIndObj == indexObjMe)
			continue;
		objNext = Storage->GetObjectPrt(nextIndObj);
		if (IsCollisionObjectToBlock(indexObjMe, objNext->Index, isNewPosition)) {
			indexObjHit = nextIndObj;
			return true;
		}
	}
	return false;
}





