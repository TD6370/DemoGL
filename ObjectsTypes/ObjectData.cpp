
//---------------------
//#include "../CreatorModelData.h"
#include "../WorldCollision.h" //--<< #include "CreatorModelData.h"  
//#include "../CoreSettings.h"
#include "../ModelData.h"
#include "../GeometryLib.h"
//------------------------
#include "ObjectData.h"
#include "../Serialize/SceneSerialize.h"
//#include "../GeomertyShapes/ShapeBase.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <iostream>
//#include <fstream>
#include <string>
#include <vector>

using std::shared_ptr;

ObjectData::ObjectData() {
};

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

	Shape = new ShapeBase();
	Shape->UpdateShapeInfo(this);
	
	/*Shape = new ShapeBase();
	Shape->Obj = std::shared_ptr<ObjectData>(this);
	Shape->Obj = this;*/
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
	//TextureUV = ModelPtr->UV;
}

void ObjectData::UpdateState() {

}

void ObjectData::ActionBase() {
	ControlsEvents();
	RunTransform();
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
	ActionBase();
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

//string ObjectData::GetKeySectorPolygon(bool isNewPosition) {
//	vec3 pos = vec3();
//	if (isNewPosition)
//		pos = NewPostranslate;
//	else
//		pos = Postranslate;
//
//	int x_sector = pos.x / Storage->Clusters->SectorSizePlane;
//	int z_sector = pos.z / Storage->Clusters->SectorSizePlane;
//	return std::to_string(x_sector) + "_" + std::to_string(z_sector);
//}

void ObjectData::RunTransform()
{
	glm::mat4 trans = Transform(1, Storage->Inputs->ParamCase, false,
		glm::mat4(1.0f),
		Postranslate,
		TranslateAngle,
		Size);

	TransformResult = trans;
	//---------------------------------------------------
	ModelPtr->ConfUniform.SetTransform(&trans);
}

void ObjectData::Action()
{
	RunAction();
}

/*
glm::vec3 ObjectData::GetVertexPosition(int indVertex)
{
	return ShapeObj->GetVertexPosition(indVertex);
}
*/

shared_ptr<Plane> ObjectData::GetPlaneFromVertIndex(int indexVertPlane) {
	
	//return Planes[(int)indexVertPlane / 3];
	return Shape->Planes[(int)indexVertPlane / 3];
}

shared_ptr<Plane> ObjectData::GetPlanePrt(int indexPlane) {

	//return Planes[indexPlane];
	return Shape->Planes[indexPlane];
}


std::vector<glm::vec3> ObjectData::GetVertices() {
	return ModelPtr->Vertices;
}

std::vector<glm::vec2> ObjectData::GetUV() {
	return ModelPtr->UV;
}

std::vector<glm::vec3> ObjectData::GetNormals() {

	if (Normals.size() != 0) 
		return Normals;
	else
		return ModelPtr->Normals;
}


void ObjectData::SetMesh() {
	ModelPtr->SetVAO();
}

void ObjectData::MeshTransform() {
}

void ObjectData::SelectedEvent() {
}

void ObjectData::UnselectedEvent() {
}

void ObjectData::UpdateTextureUV() {
	if (TextureUV.size() == 0)
		return;
	ModelPtr->SetUV(TextureUV);
}

void ObjectData::ControlsEvents() {
}

//void ObjectData::Billboard() {
//
//	TranslateAngle.y = Storage->Oper->HorizontalAngle + (0.5 * M_PI);
//	TranslateAngle.z = -Storage->Oper->VerticalAngle;
//}

bool ObjectData::GetVisible() {
	return IsVisible;
}

void ObjectData::AddChild(string key)
{
	ChildObjects.push_back(key);
}

std::shared_ptr<ObjectData> ObjectData::GetChild(string key)
{
	return Storage->GetObjectPrt(key);
}

void ObjectData::InitChildObjects() {

}

string ObjectData::GetInfo() {

	SceneSerialize* sceneSerialize = new SceneSerialize();
	string typeName = sceneSerialize->GetNameType(TypeObj);

	std::stringstream ss;
	ss << "Name: " << Name << " T:" + typeName;
	return ss.str();
}

//------ #SaveFieldSpecific

vector<ObjectFiledsSpecific> ObjectData::GetSpecificFiels() {

	vector<ObjectFiledsSpecific> result;
	return result;
}

void ObjectData::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {


}