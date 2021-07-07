#pragma once


#include "ObjectTextBox.h"
#include "../CoreSettings.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::map;
using std::string;
using std::vector;
using std::shared_ptr;

class ModelData;
struct ObjectFiledsSpecific;

class ObjectEditBox :
	public ObjectTextBox
{
private:
	string GetSymbol(int keyIndex);
	map<int, string> mapAlphabetKeys;

public:

	ObjectEditBox(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		//TypeObject p_typeObj = TypeObject::TextBox,
		TypeObject p_typeObj,
		vec3 p_pos = vec3(0))
		: ObjectTextBox(p_index,
			p_model,
			p_typeObj,
			p_pos) {

		ParamValue = 1;
		IsAbsolutePosition = true;
		IsFocusable = false;
	};

	~ObjectEditBox();

	void InitData();

	void ActionWork();

	void DefaultView();

	void AddSymbolMessage(int keyIndex);

	void InitAlphabetKeys();
	//-------------

	/*vector<ObjectFiledsSpecific> GetSpecificFiels();
	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);*/
};
