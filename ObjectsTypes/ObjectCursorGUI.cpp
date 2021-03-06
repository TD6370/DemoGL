﻿#include "ObjectCursorGUI.h"
#include "../GeomertyShapes//ShapeBase.h"
#include "..\CreatorModelData.h"

ObjectCursorGUI::~ObjectCursorGUI()
{

}

void ObjectCursorGUI::InitData() {

	ObjectGUI::InitData();

	ActionObjectCurrent = Moving;
}

void ObjectCursorGUI::ActionWork() {

	ObjectGUI::ActionWork();
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
	vec2 startPosRect = Shape->GetStartPositWorld();

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