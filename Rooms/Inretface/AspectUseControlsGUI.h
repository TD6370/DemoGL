#pragma once

#include "../AspectBase.h"
//#include "../RoomUseInterface.h"
#include "../../CoreSettings.h"

#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using glm::vec3;
using glm::vec2;
using std::shared_ptr;

class SceneConstructor;
class RoomUseInterface;

class AspectUseControlsGUI : public AspectBase
{
private:
	RoomUseInterface* m_base;

	const int m_endEditKey = GLFW_KEY_ENTER;
public:

	AspectUseControlsGUI(string Name, SceneConstructor* sceneConstructor)
		: AspectBase(Name, sceneConstructor) {};

	void Init(RoomUseInterface* base);
	void Init();

	void Config();
	void Work();
	
	//============== Event Use Controls ========================================

	//--- Use = Edit on/off
	void ModeEditControls();

	//--- Use = Click control
	void EventStartClickControl();

	//--- Edit = Name control
	void EventStartRenameObject();

	//--- Use = Edit box controls
	void EventEditTextControl();

	//--- Use = Message - EditBox & TextBox
	void EventReadKeyInput();
};
