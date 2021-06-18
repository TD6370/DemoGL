
#include "ShapeSquare.h"

ShapeSquare::~ShapeSquare() {

}


void ShapeSquare::UpdateState() {
	ObjectGUI* obj = m_objGUI;
	if (!obj->Storage->SceneData->IsGUI)
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

	if (obj->IsAbsolutePosition)
	{
		float offsetOfCenter = 0;
		//-----------------------------
		int indOwner = obj->IndexObjectOwner;
		if (indOwner == -1)
			indOwner = obj->Storage->SceneData->IndexGUIObj;
		if (obj->IndexObjectOwner != -1)
		{
			auto objOwn = obj->Storage->GetObjectPrt(obj->IndexObjectOwner);
			auto  objPh = std::dynamic_pointer_cast<ObjectPhysic>(objOwn);
			float lenghtLineOwner = objPh->Shape->GetLineLenght(0);
			float lenghtLine = obj->Shape->GetLineLenght(0);
			float offsetOwner = lenghtLineOwner / 2;
			float offsetAbs = lenghtLine / 2;
			offsetOfCenter = offsetOwner - offsetAbs;
		}

		vec3 startPos = vec3(obj->StartPos.x - offsetOfCenter, obj->StartPos.y - offsetOfCenter, obj->StartPos.z); //when SetSizeControl - is disabled
		obj->Postranslate = obj->NewPostranslate = GetVectorForwardFaceOffset(obj->Storage->ConfigMVP,
			obj->PanelDepth - obj->StartPos.z,
			obj->Storage->Oper, startPos);

		//================================================== TEST
		//TEST -- hystory move pos TextBox
		/*if (Postranslate != tmp_posit && TypeObj == TextBlock)
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
	Billboard();
}

void ShapeSquare::SetSizeControl(vec3 vertOffset) {
	ObjectGUI* obj = m_objGUI;
	if (obj->IsAbsolutePosition)
		return;

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