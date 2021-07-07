#include "ObjectEditBox.h"
#include "../ModelData.h"
#include "../CoreSettings.h"

ObjectEditBox::~ObjectEditBox() {

}

void ObjectEditBox::InitAlphabetKeys() {

	//map<int, int> mapAlphabetKeys;

	mapAlphabetKeys = map<int, string>{
		{GLFW_KEY_Q ,"é"},
		{GLFW_KEY_W ,"ö"},
		{GLFW_KEY_E ,"ó"},
		{GLFW_KEY_R ,"ê"},
		{GLFW_KEY_T ,"å"},
		{GLFW_KEY_Y ,"í"},
		{GLFW_KEY_U ,"ã"},
		{GLFW_KEY_I ,"ø"},
		{GLFW_KEY_P ,"ç"},
		{GLFW_KEY_LEFT_BRACKET ,"õ"},
		{GLFW_KEY_A ,"ô"},
		{GLFW_KEY_S ,"û"},
		{GLFW_KEY_D ,"â"},
		{GLFW_KEY_F ,"à"},
		{GLFW_KEY_G ,"ï"},
		{GLFW_KEY_H ,"ð"},
		{GLFW_KEY_J ,"î"},
		{GLFW_KEY_K ,"ë"},
		{GLFW_KEY_L ,"ä"},
		{GLFW_KEY_SEMICOLON ,"æ"},	//GLFW_KEY_APOSTROPHE   39 /* ' */	GLFW_KEY_SEMICOLON   59 /* ; */
		{GLFW_KEY_APOSTROPHE ,"ý"},
		{GLFW_KEY_Z ,"ÿ"},
		{GLFW_KEY_X ,"÷"},
		{GLFW_KEY_C ,"ñ"},
		{GLFW_KEY_V ,"ì"},
		{GLFW_KEY_B ,"è"},
		{GLFW_KEY_N ,"ò"},
		{GLFW_KEY_M ,"ü"},
		{GLFW_KEY_COMMA ,"á"},	//GLFW_KEY_COMMA   44 /* , */	GLFW_KEY_PERIOD   46 /* . */
		{GLFW_KEY_PERIOD ,"þ"},
		{GLFW_KEY_SPACE ," "},
		/*{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},*/

	};

}

void ObjectEditBox::InitData() {

	ObjectGUI::InitData();

	InitAlphabetKeys();
}


void ObjectEditBox::DefaultView() {
	
	ObjectGUI::DefaultView();

	ParamValue = 1;
}

void ObjectEditBox::ActionWork() 
{
	AnimationParamGUI animParam;

	Color = m_color_work;
	
	ParamValue = animParam.StartPressedParamShaderID;
}


void ObjectEditBox::AddSymbolMessage(int keyIndex) {

	Message += GetSymbol(keyIndex);
	//CreateMessage();
	UpdateMessage();
}

string ObjectEditBox::GetSymbol(int keyIndex) {

	map <int, string> ::iterator it;
	it = mapAlphabetKeys.find(keyIndex);
	if (it != mapAlphabetKeys.end()) 
	{
		return mapAlphabetKeys[keyIndex];
	}
	return "";
}
