#include "..\WorldCollision.h"

#include "ObjectHero.h"

void ObjectHero::InitData()
{
	ActionObjectCurrent = Search;
	IsGravity = true;
	Storage->SceneParam->IndexHeroObj = Index;

	LoadCursorRay();
}

void ObjectHero::LoadCursorRay()
{
	std::shared_ptr<ModelData> model = Storage->GetModelPrt("cursorRay");
	auto obj = Storage->AddObject("CursorRay", model, CursorRay, vec3(-50, -55, 70));
	obj->IndexObjectOwner = Index;
}

void ObjectHero::SaveHeroOperator(bool onlyVertical) {
	float heroHeight = 1.5;
	float steps = 3;

	//return;

	//Object to Operator
	if (onlyVertical) {
		float opDown = Storage->Operator->m_position.y - heroHeight;
		float speed = (NewPostranslate.y - opDown) / steps;
		float newPosOperatorY = opDown + speed;
		
		//Storage->Operator->m_position.y = NewPostranslate.y + heroHeight;
		Storage->Operator->m_position.y = newPosOperatorY + heroHeight;
	}
	else {
		vec3 stepsV = vec3(steps);
		vec3 opPos = Storage->Operator->m_position;
		opPos.y -= heroHeight;
		vec3 speed = (NewPostranslate - opPos) / stepsV;
		vec3 newPosOperator = opPos + speed;

		//Storage->Operator->m_position = vec3(NewPostranslate.x, NewPostranslate.y + heroHeight, NewPostranslate.z);
		Storage->Operator->m_position = vec3(newPosOperator.x, newPosOperator.y + heroHeight, newPosOperator.z);
	}
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

	//Operator to Object
	//Vers 2.
	NewPostranslate = Target; 
	ActionObjectCurrent = Search;
}

void ObjectHero::TargetCompleted()
{
}
