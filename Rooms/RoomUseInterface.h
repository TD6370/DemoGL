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
using std::string;
using glm::perspective;

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
	vec2 m_tempMousePos = vec2(0);
	vec3 m_tempMousePosWorld = vec3(0);
	string m_stringDebug = "";
	bool m_isDebug = false;
	mat4 m_projectionPerspective;
	int m_KeyPush = GLFW_MOUSE_BUTTON_1;

	int IndexObjectFocused = -1;
	int IndexObjectSelected = -1;
	vec3 CursorMovePos = vec3(0);
	vec3 SelectObjectOffsetPos = vec3(0);

public:
		
	RoomUseInterface(string Name, SceneConstruction* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	~RoomUseInterface();

	vec3 GetMousePosWorld();

	void Init();
	void Config();
	void Work();

};

