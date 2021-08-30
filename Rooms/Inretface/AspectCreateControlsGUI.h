#pragma once

#include "../AspectBase.h"
#include "../../CoreSettings.h"

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using glm::vec3;
using glm::vec2;
using std::shared_ptr;

class SceneConstructor;
class RoomUseInterface;
class ObjectData;

class AspectCreateControlsGUI : public AspectBase
{
private:
	RoomUseInterface* m_base;

	std::shared_ptr<ObjectData> ObjectSelectedEdit = nullptr;
	vec3 CursorRayPos = vec3(0);

public:

	AspectCreateControlsGUI(string Name, SceneConstructor* sceneConstructor)
		: AspectBase(Name, sceneConstructor) {};

	void Init(RoomUseInterface* base);
	void Init();

	void Config();
	void Work();

	//============= Event Create Controls ==============

	//------- CreateObject
	void EventSelectedInfoCreateObject(); // Event selected info create object
	void EventStartCreateObject();
	void EventEndCreateObject();

	//---------- Fill fields selected object
	void EventFillFieldsEdit();
	bool EventSaveFieldsEdit();
	void SaveEditObjectWoldInfo();
};