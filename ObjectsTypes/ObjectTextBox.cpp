#include "ObjectTextBox.h"

#include "./ObjectData.h"
#include "../CoreSettings.h"
#include "../WorldCollision.h"
#include "../ModelData.h"
#include "../OperationString.h"
#include "../GeomertyShapes/ShapeBase.h"
#include "../Serialize/SceneSerialize.h"

#include <sstream>

using glm::vec3;
using glm::vec4;
using glm::vec2;
using std::map;
using std::string;
using std::shared_ptr;
glm::mat4;


ObjectTextBox::~ObjectTextBox()
{

}

void ObjectTextBox::SetDataToShader() {

	ObjectGUI::SetDataToShader();

	ModelPtr->SetBuffer(Buffer);
	auto normals = GetNormals();
	ModelPtr->SetModelInBuffer(TextureUV, normals, false);
}

void ObjectTextBox::CreateMessage() {

	mapAlphabet = map<string, int>{
		{"�",0},
		{"�",1},
		{"�",2},
		{"�",3},
		{"�",4},
		{"�",5},
		{"�",6},
		{"�",7},
		{"�",8},
		{"�",9},
		{"�",10},
		{"�",11},
		{"�",12},
		{"�",13},
		{"�",14},
		{"�",15},
		{"�",16},
		{"�",17},
		{"�",18},
		{"�",19},
		{"�",20},
		{"�",21},
		{"�",22},
		{"�",23},
		{"�",24},
		{"�",25},
		{"�",26},
		{"�",27},
		{"�",28},
		{"�",29},
		{" ",30},

		{"1",31},
		{"2",32},
		{"3",33},
		{"4",34},
		{"5",35},
		{"6",36},
		{"7",37},
		{"8",38},
		{"9",39},
		{"0",40},
		{".",41},
		{",",42},
		{"!",43},
		{"?",44},
		{":",45},
		{"-",46},
		{"=",47},
		{"<",48},
		{">",49},
	};

	MessageCode = vector<int>();
	for (char symbC : Message) {
		char symbLow = std::tolower(symbC);
		string symb(1, symbLow);
		if (!IsMapContains_StrInt(mapAlphabet, symb))
			continue;

		int code = mapAlphabet[symb];
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

	for (int codeSymb : MessageCode) {
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




