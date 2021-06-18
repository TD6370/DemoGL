
#include "ShapeHexagon.h"

ShapeHexagon::~ShapeHexagon() {

}

void ShapeHexagon::SelectVertexBlock() {

	ObjectPhysic* obj = m_objPhysic;
	vec3 posCursor;

	int indCursor = obj->Storage->SceneData->IndexCursorRayObj;
	std::shared_ptr<ObjectData> CusrorObj = obj->Storage->GetObjectPrt(indCursor);
	posCursor = CusrorObj->Postranslate;

	/*int indHero = Storage->SceneParam->IndexHeroObj;
	std::shared_ptr<ObjectData> HeroObj = Storage->GetObjectPrt(indHero);
	posCursor = HeroObj->Postranslate;*/

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
	if (obj->Storage->Inputs->Key == keyUpTopVertex)
		upSize = 1;
	if (obj->Storage->Inputs->Key == keyDownTopVertex)
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