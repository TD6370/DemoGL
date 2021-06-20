#pragma once

#include "SceneRoom.h"
#include "../GeometryLib.h"

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

class ObjectGUI;
class SceneConstruction;

class RoomUseInterface : public SceneRoom
{
private:
	vec3 color_red = vec3(1, 0, 0);
	vec3 color_blue = vec3(0, 0, 1);
	vec3 color_green = vec3(0, 1, 0);
	vec3 color_violet = vec3(1, 0, 1);
	vec3 color_yelow = vec3(1, 1, 0);
	vec3 color_none = vec3(0);
	vec3 color_default = vec3(1);
	vec3 color_selected = vec3(0, 0, 0);
	//vec3 color_resize = vec3(0.38, 0.159, .235);
	vec3 color_resize = vec3(0.022, 0.099, 0.950); // 15 %, 62 %, 92 %
	vec2 m_tempMousePos = vec2(0);
	vec3 m_tempMousePosWorld = vec3(0);
	float m_sizeBorder = 0.2;
	string m_stringDebug = "";
	float m_startDefaultParamShaderID = 0;
	float m_startFocusParamShaderID = 1;
	float m_startMoveParamShaderID = 2;
	float m_startResizeParamShaderID = 3;
	float m_startCheckBorderParamShaderID = 4;
	float m_startClickParamShaderID = 5;
	float m_CurrentStartedEventID = -1;
		
	bool m_isDebug = false;
	mat4 m_projectionPerspective;
	int m_KeyPush = GLFW_MOUSE_BUTTON_1;

	bool IsFocused = false;
	bool IsCursorClickEvent = false;
	bool IsCursorClickEventConst = false;
	bool IsCheckBorder = false;
	int IndexObjectFocused = -1;
	int IndexObjectSelected = -1;
	float FocusedOrder = -1;
	vec3 CursorMovePos = vec3(0);
	vec3 SelectObjectOffsetPos = vec3(0);
	vec2 m_startSizePanel = vec2(0);

public:
		
	RoomUseInterface(string Name, SceneConstruction* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	~RoomUseInterface();

	void CalculateMousePosWorld();

	void Init();
	void Config();
	void Work();

	//-------- Move control
	void EventStartMovingControl(shared_ptr<ObjectGUI> objGUI);
	void EventMovingControl(shared_ptr<ObjectGUI> objGUI);
	bool EventEndMovingControl(shared_ptr<ObjectGUI> objGUI);

	//-------- Resize control
	void EventStartResizeControl(shared_ptr<ObjectGUI> objGUI);
	void EventResizeControl(shared_ptr<ObjectGUI> objGUI);
	bool EventEndResizeControl(shared_ptr<ObjectGUI> objGUI);

	//-------- Focus control
	//bool IsFocusedControl(shared_ptr<ObjectGUI> objGUI);
	void CheckFocusBoxAndBorderControl(shared_ptr<ObjectGUI> objGUI);
	void EventFocusControl(shared_ptr<ObjectGUI> objGUI);

	void SetCurrentEventParam(shared_ptr<ObjectGUI> obj, float value);
};

