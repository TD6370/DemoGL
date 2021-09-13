#pragma once

#include "TextBoxComponent.h"

#include "../ObjectsTypes/ObjectData.h"
#include "../ObjectsTypes/ObjectGUI.h"

//----------------------

#include "../CoreSettings.h"
#include "../WorldCollision.h"
#include "../ModelData.h"
#include "../OperationString.h"
#include "../GeomertyShapes/ShapeBase.h"
#include "../Serialize/SceneSerialize.h"
#include "../CreatorModelData.h"
#include "../SceneConstructor.h"
#include "../Components/RenderComponent.h"

#include <sstream>
#include <vector>
#include <map>

using glm::vec3;
using glm::vec4;
using glm::vec2;
using std::map;
using std::string;
using std::shared_ptr;
glm::mat4;

extern map<string, int> MapAlphabet;
extern map<string, int> MapAlphabetEng;


void TextBoxComponent::Init(ObjectData* obj) {

	m_obj = obj;

	m_obj->IsSquareModel = true;
	m_obj->IsAbsolutePosition = true;
	m_obj->IsFocusable = false;

	if (m_obj->TypeObj == TypeObject::EditBox)
	{
		m_obj->ParamValue = 1;
	}
}

//void TextBoxComponent::Click() {
//	m_obj->IsChecked = !m_obj->IsChecked;
//	if (m_obj->IsChecked)
//		m_obj->ActionObjectCurrent = Woking;
//	else
//		m_obj->ActionObjectCurrent = Stay;
//}

void TextBoxComponent::ActionWork() {

	m_obj->MaterialData.Color = m_color_work;

	if (m_obj->TypeObj == TypeObject::EditBox)
	{
		AnimationParamGUI animParam;
		m_obj->ParamValue = animParam.StartPressedParamShaderID;
	}
}

string TextBoxComponent::GetCashStateUpdateDataToShader() {
	return Message;
}

vector<int> TextBoxComponent::MessageCodeConvert(string message)
{
	MessageCode = vector<int>();
	int code;
	char symbLow;
	for (char symbC : message) {
		symbLow = std::tolower(symbC);
		string symb(1, symbLow);
		if (IsMapContains_StrInt(MapAlphabet, symb)) {
			code = MapAlphabet[symb];
		}
		else if (IsMapContains_StrInt(MapAlphabetEng, symb)) {
			code = MapAlphabetEng[symb] + 100;
		}
		else
			continue;
		MessageCode.push_back(code);
	}
	return MessageCode;
}


void TextBoxComponent::CreateMessage() {

	MessageCodeConvert(Message);

	MeshTransform();
}

void TextBoxComponent::MeshTransform() {

	if (m_obj->MeshData.Vertices.size() == 0)
		m_obj->MeshData.Vertices = m_obj->ModelPtr->MeshData.Vertices;
	if (m_obj->MeshData.Normals.size() == 0)
		m_obj->MeshData.Normals = m_obj->ModelPtr->MeshData.Normals;
	if (m_obj->MeshData.UV.size() == 0)
		m_obj->MeshData.UV = m_obj->ModelPtr->MeshData.UV;

	vector<vec3> vertexFrame = m_obj->MeshData.Vertices;
	vector<vec2> textureUV = m_obj->MeshData.UV;
	vector<vec3> normals = m_obj->MeshData.Normals;
	vector<vec3> bufferLats = vector<vec3>();
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));

	m_obj->MeshData.TrianglesCount = m_obj->ModelPtr->MeshData.TrianglesCount;

	m_obj->MeshData.Buffer.clear();

	int firstStep = 0;
	bool isFirstStep = false;
	int indVert = 0;
	float offsetX = 0.035;
	int vertexSize = 6;
	int codeSymb;
	int sizeMessage = MessageCode.size();
	int spaceSymb = MapAlphabet[" "];

	for (int indSymb = 0; indSymb < MessageSlots; indSymb++) {

		if (indSymb > sizeMessage - 1)
			codeSymb = -1;
		else
			codeSymb = MessageCode[indSymb];
		isFirstStep = firstStep == 0;

		for (int i = 0; i < vertexSize; i++)
		{
			if (!isFirstStep)
				vertexFrame[i].z -= offsetX;
			bufferLats[i].x = codeSymb;
			bufferLats[i].y = codeSymb;
			bufferLats[i].z = codeSymb;
		}

		m_obj->MeshData.Buffer.insert(m_obj->MeshData.Buffer.end(), bufferLats.begin(), bufferLats.end());
		if (!isFirstStep)
		{
			m_obj->MeshData.Vertices.insert(m_obj->MeshData.Vertices.end(), vertexFrame.begin(), vertexFrame.end());
			m_obj->MeshData.UV.insert(m_obj->MeshData.UV.end(), textureUV.begin(), textureUV.end());
			m_obj->MeshData.TrianglesCount += vertexSize;
		}
		firstStep++;
	}

	m_obj->Shape->FillVertextBox();

	m_obj->Render->ResetMem_Vertex();
	m_obj->Render->ResetMem_UV();
}


void TextBoxComponent::Refresh() {

	UpdateMessage();
}

void TextBoxComponent::UpdateMessage()
{
	if (m_obj->EngineData->Inputs->Key == GLFW_KEY_BACKSPACE) {
		if (Message.size() != 0)
			Message.erase(Message.end() - 1);
	}
	if (m_obj->EngineData->Inputs->Key == GLFW_KEY_DELETE) {
		Message.clear();
	}

	MessageCodeConvert(Message);

	int vertexSize = 6;
	int indSymb = 0;
	int codeSymb = 0;
	int ind = -1;
	int sizeMessage = MessageCode.size();
	int spaceSymb = MapAlphabet[" "];

	for (vec3& dataSymb : m_obj->MeshData.Buffer) {

		if (ind == vertexSize || ind == -1) {
			ind = 0;
			if (indSymb > sizeMessage - 1)
				codeSymb = -1;
			else
				codeSymb = MessageCode[indSymb++];
		}

		dataSymb.x = codeSymb;
		dataSymb.y = codeSymb;
		dataSymb.z = codeSymb;
		ind++;
	}

}

int TextBoxComponent::GetRightBorderVertexIndex() {

	return (Message.size() * 2) - 1;
}

vector<ObjectFiledsSpecific> TextBoxComponent::GetSpecificFiels() {

	vector<ObjectFiledsSpecific> result{
	{ "Message:", Message } 
	};

	return result;
}

void TextBoxComponent::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (m_obj->IndexObjectOwner == -1) //Back GUI
		return;

	m_obj->Render->ResetMem_Vertex();
	m_obj->Render->ResetMem_UV();

	Message = filedsSpecific[filedsSpecific.size() - 1].Value;

	m_obj->MeshData.Vertices.clear();
	m_obj->MeshData.Normals.clear();
	m_obj->MeshData.UV.clear();
	m_obj->MeshData.Buffer.clear();

	CreateMessage();
}

void TextBoxComponent::AddSymbolMessage(string symbol) {

	if (symbol.size() != 0)
		Message += symbol;
	
	UpdateMessage();
}


void TextBoxComponent::DefaultView() {

	m_obj->ParamValue = 1;
}