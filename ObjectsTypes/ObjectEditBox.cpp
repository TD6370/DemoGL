#include "ObjectEditBox.h"
#include "../ModelData.h"
#include "../CoreSettings.h"

ObjectEditBox::~ObjectEditBox() {

}

void ObjectEditBox::InitAlphabetKeys() {

	//map<int, int> mapAlphabetKeys;

	mapAlphabetKeys = map<int, string>{
		{GLFW_KEY_Q ,"�"},
		{GLFW_KEY_W ,"�"},
		{GLFW_KEY_E ,"�"},
		{GLFW_KEY_R ,"�"},
		{GLFW_KEY_T ,"�"},
		{GLFW_KEY_Y ,"�"},
		{GLFW_KEY_U ,"�"},
		{GLFW_KEY_I ,"�"},
		{GLFW_KEY_P ,"�"},
		{GLFW_KEY_LEFT_BRACKET ,"�"},
		{GLFW_KEY_A ,"�"},
		{GLFW_KEY_S ,"�"},
		{GLFW_KEY_D ,"�"},
		{GLFW_KEY_F ,"�"},
		{GLFW_KEY_G ,"�"},
		{GLFW_KEY_H ,"�"},
		{GLFW_KEY_J ,"�"},
		{GLFW_KEY_K ,"�"},
		{GLFW_KEY_L ,"�"},
		{GLFW_KEY_SEMICOLON ,"�"},	//GLFW_KEY_APOSTROPHE   39 /* ' */	GLFW_KEY_SEMICOLON   59 /* ; */
		{GLFW_KEY_APOSTROPHE ,"�"},
		{GLFW_KEY_Z ,"�"},
		{GLFW_KEY_X ,"�"},
		{GLFW_KEY_C ,"�"},
		{GLFW_KEY_V ,"�"},
		{GLFW_KEY_B ,"�"},
		{GLFW_KEY_N ,"�"},
		{GLFW_KEY_M ,"�"},
		{GLFW_KEY_COMMA ,"�"},	//GLFW_KEY_COMMA   44 /* , */	GLFW_KEY_PERIOD   46 /* . */
		{GLFW_KEY_PERIOD ,"�"},
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
