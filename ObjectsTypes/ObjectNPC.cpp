#include "ObjectNPC.h"


void ObjectNPC::InitData()
{
	ObjectData::InitData();

	ActionObjectCurrent = Starting;
	IsGravity = true;
}

void ObjectNPC::Pathfinding() {

	GLfloat newTranslateAngle = 0.5f;

	GLfloat y = Target.y;
	vec3 vecR = Target - Postranslate;
	Target.x = vecR.x * cos(newTranslateAngle) - vecR.z * sin(newTranslateAngle);
	Target.z = vecR.x * sin(newTranslateAngle) + vecR.z * cos(newTranslateAngle);
	Target = Target + Postranslate;
	Target.y = y;

	vec3 ray = normalize(Target - Postranslate);
	TargetAngle.y = glm::atan(ray.x, ray.z) + m_angleModel;

	ActionObjectCurrent = Look;
	return;
}

bool ObjectNPC::CalculateTatget(vec3& resultTarget) {

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

	return true;
}

void ObjectNPC::TargetCompleted()
{
	ActionObjectCurrent = Search;
}

