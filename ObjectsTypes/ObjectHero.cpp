#include "..\WorldCollision.h"
#include "../CreatorModelData.h" 

#include "ObjectHero.h"

void ObjectHero::InitData()
{
	ObjectData::InitData();

	ActionObjectCurrent = Search;
	IsGravity = true;
	EngineData->SceneData->IndexHeroObj = Index;
}

void ObjectHero::SaveHeroOperator(bool onlyVertical) {
	float heroHeight = 1.5;
	float steps = 3;

	//Object to Operator
	if (onlyVertical) {
		float opDown = EngineData->Oper->m_position.y - heroHeight;
		float speed = (NewPostranslate.y - opDown) / steps;
		float newPosOperatorY = opDown + speed;
		
		//Storage->Operator->m_position.y = NewPostranslate.y + heroHeight;
		EngineData->Oper->m_position.y = newPosOperatorY + heroHeight;
	}
	else {
		vec3 opPos = EngineData->Oper->m_position;
		opPos.y -= heroHeight;
		vec3 speed = (NewPostranslate - opPos) / steps;
		vec3 newPosOperator = opPos + speed;

		//Storage->Operator->m_position = vec3(NewPostranslate.x, NewPostranslate.y + heroHeight, NewPostranslate.z);
		EngineData->Oper->m_position = vec3(newPosOperator.x, newPosOperator.y + heroHeight, newPosOperator.z);
	}
}

vec3 ObjectHero::GetOperatorPosition()
{
	return EngineData->Oper->m_position;
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

	//Operator to Object
	//Vers 2.
	NewPostranslate = Target; 
	ActionObjectCurrent = Search;
}

void ObjectHero::TargetCompleted()
{
}
