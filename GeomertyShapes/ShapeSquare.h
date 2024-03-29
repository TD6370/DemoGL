
#pragma once
#include "ShapeBase.h"

using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::string;

//class ShapeBase;


class ShapeSquare : public ShapeBase
{
private:
	string hyst_posit = "";
	vec3 tmp_posit = vec3();
	vec3 start_vertBottomLeft = vec3(0);
	vec3 start_vertBottomRight = vec3(0);
	vec3 start_vertTopLeft = vec3(0);
	float m_startWightLenght = 0;
	float m_startHeightLenght = 0;

	float m_lastWightLenght = 0;
	float m_lastHeightLenght = 0;
	vec3 m_lastPosGUI = vec3(0);
	vec3 m_lastPosWorld = vec3(0);

	vec2 m_sizeBackground = vec2(-1);
	vec3 m_startPosParent = vec3();

	// -- move to parent for not Absolute
	vec3 m_firstPosParent = vec3(-5000);
	vec3 m_firstPos = vec3(-5000);
	vec3 m_offsetPosParent = vec3();

	float cash_stateOperator = -1;

	void SaveSizeFactor(bool isInit = false);

	void SavePosFactor(vec3 posGUI, vec3 posWorld);
	

public:
	float WidthFactor = -1;
	float HeightFactor = -1;
	vec3 PosMoveFactor = vec3();
	vec3 PosMoveSizeFactor = vec3();

	bool IsNeedCalculateForm = false;
	
	ShapeSquare() :ShapeBase() {};

	~ShapeSquare();

	void FormUpdate(bool isForce = false);

	void SetOwnerPosition(vec2 sizeBackground, vec3 startPosParent);

	void SetSizeControl(vec3 vertOffset);

	void ResizeTextureUV();
};


