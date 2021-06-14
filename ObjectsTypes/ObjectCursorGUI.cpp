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
	if (ActionObjectCurrent != Moving)
		return;

	SaveNewPosition();
}

void ObjectCursorGUI::SaveNewPosition() {

	//--- Set position Cursor
	vec3 mouse = Storage->Oper->PositionCursorWorld;
	vec2 startPosRect = GetStartPositWorld();

	bool notX = false;
	bool notY = false;
	float k = 0.2;
	
	if (mouse.z > 0 || startPosRect.x != startPosRect.x)
	{
		return;
	}

	//=========================================
	float stepFactorX = mouse.x / startPosRect.x;
	float stepFactorY = mouse.y / startPosRect.y;

	float stepX = mouse.x - startPosRect.x;
	float stepY = startPosRect.y - mouse.y;
	stepX *= k;
	stepY *= k;

	float limit = 0.001;
	if (stepX <limit && stepX > -limit && stepY < limit && stepY > -limit) {

		return;
	}

	float limitBordMin = 0.01;
	float limitBordMax = 1.98;
	float corrBord = 0.02;
	if (StartPos.x + stepX < limitBordMin) {
		StartPos.x = limitBordMin + corrBord;
		notX = true;
	}
	if (StartPos.x + stepX > limitBordMax) {
		StartPos.x = limitBordMax - corrBord;
		notX = true;
	}
	if (StartPos.y + stepY < limitBordMin) {
		StartPos.y = limitBordMin + corrBord;
		notY = true;
	}
	if (StartPos.y + stepY > limitBordMax) {
		StartPos.y = limitBordMax - corrBord;
		notY = true;
	}

	if (!notX)
		StartPos.x += stepX;
	if (!notY)
		StartPos.y += stepY;
}

/*
void ObjectCursorGUI::SaveNewPosition() {

	vec3 mouse = Storage->Oper->PositionCursorWorld;
	vec2 mouseView = vec2(Storage->Oper->m_MouseX, Storage->Oper->m_MouseY);
	float zOrder;
	vec2 endPosRect, startPosRect;
	GetPositRect(startPosRect, endPosRect, zOrder);

	bool isReturn = false;
	bool notX = false;
	bool notY = false;

	if (StartPos.x < 0.01) {
		StartPos.x = 0.05;
		notX = true;
	}
	if (StartPos.x > 1.9) {
		StartPos.x = 1.85;
		notX = true;
	}
	if (StartPos.y < 0.01) {
		StartPos.y = 0.05;
		notY = true;
	}
	if (StartPos.y > 1.9) {
		StartPos.y = 1.85;
		notY = true;
	}

	//-------- Debug
	stringstream ssDebug;
	ssDebug << "Mouse View " << mouseView.x << " x " << mouseView.x << "\n"
		<< "Mouse World: " << mouse.x << " x " << mouse.y << "  z:" << mouse.z << "\n"
		<< "Obj Abs: " << startPosRect.x << " x " << startPosRect.y << "\n"
		<< "Obj Move: " << StartPos.x << " x " << StartPos.y << "\n"
		<< "----------------------------------------" << "\n";
	string strTemp = ssDebug.str();
	if (m_stringDebug != strTemp) {
		m_stringDebug = strTemp;
		std::cout << m_stringDebug;
	}
	//---------------------

	if (mouse.z > 0)
	{
		return;
	}
	if (startPosRect.x != startPosRect.x) {
		StartPos = vec3(.15, .15, .01);
		return;
	}

	float k = .2;
	float limitRect = 5.;
	if (startPosRect.x > limitRect || startPosRect.x < -limitRect)
		return;
	if (startPosRect.y > limitRect || startPosRect.y < -limitRect)
		return;

	//----
	float stepX = mouse.x - startPosRect.x;
	float stepY = startPosRect.y - mouse.y;

	stepX *= k;
	stepY *= k;
	//----

	//float limit = 0.001;
	//if (stepX <limit && stepX > -limit && stepY < limit && stepY > -limit)
	//	return;

	//if (StartPos.x + stepX <limit && StartPos.x + stepX > -limit && StartPos.y + stepY < limit && StartPos.y + stepY > -limit)
	//	return;

	if (StartPos.x + stepX < 0.01) {
		StartPos.x = 0.05;
		notX = true;
	}
	if (StartPos.x + stepX > 1.9) {
		StartPos.x = 1.89;
		notX = true;
	}
	if (StartPos.y + stepY < 0.01) {
		StartPos.y = 0.05;
		notY = true;
	}
	if (StartPos.y + stepY > 1.9) {
		StartPos.y = 1.89;
		notY = true;
	}

	//--------
	if (!notX)
		StartPos.x += stepX;
	if (!notY)
		StartPos.y += stepY;
}
*/