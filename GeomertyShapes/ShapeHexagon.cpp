
#include "ShapeHexagon.h"
//#include "ShapeBase.h"

#include "../CoreSettings.h"
#include "../ModelData.h"
#include "../GeometryLib.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../CreatorModelData.h"
#include "../WorldCollision.h" //--<< #include "CreatorModelData.h"  
#include "../ObjectsTypes/ObjectPhysic.h"
#include "../ObjectsTypes/ObjectGUI.h"	//##$$5.

ShapeHexagon::~ShapeHexagon() {

}

void ShapeHexagon::SelectVertexBlock() {

	ObjectPhysic* obj = m_objPhysic;
	vec3 posCursor;

	posCursor = obj->Target;

	int distMin = -1;
	for (int indV = 0; indV < 4; indV++) {
		vec3 vertBlock = GetBottom(indV);

		vertBlock += obj->Postranslate;
		int dist = glm::distance(vec2(posCursor.x, posCursor.z), vec2(vertBlock.x, vertBlock.z));

		if (distMin == -1 || distMin > dist) {
			distMin = dist;
			obj->IndexVertexTransform = indV;
		}
	}
}

void ShapeHexagon::ResizeTextureUV() {
	ObjectPhysic* obj = m_objPhysic;
	if (obj->IsTextureRepeat) {
		std::vector< glm::vec2 > repeat_UV = obj->ModelPtr->UV;
		for (auto& uv : repeat_UV) {
			uv.x *= obj->TextureRepeat;
		}
		obj->TextureUV = repeat_UV;
	}
}

void ShapeHexagon::ResizeVerticaleWall(int keyUpTopVertex, int keyDownTopVertex) {
	ObjectPhysic* obj = m_objPhysic;
	//resize Vertical wall
	if (obj->IndexVertexTransform == -1)
		return;

	float upSize = 1;
	if (obj->EngineData->Inputs->Key == keyUpTopVertex)
		upSize = 1;
	if (obj->EngineData->Inputs->Key == keyDownTopVertex)
		upSize = -1;

	vec3 vertSelect = GetTop(obj->IndexVertexTransform);
	vertSelect.y += upSize;;
	
	SetTop(obj->IndexVertexTransform, vertSelect);

	obj->SaveNewPosition();
}


void ShapeHexagon::CalculateTextureUV(bool isInit) {
	ObjectPhysic* obj = m_objPhysic;

	if (obj->IsTextureRepeat) {
		int factorRepeat = 1;
		float x1, x2, y1, y2;
		float maxLenght = -1;
		float lenghtLine = 0;

		for (int indLine = 0; indLine < 4; indLine++) {
			lenghtLine = GetLineLenght(indLine);
			if (lenghtLine > maxLenght) {
				maxLenght = lenghtLine;
			}
		}

		if (isInit) {
			StartLenghtWall = maxLenght; //start len resize wall -> max len wal
			obj->TextureUV = obj->ModelPtr->UV;
		}
		else
			factorRepeat = maxLenght / StartLenghtWall;

		obj->TextureRepeat = factorRepeat;

		if (!isInit)
			ResizeTextureUV();
	}
}