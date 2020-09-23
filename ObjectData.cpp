
//---------------------
#include "CreatorModelData.h"
#include "WorldCollision.h"
#include "CoreSettings.h"
#include "ModelData.h"
#include "ObjectData.h"
#include "GeometryLib.h"
//------------------------

#include "ObjectData.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::shared_ptr;

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

	switch (TypeObj)
	{
	case Polygon:
		ActionObjectCurrent = Lock;
		FillPlanes();
		break;
	case Block:
		ActionObjectCurrent = Stay;
		break;
	case Tree:
		ActionObjectCurrent = Stay;
		break;
	case Terra:
		ActionObjectCurrent = Stay;
		break;
	case NPC:
		ActionObjectCurrent = Starting;
		break;
	case Bullet:
		ActionObjectCurrent = Stay;
		break;
	default:
		ActionObjectCurrent = Lock;
		break;
	}

}

ObjectData::~ObjectData()
{

}

void ObjectData::ChekNextWay()
{
	//GLuint dist = glm::distance(Target, Postranslate);
	GLuint dist = glm::distance(vec2(Target.x, Target.z), vec2(Postranslate.x, Postranslate.z));
	if (dist <= 2 + 0.1) {
		switch (TypeObj)
		{
			case Bullet:
				ActionObjectCurrent = Stay;
				break;
			default:
				ActionObjectCurrent = Search;
				break;
		}
	}
}

bool ObjectData::IsContactWorldBorder(vec3 pos) {

	World WorldSetting;
	bool result = false;
	if (pos.x > WorldSetting.Radius || pos.x < -WorldSetting.Radius || pos.z > WorldSetting.Radius || pos.z < -WorldSetting.Radius)
		result = true;
	return result;
}

void ObjectData::ActionMoving()
{
	ChekNextWay();
	if (ActionObjectCurrent != Moving)
		return;

	Move = normalize(Target - Postranslate) * Speed;
	NewPostranslate = Postranslate + Move;
	if (CheckIsLock()) {
		// --- test
		//ActionObjectCurrent = Search;
		return;
	}

	//------------------- TEST -------------------
	//return;

	Color = vec3(0);
	Storage->Clusters->SaveClusterObject(Index);
	Postranslate = NewPostranslate;
}

bool ObjectData::CheckIsLock() {

	if (TypeObj == Bullet) //CHILD CLASS
		return false;

	bool isPolygon = CheckIsPolygon();

	bool isNotValidMove = IsContactWorldBorder(NewPostranslate);
	if (!isNotValidMove)
		isNotValidMove = Storage->Clusters->IsCollisionObject(Index, true);
	if (isNotValidMove)
	{
		Color = vec3(1, 0, 0);

		if (ActionObjectCurrent == Moving) {
			//------------------
			GLfloat newTranslateAngle = 0.5f;

			GLfloat y = Target.y;
			vec3 vecR = Target - Postranslate;
			Target.x = vecR.x * cos(newTranslateAngle) - vecR.z * sin(newTranslateAngle);
			Target.z = vecR.x * sin(newTranslateAngle) + vecR.z * cos(newTranslateAngle);
			Target = Target + Postranslate;
			Target.y = y;

			vec3 ray = normalize(Target - Postranslate);
			TargetAngle = glm::atan(ray.x, ray.z) + m_angleModel;

			ActionObjectCurrent = Look;
		}
		return true;
	}
	else {
		return false;
	}
	return true;
}

void ObjectData::ActionLook() {
	float rangeLook = 0.1f;
	if (TargetAngle - rangeLook < TranslateAngle && TargetAngle + rangeLook > TranslateAngle) {
		NewPostranslate = vec3(0);
		ActionObjectCurrent = Moving;
	}

	if (ActionObjectCurrent != Look)
		return;

	if (TargetAngle > TranslateAngle)
		TranslateAngle += m_speedRotate;
	else
		TranslateAngle -= m_speedRotate;
}

