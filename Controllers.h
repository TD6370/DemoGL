#pragma once
#include "CoreSettings.h"
#include "SceneConstruction.h"

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//OpenGL Mathematics
#include <glm/glm.hpp>
//using glm::vec3;

struct ControllerInput;

class SceneConstruction;

glm::vec3 GetPositionModelCursor(glm::mat4 p_projection, glm::mat4  p_view, glm::mat4 p_model, GLdouble mouseX, GLdouble mouseY, int m_widthWindow, int m_heightWindow);

class Controllers {

public:
	//glm::vec3 GetPositionModelCursor(glm::mat4 p_projection, glm::mat4  p_view, glm::mat4 p_model, GLdouble mouseX, GLdouble mouseY, int m_widthWindow, int m_heightWindow);

	//void MouseEvents(GLFWwindow* window,
	//	int m_widthWindow, int m_heightWindow,
	//	float m_deltaTime,
	//	Operator* p_Operator,
	//	ControllerInput* p_inputs);
	void MouseEvents(GLFWwindow* window,
		int m_widthWindow, int m_heightWindow,
		SceneConstruction* Scene);

	void MouseButtonEvents(GLFWwindow* window, SceneConstruction* Scene);

	//void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mode,
	//	Operator* m_operator,
	//	//GLfloat* paramCase,
	//	float m_speed,
	//	float m_deltaTime,
	//	ControllerInput* inputs);
	void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mode,
		SceneConstruction* Scene);


};

