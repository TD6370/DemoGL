//---------------------
#include "..\WorldCollision.h"
//-------------------
#include "ObjectDynamic.h"

void ObjectDynamic::InitData()
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

	IsGravity = true;
}

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

void ObjectDynamic::LockObjectResult() {

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
		LockObjectResult();
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

	Move = normalize(Target - Postranslate) * Speed;
	NewPostranslate = Postranslate + Move;
}

void ObjectDynamic::Pathfinding() {
}

bool ObjectDynamic::CalculateTatget(vec3& resultTarget) {
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