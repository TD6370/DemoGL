#include "ObjectEditBox.h"
#include "../ModelData.h"
#include "../CoreSettings.h"

ObjectEditBox::~ObjectEditBox() {

}


void ObjectEditBox::DefaultView() {
	
	ObjectGUI::DefaultView();

	ParamValue = 1;
}

void ObjectEditBox::Work() 
{
	AnimationParamGUI animParam;

	Color = m_color_work;
	
	ParamValue = animParam.StartPressedParamShaderID;
}

