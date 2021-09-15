
#include "ShapeSquare.h"

#include "../CoreSettings.h"
#include "../ModelData.h"
#include "../GeometryLib.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../CreatorModelData.h"
#include "../WorldCollision.h" //--<< #include "CreatorModelData.h"  
#include "../ObjectsTypes/ObjectPhysic.h"
#include "../Components/RenderComponent.h"
#include "../Components/GUIComponent.h"

ShapeSquare::~ShapeSquare() {

}

void ShapeSquare::SetOwnerPosition(vec2 sizeBackground, vec3 startPosParent) {

	m_sizeBackground = sizeBackground;

	if (!m_obj->IsAbsolutePosition) {
		if (m_firstPosParent == vec3(-5000))
			m_firstPosParent = startPosParent;
		if (m_firstPos == vec3(-5000))
			m_firstPos = m_obj->StartPos;
		m_offsetPosParent = m_firstPosParent - startPosParent;
	}

	m_startPosParent = startPosParent;
}		


void ShapeSquare::FormUpdate(bool isForce) {
	
	if (!isForce && !m_obj->EngineData->SceneData->IsGUI)
		return;

	//--- cash state Posititon
	if (m_obj->ActionObjectCurrent == Stay)
	{
		float cash_owner = 0;
		if(m_obj->IndexObjectOwner != -1)
			cash_owner = m_obj->OwnerObj->Postranslate.x + m_obj->OwnerObj->Postranslate.y;

		float currCash = m_obj->EngineData->Oper->HorizontalAngle + m_obj->EngineData->Oper->VerticalAngle + (m_obj->Postranslate.x + m_obj->Postranslate.y) + cash_owner;
		if (cash_stateOperator == currCash)
			return;
		cash_stateOperator = currCash;
	}

	vec3 directionOut = vec3(0);
	m_obj->ComponentGUI->PanelDepth = 3.8;
	int indexBackground = m_obj->EngineData->SceneData->IndexBackgroundGUIObj;
	float leftBackFactor = 1 + (m_sizeBackground.x - BACKGROUND_GUI_WIDTH_F);	//2.7
	//leftBackFactor = 1; // --- base box

	if (m_obj->IsAbsolutePosition)
	{
		vec2 offsetOfCenter = vec2(0);
		
		int indOwner = m_obj->IndexObjectOwner;
		if (indOwner == -1)
			indOwner = m_obj->EngineData->SceneData->IndexBackgroundGUIObj;
		if (indOwner != -1)
		{
			//---- normalize position on center backgruong control
			float lenghtLine = m_obj->Shape->GetLineLenght(0);
			offsetOfCenter = vec2(leftBackFactor - (lenghtLine / 2), 1 - (lenghtLine / 2));
			if (indOwner != indexBackground)
			{
				//---- normalize position on parent control
				offsetOfCenter.x -= m_startPosParent.x;
				offsetOfCenter.y -= m_startPosParent.y;
			}
		}

		vec3 startPos = vec3(m_obj->StartPos.x - offsetOfCenter.x, m_obj->StartPos.y - offsetOfCenter.y, m_obj->StartPos.z); //when SetSizeControl - is disabled
		m_obj->Postranslate = m_obj->NewPostranslate = GetVectorForwardFaceOffset(m_obj->EngineData->ConfigMVP,
			m_obj->ComponentGUI->PanelDepth - m_obj->StartPos.z,
			m_obj->EngineData->Oper, startPos);
	}
	else {
		if (m_obj->StartPos == vec3(0)) {
			m_obj->Postranslate = m_obj->NewPostranslate = GetVectorForwardFace(m_obj->EngineData->ConfigMVP, m_obj->ComponentGUI->PanelDepth, m_obj->EngineData->Oper);
		}
		else {
			if (m_obj->IndexObjectOwner != -1 && m_obj->IndexObjectOwner != indexBackground && m_firstPos != vec3(-5000))
			{
				m_obj->StartPos.x = m_firstPos.x - m_offsetPosParent.x;
				m_obj->StartPos.y = m_firstPos.y - m_offsetPosParent.y;
			}
			float lbFactor = leftBackFactor - 1;
			vec3 startPos = vec3(m_obj->StartPos.x - lbFactor, m_obj->StartPos.y, m_obj->StartPos.z);
			m_obj->Postranslate = m_obj->NewPostranslate = GetVectorForwardFaceOffset(m_obj->EngineData->ConfigMVP, m_obj->ComponentGUI->PanelDepth - startPos.z, m_obj->EngineData->Oper, startPos);
		
		}
	}
	
	SavePosFactor(m_obj->StartPos, m_obj->Postranslate);

	Billboard();
}

