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
	//RunTransform();
	ObjectData::ActionBase();
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
	if (TargetAngle.y - rangeLook < TranslateAngle.y && TargetAngle.y + rangeLook > TranslateAngle.y) {
		NewPostranslate = vec3(0);

		ActionObjectCurrent = Moving;
	}

	if (ActionObjectCurrent != Look)
		return;

	if (TargetAngle.y > TranslateAngle.y)
		TranslateAngle.y += m_speedRotate;
	else
		TranslateAngle.y -= m_speedRotate;
}

void ObjectDynamic::LockObjectResult() {

}

void ObjectDynamic::ActionSearch() {

	int indObjHit = -1;
	vec3 newTarget;
	if (!CalculateTatget(newTarget))
		return;

	bool isLock = IsContactWorldBorder(newTarget);
	if (isLock)
		return;
	NewPostranslate = newTarget;
	isLock = IsCollisionObject(Index, indObjHit, true);
	if (isLock) {
		NewPostranslate = Postranslate;
		LockObjectResult();
		return;
	}
	Target = newTarget;

	vec3 ray = normalize(Target - Postranslate);
	TargetAngle.y = glm::atan(ray.x, ray.z) + m_angleModel;

	//<<!!!>>
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
	if (dist <= 2 + 0.1) 
		TargetCompleted();
}

void ObjectDynamic::TargetCompleted()
{
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
	SaveToCluster();
	Postranslate = NewPostranslate;
}

