//@@@
#include "WorldCollision.h"
#include "ModelData.h"
#include "ObjectData.h"
#include "GeometryLib.h"
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

	std::shared_ptr<ObjectData> object = Storage->CurrentPolygonObject;
	std::shared_ptr<ModelData> model = object->ModelPtr;
	//std::vector< glm::vec3 > vertices = model->Vertices;
	//std::vector<Plane> planes = *object->Planes;
	std::vector<shared_ptr<Plane>> planes = object->Planes;

	//for (const auto& vec : planes) {
	//for (int indVert = 0; indVert < vertices.size(); indVert++)
	for (shared_ptr<Plane> plane : planes)
	{
		vec3 posWorldA = object->ToWorldPosition(plane->V0);
		vec3 posWorldB = object->ToWorldPosition(plane->V1);
		vec3 posWorldC = object->ToWorldPosition(plane->V2);

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

	float radius = object->ModelPtr->RadiusCollider;
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

	object->Color = vec3(1, 1, 1);

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

	float radius = object->ModelPtr->RadiusCollider;
	vec3 pos = object->Postranslate;
	pos = vec3(pos.x, pos.y - radius, pos.z);

	object->PlaneDownIndex = -1;
	object->Color = vec3(1, 1, 1);
	object->TempVectors = vector<vec3>();
	object->PlaneDownPosition = vec3(0);

	float libra = 2;
	float y;
	float y1;
	float y2;
	vec3 posCalc = vec3(pos.x, 0, pos.z);
	int step = 1 / object->Speed;
	//----
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
			if (object->TypeObj == Hero)
				vers = 1;
			//--- version 1
			if (vers == 1) {
				//object->Postranslate.y = object->PlaneDownPosition.y + radius;
				object->NewPostranslate.y = object->PlaneDownPosition.y + radius;
			}
			//--- version 2
			if (vers == 2) {
				if (pos.y < object->PlaneDownPosition.y)
					stateResult = COLLISE_UP;
				else if (pos.y > object->PlaneDownPosition.y + 0.5) {
					//object->Postranslate.y = object->PlaneDownPosition.y + radius;
					//object->NewPostranslate.y = object->PlaneDownPosition.y + radius;
					stateResult = COLLISE_DOWN;
				}
			}
			//--- version 3
			if (vers == 3) {
				float speed = (object->NewPostranslate.y - (radius*2) - object->PlaneDownPosition.y)/step;
				object->NewPostranslate.y -= speed;
			}
			//---------------------------

			break;
		}
		//---------------------
	}

	return stateResult;
}

vector<int> WorldCluster::GetIndexPlanePolygonFromObject(int indexObj, vector<string>& checkedZona)
{
	std::shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);
	int radius = object->ModelPtr->RadiusCollider;
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
	int radius = object->ModelPtr->RadiusCollider;
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

vector<int> WorldCluster::GetSectorObjects(int indexObj, bool isNewPosition)
{
	std::shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);
	int radius = object->ModelPtr->RadiusCollider;
	vector<string> checkedZona = vector<string>();
	vector<int> resultIndexObjects = vector<int>();

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

		if (Sectors->SectorsObjects.find(keyPosSectorStr) != Sectors->SectorsObjects.end())
		{
			vector<int> indObjsInSecor = Sectors->SectorsObjects[keyPosSectorStr];
			for (const auto& nextIndObj : indObjsInSecor) {
				if (nextIndObj == indexObj)
					continue;
				resultIndexObjects.push_back(nextIndObj);
			}
		}
	}
	return resultIndexObjects;
}

void WorldCluster::SaveClusterObject(int indexObj)
{
	std::shared_ptr <ObjectData> object = Storage->GetObjectPrt(indexObj);
	int radius = object->ModelPtr->RadiusCollider;
	vector<string> checkedZona = vector<string>();

	glm::vec3 pos;
	//clear old  position in zona
	for(int i=0;i<=5;i++)
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
	r1 = objectMe->ModelPtr->RadiusCollider;

	x2 = object2->Postranslate.x;
	y2 = object2->Postranslate.z;
	r2 = object2->ModelPtr->RadiusCollider;

	double r = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); //���������� ����� ��������
	// �������� �� ���������� 2-�� �����������
	if (r == 0 && r1 == r2)
		return true;

	if (r1 + r2 >= r && r1 + r >= r2 && r2 + r >= r1)
		return true;
	else
		return false;
}


bool WorldCluster::IsCollisionObject(int indexObjMe, bool isNewPosition)
{
	vector<int> indObjsInSecor = GetSectorObjects(indexObjMe, isNewPosition);
	std::shared_ptr <ObjectData> objNext;
	for (const auto& nextIndObj : indObjsInSecor) 
	{
		if (nextIndObj == indexObjMe)
			continue;
		objNext = Storage->GetObjectPrt(nextIndObj);
		if (IsCollisionCircle(indexObjMe, objNext->Index, isNewPosition))
			return true;
	}
	return false;
}



