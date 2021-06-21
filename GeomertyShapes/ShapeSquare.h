
#pragma once

#include "ShapeBase.h"
#include "../ObjectsTypes/ObjectPhysic.h"
#include "../ObjectsTypes/ObjectGUI.h"


using glm::vec4;
using glm::vec3;
using glm::vec2;
//using glm::mat4;
using std::string;
//using std::shared_ptr;
//using std::map;


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

	void SaveSizeFactor(bool isInit = false);

public:
	float WidthFactor = -1;
	float HeightFactor = -1;

	ShapeSquare() :ShapeBase() {};

	~ShapeSquare();

	void UpdateState();

	void SetSizeControl(vec3 vertOffset);

	void ResizeTextureUV();
};


