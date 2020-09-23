#pragma once
#include "CoreSettings.h"

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//OpenGL Mathematics
#include <glm/glm.hpp>
//using glm::vec3;

glm::vec3 GetPositionModelCursor(glm::mat4 p_projection, glm::mat4  p_view, glm::mat4 p_model, GLdouble mouseX, GLdouble mouseY, int m_widthWindow, int m_heightWindow);

void MouseEvents(GLFWwindow* window,
	int m_widthWindow, int m_heightWindow,
	float m_deltaTime,
	Operator* p_Operator);

void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mode,
	Operator* m_operator,
	GLfloat* paramCase,
	float m_speed,
	float m_deltaTime);




