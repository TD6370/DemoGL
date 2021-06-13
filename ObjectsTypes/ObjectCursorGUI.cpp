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
	//NewPostranslate = Storage->Oper->PositionCursorWorld;
}

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
	ssDebug << "Mouse View " << mouseView.x << " x " << mouseView.x  << "\n"
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
		//StartPos = vec3(.15, .15, .01);
		return;
	}
	if (startPosRect.x != startPosRect.x) {
		//startPosRect = vec2(0);
		//std::cout << "NAN !!!";
		StartPos = vec3(.15, .15, .01);
		return;
	}

	float k = .2;
	//float k = .39;
	//float k = 20.2;
	//float k = 0.7;
	float limitRect = 5.;
	if (startPosRect.x > limitRect || startPosRect.x < -limitRect)
		return;
	if (startPosRect.y > limitRect || startPosRect.y < -limitRect)
		return;

	//----
	//	float stepFactorX = mouse.x/startPosRect.x;
	//	float stepFactorY = mouse.y/startPosRect.y;
	//	//---------------------
	//	/*float stepFactorX = startPosRect.x / mouse.x;
	//	float stepFactorY = startPosRect.y / mouse.y;*/
	//	//---------------------
	//	//float stepFactorX = mouse.x / startPosRect.x;
	//	//float stepFactorY = startPosRect.y / mouse.y;
	//	//---------------------
	//	//float stepFactorX = startPosRect.x / mouse.x;
	//	//float stepFactorY = mouse.y / startPosRect.y;


	//float stepX = mouse.x - startPosRect.x;
	//float stepY = startPosRect.y - mouse.y;
	//	//float stepY = mouse.y - startPosRect.y;
	//	//stepX *= (stepFactorX * k);
	//	//stepY *= (stepFactorY * k);
	//stepX *= (std::abs(stepFactorX) * k);
	//stepY *= (std::abs(stepFactorY) * k);
	//	//stepX *= k;
	//	//stepY *= k; 
	//----
	float stepX = mouse.x - startPosRect.x;
	float stepY = startPosRect.y - mouse.y;

	//stepX = std::abs(stepX);
	//stepY = std::abs(stepY);

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
	
	//---------------
	/*if (!notX)
		StartPos.x *= stepFactorX;
	if (!notY)
		StartPos.y *= stepFactorY;*/
	//---------------
	if (!notX)
		StartPos.x += stepX;
	if (!notY)
		StartPos.y += stepY;
	//---------------

	/*vec2 vertOffset = vec2(0);
	if (mouse.x >= startPosRect.x)
		vertOffset.x = k;
	else
		vertOffset.x = -k;

	if (mouse.y <= startPosRect.y)
		vertOffset.y = k;
	else
		vertOffset.y = -k;

	StartPos.x += vertOffset.x;
	StartPos.y += vertOffset.y;*/

	//---------------
	
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