#pragma once
#include "ShapeBase.h"

class ShapeBase;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

class ShapeHexagon : public ShapeBase
{
private:

public:
	float StartLenghtWall = 1;

	ShapeHexagon() :ShapeBase() {};

	~ShapeHexagon();
	
	void SelectVertexBlock();

	void ResizeTextureUV();

	void ResizeVerticaleWall(int keyUpTopVertex, int keyDownTopVertex);

	void CalculateTextureUV(bool isInit);

};