void ObjectData::ActionSearch() {
	World WorldSetting;
	GLfloat x = linearRand(minDist, DistanceTarget);
	GLfloat z = linearRand(minDist, DistanceTarget);
	if (x > WorldSetting.Radius || x < -WorldSetting.Radius || z > WorldSetting.Radius || z < -WorldSetting.Radius)
		return;

	int invers = linearRand(0, 1);
	if (invers == 1)
		x *= -1;
	invers = linearRand(0, 1);
	if (invers == 1)
		z *= -1;

	Move = vec3(x, 0, z);
	vec3 newTarget = Postranslate + Move;
	bool isLock = IsContactWorldBorder(newTarget);
	if (isLock)
		return;
	NewPostranslate = newTarget;
	isLock = Storage->Clusters->IsCollisionObject(Index, true);
	if (isLock)
		return;
	Target = newTarget;

	vec3 ray = normalize(Target - Postranslate);
	TargetAngle = glm::atan(ray.x, ray.z) + m_angleModel;

	ActionObjectCurrent = Look;
}

void ObjectData::RunAction() {

	if (ActionObjectCurrent != Lock)
	{
		switch (ActionObjectCurrent)
		{
		case Starting:
				GenStartPosition();
				break;
			case Moving:
				ActionMoving();
				break;
			case Stay:
				break;
			case Look:
				ActionLook();
				break;
			case Search:
				ActionSearch();
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

	if (!CheckIsLock())
	{
		Storage->Clusters->SaveClusterObject(Index);
		Postranslate = NewPostranslate; 
		ActionObjectCurrent = Search;
	}
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

//--------------------------------------------------------PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
bool ObjectData::CheckIsPolygon() {

	//string keyPosSectorStr = GetKeySectorPolygon(true);
	float step = 0.3;
	Plane* plane = NULL;
	//vec3 vertex = vec3(-5000);
	vec4 vertex = vec4(-5000);

	CollisionPolygonState = Storage->Clusters->IsCollisionPolygon(Index, plane, vertex);

	switch (CollisionPolygonState)
	{
		case COLLISE_UP:
		{
			//Postranslate.y = NewPostranslate.y = (NewPostranslate.y + step);
			NewPostranslate.y = (NewPostranslate.y + step);
			break;
		}
		case COLLISE_DOWN:
			//Postranslate.y = NewPostranslate.y = (NewPostranslate.y - step);
			NewPostranslate.y = (NewPostranslate.y - step);
			break;
		case COLLISE_NORMAL:
			break;
		default:
			break;
	}

	return CollisionPolygonState == COLLISE_UP || CollisionPolygonState == COLLISE_NORMAL;
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

//------------------

void ObjectData::FillPlanes()
{
	//Planes = new vector<Plane>();
	int indVert = 0;
	int indexPlane = 0;
	//for (int indVert = 0; indVert < Vertices.size(); indVert+3)
	//for (const auto& vertex : Vertices) {

	glm::mat4 trans = Transform(1, 0, false,
		glm::mat4(1.0f),
		Postranslate,
		TranslateAngle);
	//glm::vec3 posVert = vec3(trans * vec4(posNorm, 1));


	while (indVert < ModelPtr->Vertices.size())
	{
		vec3 vertexNormA = ModelPtr->Vertices[indVert++];
		vec3 vertexNormB = ModelPtr->Vertices[indVert++];
		vec3 vertexNormC = ModelPtr->Vertices[indVert++];

		//glm::vec3 posA = vec3(trans * vec4(vertexNormA, 1));
		//glm::vec3 posB = vec3(trans * vec4(vertexNormB, 1));
		//glm::vec3 poCB = vec3(trans * vec4(vertexNormC, 1));
				
		Plane plane(vertexNormA, vertexNormB, vertexNormC, trans);
		plane.Index = indexPlane++;
		//Planes->push_back(plane);
		Planes.push_back(std::make_unique<Plane>(plane));
	}
}

shared_ptr<Plane> ObjectData::GetPlaneFromVertIndex(int indexVertPlane) {
	
	return Planes[(int)indexVertPlane / 3];
}


shared_ptr<Plane> ObjectData::GetPlanePrt(int indexPlane) {

	return Planes[indexPlane];
}





