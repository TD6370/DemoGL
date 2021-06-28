#include "ObjectButton.h"

#include "../GeomertyShapes//ShapeBase.h"
#include "ObjectData.h"
//#include "ObjectGUI.h"
//#include "ObjectPhysic.h"
#include "../CoreSettings.h"
//#include "../CreatorModelData.h"
#include "../Serialize/SceneSerialize.h"
#include "../OperationString.h"

ObjectButton::~ObjectButton()
{

}

void ObjectButton::Work() {

	ObjectGUI::Work();

	Color = m_color_work;

	if (IsToogleButon)
		IsToogleButonOn = !IsToogleButonOn;

	SceneCommand->Enable = true;
	SceneCommand->SourceIndex = Index;

	ActionObjectCurrent = Stay; //Off
}
// ----------------------------------------------------------------

vector<ObjectFiledsSpecific> ObjectButton::GetSpecificFiels() {

	vector<ObjectFiledsSpecific> result = ObjectGUI::GetSpecificFiels();
	result.push_back({ "IsToogleButon:", BoolToStr(IsToogleButon) });
	return result;
}

void ObjectButton::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (IndexObjectOwner == -1) //Back GUI
		return;

	ObjectGUI::SetSpecificFiels(filedsSpecific);

	string valueStr = filedsSpecific[filedsSpecific.size() - 1].Value;
	IsToogleButon = StrToBool(valueStr);
}

//----------------------------------------------------------------
