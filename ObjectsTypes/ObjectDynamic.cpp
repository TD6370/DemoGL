
//---------------------
#include "..\WorldCollision.h"
#include "..\ModelData.h"
#include "..\GeometryLib.h"
//------------------------

//-------------------
#include "ObjectDynamic.h"

void ObjectDynamic::RunAction() {

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

void ObjectDynamic::ActionMoving()
{
	ChekNextWay();
	if (ActionObjectCurrent != Moving)
		return;

	CalculateNextPosition();

	if (CheckIsLock())
		return;
	else
		SaveNewPosition();
}

void ObjectDynamic::ActionLook() {
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

void ObjectDynamic::ActionSearch() {

	vec3 newTarget;
	if (!CalculateTatget(newTarget))
		return;

	bool isLock = IsContactWorldBorder(newTarget);
	if (isLock)
		return;
	NewPostranslate = newTarget;
	isLock = IsCollisionObject(Index, true);
	if (isLock) {
		NewPostranslate = Postranslate;
		if (TypeObj == Hero)
			SaveHeroOperator();
		return;
	}
	Target = newTarget;

	vec3 ray = normalize(Target - Postranslate);
	TargetAngle = glm::atan(ray.x, ray.z) + m_angleModel;

	if (TypeObj == Hero)
		ActionObjectCurrent = Moving; //Type 2.
	if (TypeObj == NPC)
		ActionObjectCurrent = Look;
}
//----------

void ObjectDynamic::ChekNextWay()
{
	//GLuint dist = glm::distance(Target, Postranslate);
	GLuint dist = glm::distance(vec2(Target.x, Target.z), vec2(Postranslate.x, Postranslate.z));
	if (dist <= 2 + 0.1) {
		switch (TypeObj)
		{
		case Bullet:
			ActionObjectCurrent = Stay;
			break;
		case Hero:
			break;
		default:
			ActionObjectCurrent = Search;
			break;
		}
	}
}


void ObjectDynamic::CalculateNextPosition() {
	switch (TypeObj)
	{
		//case Bullet:
		//case NPC:
		//	break;
	case Hero:
		NewPostranslate = Target; //Vers 2.
		ActionObjectCurrent = Search; //Type 2.
		break;
	default:
		Move = normalize(Target - Postranslate) * Speed;
		NewPostranslate = Postranslate + Move;
		break;
	}
}

void ObjectDynamic::Pathfinding() {

	if (TypeObj == Hero)
	{
		SaveHeroOperator();
		Target = Postranslate;
		return;
	}
	if (TypeObj == NPC)
	{
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
		return;
	}
}

bool ObjectDynamic::CalculateTatget(vec3& resultTarget) {

	if (TypeObj == Hero)
	{
		if (Move == GetOperatorPosition()) //Move to hero
			return false;
		Move = GetOperatorPosition();
		resultTarget = vec3(Move.x, Postranslate.y, Move.z);
	}
	if (TypeObj == NPC)
	{
		World WorldSetting;
		GLfloat x = linearRand(minDist, DistanceTarget);
		GLfloat z = linearRand(minDist, DistanceTarget);
		int invers = linearRand(0, 1);
		if (invers == 1)
			x *= -1;
		invers = linearRand(0, 1);
		if (invers == 1)
			z *= -1;
		Move = vec3(x, 0, z);
		resultTarget = Postranslate + Move;
	}

	return true;
}

void ObjectDynamic::LockResult() {
	Pathfinding();
}

void ObjectDynamic::SaveNewPosition()
{
	Color = vec3(0);
	Storage->Clusters->SaveClusterObject(Index);
	Postranslate = NewPostranslate;
}