void ShapeSquare::SetSizeControl(vec3 vertOffset) {

	if (m_obj->IsAbsolutePosition)
		return;

	SaveSizeFactor(true);

	//-- set transform
	if (m_obj->MeshData.Vertices.size() != 0) {

		if (start_vertBottomLeft == vec3(0)) {
			start_vertBottomLeft = GetBottom(1);
			start_vertBottomRight = GetBottom(0);
			start_vertTopLeft = GetTop(1);
		}
		float offsetY = 2 - vertOffset.y;
		float offsetX = 2 - vertOffset.x;

		vec3 vertTopLeft = vec3(start_vertTopLeft.x, (start_vertTopLeft.y), start_vertTopLeft.z + offsetX);
		SetTop(1, vertTopLeft);

		vec3 vertBottomLeft = vec3(start_vertBottomLeft.x, (start_vertBottomLeft.y + offsetY), start_vertBottomLeft.z);
		SetBottom(1, vertBottomLeft);

		vec3 vertBottomRight = vec3(start_vertBottomRight.x, (start_vertBottomRight.y + offsetY), start_vertBottomRight.z + offsetX);
		SetBottom(0, vertBottomRight);

		m_obj->Render->ResetMem_Vertex();
	}

	SaveSizeFactor();
}

void ShapeSquare::SaveSizeFactor(bool isInit) {

	if (WidthFactor < 0)
	{
		if (isInit) {
			m_startWightLenght = GetLineLenght(0);
			m_startHeightLenght = GetLineVertLenght(0);
			m_lastWightLenght = m_startWightLenght;
			m_lastHeightLenght = m_startHeightLenght;
			return;
		}
		WidthFactor = 1;
		HeightFactor = 1;
		return;
	}
	if (isInit)
		return;

		//Save factor size
	float endWightLenght = GetLineLenght(0);
	float endHeightLenght = GetLineVertLenght(0);
	//----------------- PosMoveFactor
	PosMoveSizeFactor.x = endWightLenght - m_lastWightLenght;
	PosMoveSizeFactor.y = endHeightLenght - m_lastHeightLenght;

	m_lastWightLenght = endWightLenght;
	m_lastHeightLenght = endHeightLenght;
	//-----------------

	WidthFactor = m_startWightLenght / endWightLenght;
	HeightFactor = m_startWightLenght / endHeightLenght;
	WidthFactor = glm::abs(WidthFactor);
	HeightFactor = glm::abs(HeightFactor);
}

void ShapeSquare::SavePosFactor(vec3 posGUI, vec3 posWorld) {

	if (posWorld == vec3(0) || posGUI == vec3(0))
	{
		m_lastPosGUI = posGUI;
		m_lastPosWorld = posWorld;
		return;
	}

	vec3 posMoveFactor = m_lastPosWorld - posWorld;
	if(posMoveFactor != vec3(0))
		PosMoveFactor = posMoveFactor;

	m_lastPosGUI = posGUI;
	m_lastPosWorld = posWorld;
	
}

void ShapeSquare::ResizeTextureUV() {
	
	if (m_obj->IsTextureRepeat) {
		std::vector< glm::vec2 > repeat_UV = m_obj->ModelPtr->MeshData.UV;
		for (auto& uv : repeat_UV) {
			uv.y *= m_obj->TextureRepeat;
		}
		m_obj->MeshData.UV = repeat_UV;
		m_obj->Render->ResetMem_UV();
	}
}