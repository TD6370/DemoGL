#pragma once

#include "./ObjectGUI.h"
#include "../CoreSettings.h"

class ModelData;

class ObjectListBox : public ObjectGUI
{
public:
	ObjectListBox(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj,
		vec3 p_pos = vec3(0))
		: ObjectGUI(p_index,
			p_model,
			p_typeObj,
			p_pos) 
	{
		
	};

	void InitData();
};

