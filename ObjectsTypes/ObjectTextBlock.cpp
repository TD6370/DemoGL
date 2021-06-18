#include "ObjectTextBlock.h"

#include "./ObjectData.h"
//#include "./ObjectGUI.h"
//#include "./ObjectPhysic.h"
#include "../CoreSettings.h"
#include "..\WorldCollision.h"
#include "..\ModelData.h"
#include "..\OperationString.h"
#include "../GeomertyShapes//ShapeBase.h" //#####

#include <sstream>

using glm::vec3;
using glm::vec4;
using glm::vec2;
using std::map;
using std::string;
using std::shared_ptr;
glm::mat4;


ObjectTextBlock::~ObjectTextBlock()
{

}

void ObjectTextBlock::CreateMessage() {

	mapAlphabet = map<string, int>{
		{"à",0},
		{"á",1},
		{"â",2},
		{"ã",3},
		{"ä",4},
		{"å",5},
		{"æ",6},
		{"ç",7},
		{"è",8},
		{"ê",9},
		{"ë",10},
		{"ì",11},
		{"í",12},
		{"î",13},
		{"ï",14},
		{"ð",15},
		{"ñ",16},
		{"ò",17},
		{"ó",18},
		{"ô",19},
		{"õ",20},
		{"ö",21},
		{"÷",22},
		{"ø",23},
		{"ø",24},
		{"ü",25},
		{"û",26},
		{"ý",27},
		{"þ",28},
		{"ÿ",29},
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

void ObjectTextBlock::MeshTransform() {
	
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

	//std::stringstream ss1;

	//for (int i = 0; i < Vertices.size(); i+=6) {
	//	ss1 << " " << Vertices[i].z 
	//		<< " " << Vertices[i+1].z 
	//		<< " " << Vertices[i+2].z 
	//		<< " " << Vertices[i+3].z 
	//		<< " " << Vertices[i+4].z 
	//		<< " " << Vertices[i+5].z << "\n";
	//}

		//IsTextureRepeat = true;
	//std::cout << ss1.str();
	//std::cout << std::endl;

	Shape->FillVertextBox();
}

vector<ObjectFiledsSpecific> ObjectTextBlock::GetSpecificFiels() {

	vector<ObjectFiledsSpecific> result = ObjectGUI::GetSpecificFiels();

	//SceneSerialize* serializer = new SceneSerialize();
	result.push_back({ "Message:", Message });

	return result;
}

void ObjectTextBlock::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

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





