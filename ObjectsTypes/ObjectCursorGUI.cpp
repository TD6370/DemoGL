#include "ObjectCursorGUI.h"


ObjectCursorGUI::~ObjectCursorGUI()
{

}

void ObjectCursorGUI::InitData() {

	ObjectGUI::InitData();

	ActionObjectCurrent = Moving;
}

void ObjectCursorGUI::Work() {

	ObjectGUI::Work();
}

void ObjectCursorGUI::ActionMoving()
{
	//ChekNextWay();
	if (ActionObjectCurrent != Moving)
		return;

	CalculateNextPosition();

	SaveNewPosition();
}

void ObjectCursorGUI::CalculateNextPosition() {
	
	//Move = normalize(Target - Postranslate) * Speed;
	//NewPostranslate = Postranslate + Move;
	//NewPostranslate = Postranslate + Move;

	//NewPostranslate = Storage->Oper->PositionCursorWorld;

	//vec3 mouse = Storage->Oper->PositionCursorWorld;
	//NewPostranslate = vec3(mouse.x, mouse.y, Postranslate.z);

	//vec3 mouse = Storage->Oper->PositionCursorWorld;
	NewPostranslate = Storage->Oper->PositionCursorWorld;
}

void ObjectCursorGUI::SaveNewPosition() {

	glm::mat4 MVP = Storage->ConfigMVP->MVP;
	glm::mat4 transform = TransformResult;
	//vec3 posWorldBL =   MVP * transform * vec4(NewPostranslate, 1.0);
	//vec3 posWorld = glm::inverse(MVP) * glm::inverse(transform) * vec4(NewPostranslate, 1.0);
	vec3 posWorld = vec3(0);

	Postranslate = vec3(posWorld.x, posWorld.y, Postranslate.z);
	//Postranslate = posWorld;
	//Postranslate = NewPostranslate;
	//StartPos = Postranslate = NewPostranslate;

	//vec2 mouse = vec2(Storage->Oper->m_MouseX, Storage->Oper->m_MouseY);
	//StartPos = vec3(mouse.x, mouse.y, StartPos.z);
}
