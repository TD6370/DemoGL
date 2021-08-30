#pragma once

#include "AspectBase.h"
#include "../GeometryLib.h"
#include "..\CoreSettings.h"


#include "./Inretface/AspectTransformControlsGUI.h"
#include "./Inretface/AspectCreateControlsGUI.h"
#include "./Inretface/AspectUseControlsGUI.h"

#include <sstream>

//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat4;
using glm::perspective;
using std::string;
using std::shared_ptr;

class ObjectData;
class ObjectGUI;
class SceneConstructor;
enum TypeCommand;
struct AnimationParamGUI;

struct StateGUI {
	bool IsFocused = false;
	bool IsCursorClickEvent = false;
	bool IsCheckBorder = false;
	bool IsEditControls = false;
	bool IsBackgroundFrame = false;
	bool IsUpdatingStateObjects = true;
	bool IsCreatingObject = false;
};

class RoomUseInterface : public AspectBase
{
private:
	vec3 color_selected = vec3(0, 0, 0);
	PaletteColors m_palette = PaletteColors();

	vec2 m_tempMousePos = vec2(0);
	vec3 m_tempMousePosWorld = vec3(0);

	float m_sizeBorder = 0.2;
	string m_stringDebug = "";
		
	float m_CurrentStartedEventID = -1;
		
	bool m_isDebug = false;
	mat4 m_projectionPerspective;
	int m_KeyPush = GLFW_MOUSE_BUTTON_1;
	
	float FocusedOrder = -1;
	float StartTimer = 0;
	float EndTimer = 0;

	//=========
	AspectTransformControlsGUI AspectTransformControls;
	AspectCreateControlsGUI AspectCreateControls;
	AspectUseControlsGUI AspectUseControls;

public:

	AnimationParamGUI* AnimationParams;
	StateGUI State = StateGUI();

	int IndexObjectFocused = -1;
	int IndexObjectSelected = -1;
	int IndexObjectCreating = -1;
	int IndexObjectSelectedEdit = -1;
			
	RoomUseInterface(string Name, SceneConstructor* sceneConstructor)
		: AspectBase(Name, sceneConstructor)
		, AspectTransformControls("EventsTransformControls", sceneConstructor),
		AspectCreateControls("EventsCreateControls", sceneConstructor),
		AspectUseControls("EventsUseControls", sceneConstructor)
	{
	};

	~RoomUseInterface();

	void Init();
	void Config();
	void Work();

	//=================================================
	void CalculateMousePosWorld();

	//--- Event Focus control
	void CheckFocusBoxAndBorderControl();
	void EventFocusControl();

	//--- call refresh visible
	void CheckStateObjects();

	// -- save event status animation
	void SetCurrentEventParam(shared_ptr<ObjectData> obj, int value);
	void SetTimeAnimate(shared_ptr<ObjectData> obj, float time);
};

