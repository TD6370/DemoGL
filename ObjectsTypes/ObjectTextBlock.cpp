#include "ObjectTextBlock.h"

#include "./ObjectData.h"
//#include "./ObjectGUI.h"
//#include "./ObjectPhysic.h"
#include "../CoreSettings.h"
#include "..\WorldCollision.h"
#include "..\ModelData.h"

using glm::vec3;
using glm::vec4;
using glm::vec2;
using std::map;
using std::string;
using std::shared_ptr;
glm::mat4;

void ObjectTextBlock::SetSizeControl(vec3 vertOffset) {

	/*
	//-- set transform
	if (Vertices.size() != 0) {


		vec3 vertBottomLeft = GetBottom(1);
		vec3 vertBottomRight = GetBottom(0);
		vec3 vertTopLeft = GetTop(1);
		vec3 vertTopRight = GetTop(0);

		float offsetY = 2 - vertOffset.y;
		float offsetX = 2 - vertOffset.x;

		float factor1 = vertTopLeft.y - vertBottomLeft.y;

		vertTopLeft = vec3(vertTopLeft.x, (vertTopLeft.y), vertTopLeft.z + offsetX);
		SetTop(1, vertTopLeft);

		vertBottomLeft = vec3(vertBottomLeft.x, (vertBottomLeft.y + offsetY), vertBottomLeft.z);
		SetBottom(1, vertBottomLeft);

		vertBottomRight = vec3(vertBottomRight.x, (vertBottomRight.y + offsetY), vertBottomRight.z + offsetX);
		SetBottom(0, vertBottomRight);

		// ---- Texture Repeat
		//vertBottomLeft = GetBottom(1);
		//vertBottomRight = GetBottom(0);
		//float factor2 = vertTopLeft.y - vertBottomLeft.y;
		////TextureRepeat = factor1 / factor2;
		//TextureRepeat = factor2 / factor1;

		//std::cout << TextureRepeat;

		// ---- update Texture UV
		//ResizeTextureUV();
	}
	*/
}

void ObjectTextBlock::GetPositRect(vec2& startPos, vec2& endPos, float& zOrder) {

	glm::mat4 MVP = Storage->ConfigMVP->MVP;
	glm::mat4 transform = TransformResult;
	vec3 vertBottomLeft = GetBottom(1);
	vec3 vertBottomRight = GetBottom(0);
	vec3 vertTopLeft = GetTop(1);
	vec3 vertTopRight = GetTop(0);
	vec3 posWorldBL = MVP * transform * vec4(vertBottomLeft, 1.0);
	vec3 posWorldBR = MVP * transform * vec4(vertBottomRight, 1.0);
	vec3 posWorldTL = MVP * transform * vec4(vertTopLeft, 1.0);
	vec3 posWorldTR = MVP * transform * vec4(vertTopRight, 1.0);
	zOrder = posWorldTR.z;
	startPos.x = posWorldBL.x,
		startPos.y = posWorldTL.y;
	endPos.x = posWorldBR.x;
	endPos.y = posWorldBL.y;
}

void ObjectTextBlock::UpdateState() {

	if (!Storage->SceneData->IsGUI)
		return;

	vec3 directionOut = vec3(0);
	PanelDepth = 3.8;

	vec3 startPos = vec3(StartPos.x - 0.97, StartPos.y - 0.97, StartPos.z); //when SetSizeControl - is disabled
	Postranslate = NewPostranslate = GetVectorForwardFaceOffset(Storage->ConfigMVP, PanelDepth - 0.01f, Storage->Oper, startPos);

	Billboard();
}


ObjectTextBlock::~ObjectTextBlock()
{

}

void ObjectTextBlock::InitData()
{
	ObjectGUI::InitData();
}


void ObjectTextBlock::RunAction() {

	ObjectGUI::RunAction();

}

