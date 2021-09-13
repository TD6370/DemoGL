#pragma once

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

class ObjectData;
class ObjectGUI;
struct ObjectFiledsSpecific;

class TextBoxComponent
{
private:
	ObjectData* m_obj;

	vec3 m_color_work = vec3(0, 1, 0); //green

	vector<int> MessageCode;
	int MessageSlots = 50;
	bool isInitSlotsMessage = false;

	vector<int> MessageCodeConvert(string Message);

public:
	string Message = std::string();

	TextBoxComponent(): m_obj(nullptr)	{}
	~TextBoxComponent() {}

	void Init(ObjectData* obj);

	//void Click();

	void ActionWork();

	void CreateMessage();

	void UpdateMessage();

	void Refresh();

	void MeshTransform();

	string GetCashStateUpdateDataToShader();

	int GetRightBorderVertexIndex();

	vector<ObjectFiledsSpecific> GetSpecificFiels();
	void SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);

	//-- Edit box
	void AddSymbolMessage(string symbol);
	void DefaultView();

};

