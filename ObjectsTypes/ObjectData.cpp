
//---------------------
//#include "..\CreatorModelData.h"
#include "..\WorldCollision.h"
//#include "..\CoreSettings.h"
#include "..\ModelData.h"
#include "..\GeometryLib.h"
//------------------------
#include "ObjectData.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::shared_ptr;

ObjectData::ObjectData() {};

ObjectData::ObjectData(int p_index,
	std::shared_ptr<ModelData> p_model,
	TypeObject p_typeObj,
	vec3 p_pos)
{
	Index = p_index;
	TypeObj = p_typeObj;
	Postranslate = p_pos;
	ModelPtr = p_model;
	m_keyPosSectorStr = "";
	PlaneDownIndex = -1;
}

ObjectData::~ObjectData()
{

}

void ObjectData::InitData()
{
	switch (TypeObj)
	{
		case Block:
		case Tree:
		case Terra:
		case Bullet:
			ActionObjectCurrent = Stay;
			break;
		default:
			ActionObjectCurrent = Lock;
			break;
	}

}

void ObjectData::RunAction() {

	if (ActionObjectCurrent != Lock)
	{
		switch (ActionObjectCurrent)
		{
			case Starting:
				GenStartPosition();
				break;
			case Stay:
				break;
			default:
				break;
		}
	}
	RunTransform();
}

void ObjectData::GenStartPosition() {
	World WorldSetting;
	int distanceSpawnZone = WorldSetting.Radius - 5;
	int x = linearRand(distanceSpawnZone * -1, distanceSpawnZone);
	int z = linearRand(distanceSpawnZone * -1, distanceSpawnZone);
	Postranslate = vec3(x, -50, z);
	NewPostranslate = vec3(x, -50, z);

	CheckStartPosition();
}

void ObjectData::CheckStartPosition() {

}

vec3 ObjectData::ToWorldPosition(vec3 pos) {
	glm::mat4 trans = Transform(1, 0, false,
		glm::mat4(1.0f),
		Postranslate,
		TranslateAngle);
	glm::vec3 worldPos = vec3(trans * vec4(pos, 1));
	return worldPos;
}

string ObjectData::GetKeySectorPolygon(bool isNewPosition) {
	vec3 pos = vec3();
	if (isNewPosition)
		pos = NewPostranslate;
	else
		pos = Postranslate;

	int x_sector = pos.x / Storage->Clusters->SectorSizePlane;
	int z_sector = pos.z / Storage->Clusters->SectorSizePlane;
	return std::to_string(x_sector) + "_" + std::to_string(z_sector);
}

void ObjectData::RunTransform()
{
	glm::mat4 trans = Transform(1, Storage->Camera->paramCase, false,
		glm::mat4(1.0f),
		Postranslate,
		TranslateAngle);
	//---------------------------------------------------
	ModelPtr->ConfUniform.SetTransform(&trans);
}

void ObjectData::Action()
{
	RunAction();
}

glm::vec3 ObjectData::GetVertexPosition(int indVertex)
{
	std::vector< glm::vec3 > verticesModel = ModelPtr->Vertices;
	//if (std::find(verticesModel.begin(), verticesModel.end(), indVertex) == verticesModel.end)
	if (verticesModel.size() - 1 < indVertex)
		return vec3(-1);

	glm::vec3 posNorm = verticesModel[indVertex];
	glm::mat4 trans = Transform(1, 0, false,
		glm::mat4(1.0f),
		Postranslate,
		TranslateAngle);
	glm::vec3 posVert = vec3(trans * vec4(posNorm, 1));
	return posVert;
}

shared_ptr<Plane> ObjectData::GetPlaneFromVertIndex(int indexVertPlane) {
	
	return Planes[(int)indexVertPlane / 3];
}

shared_ptr<Plane> ObjectData::GetPlanePrt(int indexPlane) {

	return Planes[indexPlane];
}





