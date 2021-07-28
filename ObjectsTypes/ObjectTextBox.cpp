#include "ObjectTextBox.h"

#include "./ObjectData.h"
#include "../CoreSettings.h"
#include "../WorldCollision.h"
#include "../ModelData.h"
#include "../OperationString.h"
#include "../GeomertyShapes/ShapeBase.h"
#include "../Serialize/SceneSerialize.h"
#include "../CreatorModelData.h"
#include "../SceneConstructor.h"

#include <sstream>

using glm::vec3;
using glm::vec4;
using glm::vec2;
using std::map;
using std::string;
using std::shared_ptr;
glm::mat4;

extern map<string, int> MapAlphabet;
extern map<string, int> MapAlphabetEng;

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

	MaterialData.Color = m_color_work;
}

string ObjectTextBox::GetCashStateUpdateDataToShader() {
	return Message;
}


void ObjectTextBox::SetDataToShader() {

	ObjectGUI::SetDataToShader();

	ModelPtr->SetBuffer(MeshData.Buffer);

	if (!isInitSlotsMessage) {
		isInitSlotsMessage = true;
		
		ModelPtr->SetModelInBuffer(MeshData.UV, MeshData.Normals, false); //TODO: delete
	}
}

vector<int>  ObjectTextBox::MessageCodeConvert(string message)
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


void ObjectTextBox::CreateMessage() {

	MessageCodeConvert(Message);

	MeshTransform();
}

void ObjectTextBox::MeshTransform() {
	
	if(MeshData.Vertices.size() == 0)
		MeshData.Vertices = ModelPtr->MeshData.Vertices;
	if(MeshData.Normals.size() == 0)
		MeshData.Normals = ModelPtr->MeshData.Normals;
	if (MeshData.UV.size() == 0)
		MeshData.UV = ModelPtr->MeshData.UV;

	vector<vec3> vertexFrame = MeshData.Vertices;
	vector<vec2> textureUV = MeshData.UV;
	vector<vec3> normals = MeshData.Normals;
	vector<vec3> bufferLats = vector<vec3>();
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));
	bufferLats.push_back(vec3(0));

	MeshData.TrianglesCount = ModelPtr->MeshData.TrianglesCount;

	MeshData.Buffer.clear();

	int firstStep = 0;
	bool isFirstStep = false;
	int indVert = 0;
	float offsetX = 0.0477;
	int vertexSize = 6;
	int codeSymb;
	int sizeMessage = MessageCode.size();
	int spaceSymb = MapAlphabet[" "];

	for (int indSymb = 0; indSymb < MessageSlots; indSymb++) {

		if(indSymb > sizeMessage - 1)
			codeSymb = -1;
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

		MeshData.Buffer.insert(MeshData.Buffer.end(), bufferLats.begin(), bufferLats.end());
		if (!isFirstStep)
		{
			//Vertices.push_back(vertexFrame);
			MeshData.Vertices.insert(MeshData.Vertices.end(), vertexFrame.begin(), vertexFrame.end());
			MeshData.UV.insert(MeshData.UV.end(), textureUV.begin(), textureUV.end());
			//Normals.insert(Normals.end(), normals.begin(), normals.end());
			MeshData.TrianglesCount += vertexSize;
		}
		firstStep++;
	}

	Shape->FillVertextBox();

	IsLoadedIntoMem_Vertex = false;
	IsLoadedIntoMem_UV = false;
}


void ObjectTextBox::Refresh() {
	ObjectData::Refresh();
	UpdateMessage();
}

void ObjectTextBox::UpdateMessage()
{
	if (EngineData->Inputs->Action == GLFW_PRESS
		&& EngineData->Inputs->Key == GLFW_KEY_BACKSPACE)
	{
		Message.clear();
	}

	MessageCodeConvert(Message);

	int vertexSize = 6;
	int indSymb = 0;
	int codeSymb = 0;
	int ind = -1;
	int sizeMessage = MessageCode.size();
	int spaceSymb = MapAlphabet[" "];

	for (vec3& dataSymb : MeshData.Buffer) {
		
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

int ObjectTextBox::GetRightBorderVertexIndex() {

	return (Message.size() * 2) - 1;
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

	IsLoadedIntoMem_Vertex = false;
	IsLoadedIntoMem_UV = false;

	Message = filedsSpecific[filedsSpecific.size()-1].Value;

	//SetSizeControl(vec3(SizePanel.x, SizePanel.y, 1));
	MeshData.Vertices.clear();
	MeshData.Normals.clear();
	MeshData.UV.clear();
	MeshData.Buffer.clear();

	CreateMessage();
}





