#include "Controllers.h"
#include "CoreSettings.h"
#include "SceneConstruction.h"

#include "CreatorModelData.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//#include "gl_coreCpp_4_3.hpp"
// GLEW нужно подключать до GLFW.
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;

vec3 GetPositionModelCursor(glm::mat4 p_projection, glm::mat4  p_view, glm::mat4 p_model, GLdouble mouseX, GLdouble mouseY, int m_widthWindow, int m_heightWindow)
{
	vec3 vecPosMouseNear = vec3(mouseX, mouseY, 0.0);//---V
	vec3 vecPosMouseFar = vec3(mouseX, mouseY, 1.0);
	glm::vec4 viewportVec(0.0f, 0.0f, m_widthWindow, m_heightWindow);

	// точка на ближайшей отсекающей плоскости
	vec3 posnear = glm::unProject(vecPosMouseNear, p_model, p_projection, viewportVec);
	// точка на дальней отсекающей плоскости
	vec3 posfar = glm::unProject(vecPosMouseFar, p_model, p_projection, viewportVec);
	return posnear;
}

void Controllers::MouseButtonEvents(GLFWwindow* window, SceneConstruction* Scene)
{
	int state = -1;
	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS)
	{
		//isCeneter = true;
		//p_operator->m_start = false;
		Scene->Storage->Inputs->MBT = GLFW_MOUSE_BUTTON_2;
		Scene->Storage->Inputs->ActionMouse = state;
	}
	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		Scene->Storage->Inputs->MBT = GLFW_MOUSE_BUTTON_1;
		Scene->Storage->Inputs->ActionMouse = state;
		//Scene->Debug("MouseEvents Set MBT");
	}
}

void Controllers::MouseEvents(
	GLFWwindow* window,
	int m_widthWindow, int m_heightWindow,
	SceneConstruction* Scene)
{

	// горизонтальный угол : по -Z
	float horizontalAngle = 3.14f;
	// вертикальный угол : 0, смотрим на горизонт
	float verticalAngle = 0.0f;
	float mouseSpeed = 0.005f;

	//double currentTime = glfwGetTime();
	//m_deltaTime = float(currentTime- m_lastFrame);
	//m_lastFrame = currentTime;


	// получаем координаты курсора
	GLdouble xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	Scene->Storage->Oper->m_MouseX = xpos;
	Scene->Storage->Oper->m_MouseY = ypos;
	

	bool isCeneter = false;
	//int state = -1;
	//state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	//if (state == GLFW_PRESS)
	//{
	//	//isCeneter = true;
	//	//p_operator->m_start = false;
	//	Scene->Storage->Inputs->MBT = GLFW_MOUSE_BUTTON_2;
	//	Scene->Storage->Inputs->ActionMouse = state;
	//}
	//state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	//if (state == GLFW_PRESS)
	//{
	//	Scene->Storage->Inputs->MBT = GLFW_MOUSE_BUTTON_1;
	//	Scene->Storage->Inputs->ActionMouse = state;
	//	Scene->Debug("MouseEvents Set MBT");
	//}

	//----------------------------
	if (!Scene->Storage->SceneData->IsGUI) {
		if (xpos >= m_widthWindow - 2) {
			Scene->Storage->Oper->MouseOffset_x += m_widthWindow / 2;
			glfwSetCursorPos(window, m_widthWindow / 2, ypos);
			return;
		}
		if (xpos <= 0) {
			Scene->Storage->Oper->MouseOffset_x -= m_widthWindow / 2;
			glfwSetCursorPos(window, m_widthWindow / 2, ypos);
			return;
		}
		if (ypos >= m_heightWindow - 2) {
			Scene->Storage->Oper->MouseOffset_y += m_heightWindow / 2;
			glfwSetCursorPos(window, xpos, m_heightWindow / 2);
			return;
		}
		if (ypos <= 0) {
			Scene->Storage->Oper->MouseOffset_y -= m_heightWindow / 2;
			glfwSetCursorPos(window, xpos, m_heightWindow / 2);
			return;
		}
	}
	//----------------------------


	// Двигаем мышь в центр экрана
	if (!Scene->Storage->Oper->m_start)
		glfwSetCursorPos(window, m_widthWindow / 2, m_heightWindow / 2);

	if (isCeneter) {
		Scene->Storage->Oper->m_start = true;
		return;
	}

	//вычислим необходимые углы :
	horizontalAngle += mouseSpeed * Scene->m_deltaTime * float(m_widthWindow / 2 - xpos - Scene->Storage->Oper->MouseOffset_x);
	verticalAngle += mouseSpeed * Scene->m_deltaTime * float(m_heightWindow / 2 - ypos - Scene->Storage->Oper->MouseOffset_y);

	if (!Scene->Storage->SceneData->IsGUI) {


		Scene->Storage->Oper->VerticalAngle = verticalAngle;
		Scene->Storage->Oper->HorizontalAngle = horizontalAngle;

		// направление : Преобразовываем сферические координаты в декартовы
		Scene->Storage->Oper->m_direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Вектор «вправо»
		Scene->Storage->Oper->m_right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		// Вектор «вверх»: перпендикуляр к направлению и к «вправо»
		Scene->Storage->Oper->m_up = glm::cross(Scene->Storage->Oper->m_right, Scene->Storage->Oper->m_direction);

		Scene->Storage->Oper->m_start = true;
	}
}


void Controllers::KeyInput(GLFWwindow* window, int key, int scancode, int action, int mode,
	SceneConstruction* Scene)
{
	float m_speed = Scene->m_speed;
	float m_deltaTime = Scene->m_deltaTime;
	//TEST***
	m_speed = Scene->DeltaTime;

	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		Scene->Storage->SceneData->IsGUI = !Scene->Storage->SceneData->IsGUI; //Tab - user interface
	}

	if (!Scene->Storage->SceneData->IsGUI) {
		// Двигаемся вперед
		if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
			//if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			Scene->Storage->Oper->m_position += Scene->Storage->Oper->m_direction * m_deltaTime * m_speed;
		}
		// Двигаемся назад
		if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
			//if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			Scene->Storage->Oper->m_position -= Scene->Storage->Oper->m_direction * m_deltaTime * m_speed;
		}
		// Шаг вправо
		if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
			//if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			Scene->Storage->Oper->m_position += Scene->Storage->Oper->m_right * m_deltaTime * m_speed;
		}
		// Шаг влево
		if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
			//if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			Scene->Storage->Oper->m_position -= Scene->Storage->Oper->m_right * m_deltaTime * m_speed;
		}
	}
	
	if ((key == GLFW_KEY_0) && action == GLFW_PRESS)
		Scene->Storage->Inputs->ParamCase = 0;
	if ((key == GLFW_KEY_1) && action == GLFW_PRESS)
		Scene->Storage->Inputs->ParamCase = 1;
	if ((key == GLFW_KEY_2) && action == GLFW_PRESS)
		Scene->Storage->Inputs->ParamCase = 2;
	if ((key == GLFW_KEY_3) && action == GLFW_PRESS)
		Scene->Storage->Inputs->ParamCase = 3;
	if ((key == GLFW_KEY_4) && action == GLFW_PRESS)
		Scene->Storage->Inputs->ParamCase = 4;
		//*paramCase = 4;

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {

		//isPerspective = !isPerspective;
		//if (!isPerspective)
		//	camX = 0.0f;
	}

	Scene->Storage->Inputs->Key = key;
	Scene->Storage->Inputs->Action = action;
}

