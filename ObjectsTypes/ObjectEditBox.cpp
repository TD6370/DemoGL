#include "ObjectEditBox.h"
#include "../ModelData.h"
#include "../CoreSettings.h"
#include "../CreatorModelData.h"

#include "../Components/TextBoxComponent.h"

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

	MaterialData.Color = m_color_work;
	
	ParamValue = animParam.StartPressedParamShaderID;
}

void ObjectEditBox::AddSymbolMessage(string symbol) {
		
	if (symbol.size() != 0)
		Message += symbol;

	
	if (IsTextBoxComponent) //#TBX
	{
		TextBox->UpdateMessage();
	}
	else {
		UpdateMessage();  //#TBX--	 
	}
}
