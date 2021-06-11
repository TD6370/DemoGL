#pragma once
#include "ObjectData.h"
#include "ObjectPhysic.h"
//#include "ObjectTextBlock.h"
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
using std::shared_ptr;

class ObjectGUI :
	public ObjectPhysic
{
private :
	vec3 m_color_work = vec3(0, 1, 0); //green

public:
	bool IsFocused;

	//TODO: inTextBlosk object
	//string Message = std::string();
	vec3 StartPos = vec3(0);
	vec2 SizePanel = vec2(0);
	float PanelDepth;

	string CommandName;

	ObjectGUI(
		int p_index,
		std::shared_ptr<ModelData> p_model,
		TypeObject p_typeObj = TypeObject::GUI,
		vec3 p_pos = vec3(0))
		: ObjectPhysic(p_index,
			p_model,
			p_typeObj,
			p_pos) 	{
		IsGUI = true;
		IsFocused = true;
	};

	void InitData();

	void RunAction();

	void virtual UpdateState();

	bool IsShow();

	void ConfigInterface(string caption, string nameModel, string nameObject, vec3 position, vec2 size, TypeObject p_typeObj = TypeObject::GUI, vec3 color = vec3(0));

	bool IsCubeModel();

	void virtual SetSizeControl(vec3 size);

	void virtual Click();
	
	void virtual Work();

	void virtual DefaultSate();

	vector<ObjectFiledsSpecific> GetSpecificFiels();

	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);

	//-------------

	void ResizeTextureUV();
};