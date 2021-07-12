#include "ObjectTextBox.h"

#include "./ObjectData.h"
#include "../CoreSettings.h"
#include "../WorldCollision.h"
#include "../ModelData.h"
#include "../OperationString.h"
#include "../GeomertyShapes/ShapeBase.h"
#include "../Serialize/SceneSerialize.h"
#include "../CreatorModelData.h"
#include "../SceneConstruction.h"

#include <sstream>

using glm::vec3;
using glm::vec4;
using glm::vec2;
using std::map;
using std::string;
using std::shared_ptr;
glm::mat4;

extern map<string, int> MapAlphabet;

ObjectTextBox::~ObjectTextBox()
{

}

void ObjectTextBox::Click() {
	IsChecked = !IsChecked;
	if (IsChecked)
		ActionObjectCurrent = Woking;
	else
		ActionObjectCurrent = Stay;
}

void ObjectTextBox::ActionWork() {

	Color = m_color_work;
}

string ObjectTextBox::GetCashStateUpdateDataToShader() {
	return Message;
}


void ObjectTextBox::SetDataToShader() {

	ObjectGUI::SetDataToShader();

	ModelPtr->SetBuffer(Buffer);

	if (!isInitSlotsMessage) {
		isInitSlotsMessage = true;
		auto normals = GetNormals();
		ModelPtr->SetModelInBuffer(TextureUV, normals, false);
	}
}

void ObjectTextBox::CreateMessage() {

	MessageCode = vector<int>();
	for (char symbC : Message) {
		char symbLow = std::tolower(symbC);
		string symb(1, symbLow);
		if (!IsMapContains_StrInt(MapAlphabet, symb))
			continue;

		int code = MapAlphabet[symb];
		MessageCode.push_back(code);
	}

	MeshTransform();
}

void ObjectTextBox::MeshTransform() {
	
	if(Vertices.size() == 0)
		Vertices = ModelPtr->Vertices;
	if(Normals.size() == 0)
		Normals = ModelPtr->Normals;
	if (TextureUV.size() == 0)
		TextureUV = ModelPtr->UV;

	vector<vec3> vertexFrame = Vertices;
	vector<vec2> textureUV = TextureUV;
	vector<vec3> normals = Normals;
	vector<vec3> bufferLats = vector<vec3>();
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));

	TrianglesCount = ModelPtr->TrianglesCount;

	Buffer.clear();

	int firstStep = 0;
	bool isFirstStep = false;
	int indVert = 0;
	float offsetX = 0.0477;
	int vertexSize = 6;
	int codeSymb;
	int sizeMessage = MessageCode.size();
	int spaceSymb = MapAlphabet[" "];

	//for (int codeSymb : MessageCode) {
	for (int indSymb = 0; indSymb < MessageSlots; indSymb++) {

		if(indSymb > sizeMessage - 1)
			codeSymb = spaceSymb;
		else
			codeSymb = MessageCode[indSymb];
		isFirstStep = firstStep == 0;

		for (int i = 0 ;  i < vertexSize; i++)
		{
			if (!isFirstStep)
				vertexFrame[i].z -= offsetX;
			bufferLats[i].x = codeSymb;
			bufferLats[i].y = codeSymb;
			bufferLats[i].z = codeSymb;
		}

		Buffer.insert(Buffer.end(), bufferLats.begin(), bufferLats.end());
		if (!isFirstStep)
		{
			//Vertices.push_back(vertexFrame);
			Vertices.insert(Vertices.end(), vertexFrame.begin(), vertexFrame.end());
			TextureUV.insert(TextureUV.end(), textureUV.begin(), textureUV.end());
			//Normals.insert(Normals.end(), normals.begin(), normals.end());
			TrianglesCount += vertexSize;
		}
		firstStep++;
	}

	Shape->FillVertextBox();
}


void ObjectTextBox::UpdateMessage()
{
	if (Storage->Inputs->Action == GLFW_PRESS
		&& Storage->Inputs->Key == GLFW_KEY_BACKSPACE)
	{
		Message.clear();
	}

	MessageCode = vector<int>();
	for (char symbC : Message) {
		char symbLow = std::tolower(symbC);
		string symb(1, symbLow);
		if (!IsMapContains_StrInt(MapAlphabet, symb))
			continue;

		int code = MapAlphabet[symb];
		MessageCode.push_back(code);
	}

	int vertexSize = 6;
	int indSymb = 0;
	int codeSymb = 0;
	int ind = -1;
	int sizeMessage = MessageCode.size();
	int spaceSymb = MapAlphabet[" "];

	for (vec3& dataSymb : Buffer) {
		
		if (ind == vertexSize || ind == -1) {
			ind = 0;
			if (indSymb > sizeMessage - 1)
				codeSymb = spaceSymb;
			else
				codeSymb = MessageCode[indSymb++];
		}

		dataSymb.x = codeSymb;
		dataSymb.y = codeSymb;
		dataSymb.z = codeSymb;
		ind++;
	}

}

//string ObjectTextBox::GetInfo() {
//
//	return ObjectGUI::GetInfo();
//
//	string info = ObjectData::GetInfo();
//
//	std::stringstream ss;
//	ss << info << "	 M: " << Message;
//	return ss.str();
//}


vector<ObjectFiledsSpecific> ObjectTextBox::GetSpecificFiels() {

	vector<ObjectFiledsSpecific> result = ObjectGUI::GetSpecificFiels();

	//SceneSerialize* serializer = new SceneSerialize();
	result.push_back({ "Message:", Message });

	return result;
}

void ObjectTextBox::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (IndexObjectOwner == -1) //Back GUI
		return;

	ObjectGUI::SetSpecificFiels(filedsSpecific);
	//SceneSerialize* serializer = new SceneSerialize();

	Message = filedsSpecific[filedsSpecific.size()-1].Value;

	//SetSizeControl(vec3(SizePanel.x, SizePanel.y, 1));
	Vertices.clear();
	Normals.clear();
	TextureUV.clear();
	Buffer.clear();

	CreateMessage();
}





