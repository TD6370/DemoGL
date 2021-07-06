
//#include "ShapeBase.h"
#include "ShapeSquare.h"

#include "../CoreSettings.h"
#include "../ModelData.h"
#include "../GeometryLib.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../CreatorModelData.h"
#include "../WorldCollision.h" //--<< #include "CreatorModelData.h"  
#include "../ObjectsTypes/ObjectPhysic.h"
#include "../ObjectsTypes/ObjectGUI.h"	//##$$5.

ShapeSquare::~ShapeSquare() {

}


void ShapeSquare::FormUpdate(bool isForce) {
	ObjectGUI* obj = m_objGUI;
	if (!isForce && !obj->Storage->SceneData->IsGUI)
		return;

	//================================================== TEST
	/*std::stringstream ss;
	ss << "Name: " << Name << "  Model:" << ModelPtr->Name << "\n";
	ss << "SP: " << StartPos.x << " " << StartPos.y << "\n\n";
	ss << "PS: " << Postranslate.x << " " << Postranslate.y << " " << Postranslate.z << "\n";
	string spStart = ss.str();*/
	//==================================================

	vec3 directionOut = vec3(0);
	obj->PanelDepth = 3.8;
	int indexBackground = obj->Storage->SceneData->IndexGUIObj;

	if (obj->IsAbsolutePosition)
	{
		vec2 offsetOfCenter = vec2(0);
		//-----------------------------
		int indOwner = obj->IndexObjectOwner;
		if (indOwner == -1)
			indOwner = obj->Storage->SceneData->IndexGUIObj;
		if (obj->IndexObjectOwner != -1)
		{
			//---- normalize position on center backgruong control
			//auto objOwn = obj->Storage->GetObjectPrt(obj->IndexObjectOwner);
			auto objOwn = obj->Storage->GetObjectPrt(indexBackground);
			auto  objPh = std::dynamic_pointer_cast<ObjectPhysic>(objOwn);
			float lenghtLineOwner = objPh->Shape->GetLineLenght(0);
			float lenghtLine = obj->Shape->GetLineLenght(0);
			float offsetOwner = lenghtLineOwner / 2;
			float offsetAbs = lenghtLine / 2;
			offsetOfCenter = vec2(offsetOwner - offsetAbs);
			
			if (obj->IndexObjectOwner != indexBackground)
			{
				//---- normalize position on parent control
				objOwn = obj->Storage->GetObjectPrt(obj->IndexObjectOwner);
				auto  objGUI = std::dynamic_pointer_cast<ObjectGUI>(objOwn);
				offsetOfCenter.x -= objGUI->StartPos.x;
				offsetOfCenter.y -= objGUI->StartPos.y;
			}
		}

		vec3 startPos = vec3(obj->StartPos.x - offsetOfCenter.x, obj->StartPos.y - offsetOfCenter.y, obj->StartPos.z); //when SetSizeControl - is disabled
		obj->Postranslate = obj->NewPostranslate = GetVectorForwardFaceOffset(obj->Storage->ConfigMVP,
			obj->PanelDepth - obj->StartPos.z,
			obj->Storage->Oper, startPos);

		//================================================== TEST
		//TEST -- hystory move pos TextBox
		/*if (Postranslate != tmp_posit && TypeObj == TextBox)
		{
			std::stringstream ssEnd;
			ssEnd << "END PS: " << Postranslate.x << " " << Postranslate.y << " " << Postranslate.z << "\n";
			string totalEnd = ssEnd.str();
			tmp_posit = Postranslate;
			if(hyst_posit.size()==0)
				hyst_posit.append(spStart);
			hyst_posit.append(totalEnd);
			hyst_posit.append("--------------------------------");
		}*/
		//==================================================

	}
	else {
		if (obj->StartPos == vec3(0)) {
			obj->Postranslate = obj->NewPostranslate = GetVectorForwardFace(obj->Storage->ConfigMVP, obj->PanelDepth, obj->Storage->Oper);
		}
		else {
			obj->Postranslate = obj->NewPostranslate = GetVectorForwardFaceOffset(obj->Storage->ConfigMVP, obj->PanelDepth - obj->StartPos.z, obj->Storage->Oper, obj->StartPos);
		}
	}

	SavePosFactor(obj->StartPos, obj->Postranslate);

	Billboard();
}

void ShapeSquare::SetSizeControl(vec3 vertOffset) {
	ObjectGUI* obj = m_objGUI;
	if (obj->IsAbsolutePosition)
		return;

	SaveSizeFactor(true);

	//-- set transform
	if (obj->Vertices.size() != 0) {

		if (start_vertBottomLeft == vec3(0)) {
			start_vertBottomLeft = GetBottom(1);
			start_vertBottomRight = GetBottom(0);
			start_vertTopLeft = GetTop(1);
		}
		//vec3 vertTopRight = GetTop(0);

		float offsetY = 2 - vertOffset.y;
		float offsetX = 2 - vertOffset.x;

		//float factor = vertTopLeft.y - vertBottomLeft.y;


		vec3 vertTopLeft = vec3(start_vertTopLeft.x, (start_vertTopLeft.y), start_vertTopLeft.z + offsetX);
		//SetTop(obj, 1, vertTopLeft);
		SetTop(1, vertTopLeft);

		vec3 vertBottomLeft = vec3(start_vertBottomLeft.x, (start_vertBottomLeft.y + offsetY), start_vertBottomLeft.z);
		//SetBottom(obj, 1, vertBottomLeft);
		SetBottom(1, vertBottomLeft);

		vec3 vertBottomRight = vec3(start_vertBottomRight.x, (start_vertBottomRight.y + offsetY), start_vertBottomRight.z + offsetX);
		//SetBottom(obj, 0, vertBottomRight);
		SetBottom(0, vertBottomRight);
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

	/*PosMoveFactor.x = m_lastPosGUI.x - posGUI.x;
	PosMoveFactor.y = m_lastPosWorld.y - posWorld.y*/;
	vec3 posMoveFactor = m_lastPosWorld - posWorld;
	if(posMoveFactor != vec3(0))
		PosMoveFactor = posMoveFactor;

	m_lastPosGUI = posGUI;
	m_lastPosWorld = posWorld;
	
}

void ShapeSquare::ResizeTextureUV() {
	ObjectGUI* obj = m_objGUI;
	if (obj->IsTextureRepeat) {
		std::vector< glm::vec2 > repeat_UV = obj->ModelPtr->UV;
		for (auto& uv : repeat_UV) {
			uv.y *= obj->TextureRepeat;
		}
		obj->TextureUV = repeat_UV;
	}
}