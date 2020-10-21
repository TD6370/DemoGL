#include "Controllers.h"
#include "CoreSettings.h"

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

glm::vec3 GetPositionModelCursor(glm::mat4 p_projection, glm::mat4  p_view, glm::mat4 p_model, GLdouble mouseX, GLdouble mouseY, int m_widthWindow, int m_heightWindow)
{
	/*
	GLint viewport[4] = { 0.0 };
	GLdouble modelview[16] = { 0.0 };
	GLdouble projection[16] = { 0.0 };
	const float* pSourceV = (const float*)glm::value_ptr(p_view);
	for (int i = 0; i < 4; ++i)
		viewport[i] = pSourceV[i];

	const float* pSourceM = (const float*)glm::value_ptr(p_model);
	for (int i = 0; i < 16; ++i)
		modelview[i] = pSourceM[i];

	const float* pSourceP = (const float*)glm::value_ptr(p_projection);
	for (int i = 0; i < 16; ++i)
		projection[i] = pSourceP[i];
	*/

	//GLdouble mouseX, mouseY;
	//mouseX = m_MouseX;
	//mouseY = m_MouseY;
	

	vec3 vecPosMouseNear = vec3(mouseX, mouseY, 0.0);
	vec3 vecPosMouseFar = vec3(mouseX, mouseY, 1.0);
	//vec3 vecPosMouseNear = vec3(m_widthWindow / 2, m_heightWindow / 2, 0.0);
	//vec3 vecPosMouseFar = vec3(m_widthWindow / 2, m_heightWindow / 2, 1.0);

	glm::vec4 viewportVec(0.0f, 0.0f, m_widthWindow, m_heightWindow);

	// точка на ближайшей отсекающей плоскости
	vec3 posnear = glm::unProject(vecPosMouseNear, p_model, p_projection, viewportVec);
	// точка на дальней отсекающей плоскости
	vec3 posfar = glm::unProject(vecPosMouseFar, p_model, p_projection, viewportVec);
	//vec3 posnear = glm::unProject(vecPosMouseNear, p_view * p_model, p_projection, viewportVec);
	//vec3 posfar = glm::unProject(vecPosMouseFar, p_view * p_model, p_projection, viewportVec);
	//m_positionCursorModel = posnear;
	return posnear;

	/*
	vec3 mouseRay = posfar - posnear; // вектор под мышой
	vec3 norm = vec3(0.0, 0.0, 1.0);//нормаль к плоскости x=0;y=0;z=1;
	//float t = (n & (-mouseRay)) / (n & mouseRay);
	vec3 ti = (norm & (-mouseRay)) / (norm & mouseRay);
	//float t = (n * (-mouseRay)) / (n * mouseRay);

	vec3 pos = posnear + ti * mouseRay; // собственно точка пересечения
	*/
}

void MouseEvents(
	GLFWwindow* window,
	int m_widthWindow, int m_heightWindow,
	float m_deltaTime,
	Operator* p_operator,
	ControllerInput* p_inputs)
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
	p_operator->m_MouseX = xpos;
	p_operator->m_MouseY = ypos;

	bool isCeneter = false;
	int state = -1;
	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS)
	{
		//isCeneter = true;
		//p_operator->m_start = false;
		p_inputs->MBT = GLFW_MOUSE_BUTTON_2;
	}
	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		p_inputs->MBT = GLFW_MOUSE_BUTTON_1;
	}

	//----------------------------
	if (xpos >= m_widthWindow - 2) {
		p_operator->MouseOffset_x += m_widthWindow / 2;
		glfwSetCursorPos(window, m_widthWindow / 2, ypos);
		return;
	}
	if (xpos <= 0) {
		p_operator->MouseOffset_x -= m_widthWindow / 2;
		glfwSetCursorPos(window, m_widthWindow / 2, ypos);
		return;
	}
	if (ypos >= m_heightWindow - 2) {
		p_operator->MouseOffset_y += m_heightWindow / 2;
		glfwSetCursorPos(window, xpos, m_heightWindow / 2);
		return;
	}
	if (ypos <= 0) {
		p_operator->MouseOffset_y -= m_heightWindow / 2;
		glfwSetCursorPos(window, xpos, m_heightWindow / 2);
		return;
	}
	//----------------------------


	// Двигаем мышь в центр экрана
	if (!p_operator->m_start)
		glfwSetCursorPos(window, m_widthWindow / 2, m_heightWindow / 2);

	if (isCeneter) {
		p_operator->m_start = true;
		return;
	}

	//вычислим необходимые углы :
	horizontalAngle += mouseSpeed * m_deltaTime * float(m_widthWindow / 2 - xpos - p_operator->MouseOffset_x);
	verticalAngle += mouseSpeed * m_deltaTime * float(m_heightWindow / 2 - ypos - p_operator->MouseOffset_y);

	p_operator->VerticalAngle = verticalAngle;
	p_operator->HorizontalAngle = horizontalAngle;

	// направление : Преобразовываем сферические координаты в декартовы
	p_operator->m_direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	/*p_operator->m_directionCursor = glm::vec3(
		cos(verticalAngle - 0.2) * sin(horizontalAngle + 0.2),
		sin(verticalAngle - 0.2),
		cos(verticalAngle - 0.2) * cos(horizontalAngle + 0.2)
	);*/

	// Вектор «вправо»
	p_operator->m_right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Вектор «вверх»: перпендикуляр к направлению и к «вправо»
	p_operator->m_up = glm::cross(p_operator->m_right, p_operator->m_direction);

	p_operator->m_start = true;
}

void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mode,
	Operator*  p_Operator,
	GLfloat* paramCase,
	float m_speed,
	float m_deltaTime,
	ControllerInput* p_inputs)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Двигаемся вперед
	if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
		//if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		p_Operator->m_position += p_Operator->m_direction * m_deltaTime * m_speed;
	}
	// Двигаемся назад
	if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
		//if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		p_Operator->m_position -= p_Operator->m_direction * m_deltaTime * m_speed;
	}
	// Шаг вправо
	if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
		//if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		p_Operator->m_position += p_Operator->m_right * m_deltaTime * m_speed;
	}
	// Шаг влево
	if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
		//if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		p_Operator->m_position -= p_Operator->m_right * m_deltaTime * m_speed;
	}

	if ((key == GLFW_KEY_0) && action == GLFW_PRESS)
		*paramCase = 0;
	if ((key == GLFW_KEY_1) && action == GLFW_PRESS)
		*paramCase = 1;
	if ((key == GLFW_KEY_2) && action == GLFW_PRESS)
		*paramCase = 2;
	if ((key == GLFW_KEY_3) && action == GLFW_PRESS)
		*paramCase = 3;
	if ((key == GLFW_KEY_4) && action == GLFW_PRESS)
		*paramCase = 4;

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {

		//isPerspective = !isPerspective;
		//if (!isPerspective)
		//	camX = 0.0f;
	}

	p_inputs->Key = key;
}

