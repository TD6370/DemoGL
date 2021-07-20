#include "ObjectEditBox.h"
#include "../ModelData.h"
#include "../CoreSettings.h"
#include "../CreatorModelData.h"


ObjectEditBox::~ObjectEditBox() {

}

void ObjectEditBox::InitData() {

	ObjectGUI::InitData();
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

void ObjectEditBox::AddSymbolMessage(string symbol) {
		
	string tempMessage = Message;

	if (EngineData->Inputs->Key == GLFW_KEY_BACKSPACE) {
		Message.clear();
	}

	if (symbol.size() != 0)
		Message += symbol;

	if(tempMessage != Message)
		UpdateMessage();	
}
