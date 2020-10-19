#include "..\WorldCollision.h"

#include "ObjectHero.h"

void ObjectHero::InitData()
{
	ActionObjectCurrent = Search;
}

void ObjectHero::SaveHeroOperator(bool onlyVertical) {
	float heroHeight = 1.5;
	if (onlyVertical)
		Storage->Operator->m_position.y = NewPostranslate.y + heroHeight;
	else
		Storage->Operator->m_position = vec3(NewPostranslate.x, NewPostranslate.y + heroHeight, NewPostranslate.z);
}

vec3 ObjectHero::GetOperatorPosition()
{
	return Storage->Operator->m_position;
}

void ObjectHero::LockObjectResult() {
	SaveHeroOperator();
}

void ObjectHero::LockPolygonResult() {
	SaveHeroOperator(true);
}

void ObjectHero::Pathfinding() {

	SaveHeroOperator();
	Target = Postranslate;
	return;
}

bool ObjectHero::CalculateTatget(vec3& resultTarget) {

	vec3 operatorPos = GetOperatorPosition();
	if (Move == operatorPos) //Move to hero
		return false;
	Move = operatorPos;
	resultTarget = vec3(Move.x, Postranslate.y, Move.z);

	return true;
}

void ObjectHero::CalculateNextPosition() {

	NewPostranslate = Target; //Vers 2.
	ActionObjectCurrent = Search; //Type 2.
}

