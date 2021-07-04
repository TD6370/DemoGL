#pragma once

#include "TransformModel.h"
#include "Controllers.h"
#include "ConfigBuffers.h"
#include "ConfigUniformArg.h"
#include "CoreSettings.h"
//#include "ConfigUniformArg.cpp"

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

glm::mat4 Transform(unsigned int index, int paramCase, bool IsNoTranf,
	glm::mat4 p_trans, 
	glm::vec3 p_postranslate,
	glm::vec3 p_angle,
	glm::vec3 p_size)
{
	glm::mat4 trans = glm::mat4(1.0f);
	//trans = p_trans;
	if (IsNoTranf)
		return trans;

	trans = glm::translate(trans, glm::vec3(p_postranslate.x, p_postranslate.y, p_postranslate.z));
	
	//trans = glm::rotate(trans, (glm::float32)p_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, (glm::float32)p_angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, (glm::float32)p_angle.y, glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, (glm::float32)p_angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
		
	if (p_size != vec3(0))
			trans = glm::scale(trans, p_size);

	return trans;
	//-----------------------------------------------------
			
	/*spirale = glm::translate(trans, glm::vec3(
		index * 2 * sin(glfwGetTime() * speed / index),
		0,
		index * 2 * cos(glfwGetTime() * speed / index)));*/

		//trans = glm::translate(trans, glm::vec3( (float)index*2, (float)index * 2, 0.0f));
		//trans = glm::translate(trans, glm::vec3((float)index*2* sin(glfwGetTime()), (float)index *2 * cos(glfwGetTime()), 0.0f));

	//index = .1f;
	/*float power = 1.f;
	float randomSignal = sin(cos(index) * power) + cos(index * power) * power;
	float signal = sin(index) + cos(index);*/

	/*
	//spirale
	trans = glm::translate(trans, glm::vec3(
		index * 2 * sin(glfwGetTime() * speed / index),
		randomSignal,
		//signal * 10,
		index * 2 * cos(glfwGetTime() * speed / index)));


	//salto
	trans = glm::translate(trans, glm::vec3(0.0f,
		sin((float)glfwGetTime()) * sin(index) + cos(index) * 10.5f,
		cos((float)glfwGetTime()) * sin(index) + cos(index) * 10.5f
	));
	*/
	//trans = glm::translate(trans, glm::vec3(p_postranslate.x, p_postranslate.y, p_postranslate.z));

	//trans = glm::translate(trans, glm::vec3(sin((float)glfwGetTime()), 0.0f, cos((float)glfwGetTime())));
	//trans = glm::translate(trans, glm::vec3(sin((float)speed), 0.0f, cos((float)speed)));

	//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	//trans = glm::translate(trans, glm::vec3(sin((float)glfwGetTime()), cos((float)glfwGetTime()), 0.0f)); //coleso
	//trans = glm::translate(trans, glm::vec3(0.0f, sin((float)glfwGetTime()), cos((float)glfwGetTime()))); //salto
	//trans = glm::translate(trans, glm::vec3(sin((float)glfwGetTime()), 0.0f, cos((float)glfwGetTime()))); //karusel
	//trans = glm::translate(trans, glm::vec3(	sin((float)glfwGetTime()) * 5.f, 1.0f, cos((float)glfwGetTime()) * 5.f	)); //karusel - radius
	//trans = glm::translate(trans, glm::vec3(sin((float)glfwGetTime() * 10.f) , 0.0f, cos((float)glfwGetTime() * 10.f) )); //karusel - speed

	/*trans = glm::translate(trans, glm::vec3(
		sin((float)glfwGetTime()) * paramCase,
		0.0f,
		cos((float)glfwGetTime()) * paramCase
	));*/

	/*
	if(paramCase < 1)
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	else if (paramCase < 2)
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
	else if (paramCase < 3)
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	else if (paramCase < 4)
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
	*/

	return trans;
}

/*
void GenMVP(GLuint shaderProgram,
	ConfigUniform confUniform,
	int m_widthWindow,
	int m_heightWindow,
	Operator* p_operator,
	Camera p_camera)
	*/
void TransformModel::GenMVP(
		int m_widthWindow,
		int m_heightWindow,
		Operator* p_operator,
		Camera* p_camera,
		CoreMVP* ConfigMVP)
{
	/*
	// Матрица проекции : 45° Угол обзора, 4:3 соотношение сторон, дальность видимости от 0.1  до 100
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Матрица камеры
	ViewMatrix = glm::lookAt(
		position,            // Положение камеры
		position + direction, // А вот сюда мы смотрим
		up
	);
	*/

	GLfloat far = p_operator->Far; //1000.f;

	// Проекционная матрица : 45&deg; поле обзора, 4:3 соотношение сторон, диапазон : 0.1 юнит <-> 100 юнитов
	glm::mat4 Projection;
	if (p_camera->isPerspective) {
		//Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		Projection = glm::perspective(p_operator->m_FoV, 4.0f / 3.0f, 0.1f, far);
	}
	else {
		GLfloat n = 1.0f;// +camX;
		GLfloat aspect = 4.0f / 3.0f;
		Projection = glm::ortho(-n, n, -n * aspect, n * aspect, 0.1f, 100.0f);
		//Projection = glm::ortho(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	}

	// Или, для ортокамеры
	/*
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Камера находится в мировых координатах (4,3,3)
		glm::vec3(0, 0, 0), // И направлена в начало координат
		glm::vec3(0, 1, 0)  // "Голова" находится сверху
	);
	*/
	/*
	glm::mat4 View = glm::lookAt(
		glm::vec3(camX, camY, camZ), // Камера находится в мировых координатах (4,3,3)
		//glm::vec3(4, 3, 3), // Камера находится в мировых координатах (4,3,3)
		glm::vec3(lookCamX, lookCamY, 0), // И направлена в начало координат
		glm::vec3(0, 1, 0)  // "Голова" находится сверху
	);
	*/

	glm::mat4 View = glm::lookAt(
		p_operator->m_position,            // Положение камеры
		p_operator->m_position + p_operator->m_direction, // направлениеs
		p_operator->m_up
	);

	// Матрица модели : единичная матрица (Модель находится в начале координат)
	glm::mat4 Model = glm::mat4(1.0f);  // Индивидуально для каждой модели

	// Итоговая матрица ModelViewProjection, которая является результатом перемножения наших трех матриц
	glm::mat4 MVP = Projection * View * Model; // Помните, что умножение матрицы производиться в обратном порядке
	//Второй шаг - передать это в GLSL :
	// Получить хэндл переменной в шейдере
	// Только один раз во время инициализации.
	//GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

	// Передать наши трансформации в текущий шейдер
	// Это делается в основном цикле, поскольку каждая модель будет иметь другую MVP-матрицу (как минимум часть M)
	//confUniform.SetMVP(MVP);

	////VIEW param
	//confUniform.SetView(View);

	////MODEL param
	//confUniform.SetModel(Model);

	ConfigMVP->Model = Model;
	ConfigMVP->View = View;
	ConfigMVP->MVP = MVP;
	ConfigMVP->Projection = Projection;

}

vec3 GetVectorForwardFace(CoreMVP* ConfigMVP, GLfloat lenght, Operator* operatorG) {
	vec4 vecPos = glm::inverse(ConfigMVP->View) * vec4(1);
	
	float offset = 1 / lenght;
	vec3 directionFace = glm::vec3(
		cos(operatorG->VerticalAngle - offset) * sin(operatorG->HorizontalAngle + offset),
		sin(operatorG->VerticalAngle - offset),
		cos(operatorG->VerticalAngle - offset) * cos(operatorG->HorizontalAngle + offset)
	);
	vec3 direction = directionFace * lenght;
	vec3 posFace = vec3(vecPos.x, vecPos.y, vecPos.z) + direction;
	return posFace;
}

vec3 GetVectorForwardFaceOffset(CoreMVP* ConfigMVP, GLfloat lenght, Operator* operatorG, vec3 posOffset) {
	
	vec4 vecPos = glm::inverse(ConfigMVP->View) * vec4(1);

	vecPos += vec4(operatorG->m_right * posOffset.x, 1);
	vecPos -= vec4(operatorG->m_up * posOffset.y, 1);

	//if(posOffset.x > 0)
	//	vecPos += vec4(operatorG->m_right * posOffset.x, 1);
	//else if (posOffset.x < 0)
	//	vecPos -= vec4(operatorG->m_right * posOffset.x, 1);

	//if (posOffset.y < 0)
	//	vecPos += vec4(operatorG->m_up * posOffset.y, 1);
	//else if(posOffset.y > 0)
	//	vecPos -= vec4(operatorG->m_up * posOffset.y, 1);


	float offset = 1 / lenght;

	vec3 directionFace = glm::vec3(
		cos(operatorG->VerticalAngle - offset) * sin(operatorG->HorizontalAngle + offset),
		sin(operatorG->VerticalAngle - offset),
		cos(operatorG->VerticalAngle - offset) * cos(operatorG->HorizontalAngle + offset)
	);
		
	vec3 direction = directionFace * lenght;
	vec3 posFace = vec3(vecPos.x, vecPos.y, vecPos.z) + direction;

	return posFace;
}

vec3 GetVectorForward(CoreMVP* ConfigMVP, GLfloat lenght, Operator* operatorG) {
	vec4 vecPos = glm::inverse(ConfigMVP->View) * vec4(1);
	vec3 direction = operatorG->m_direction * lenght;
	vec3 posForward = vec3(vecPos.x, vecPos.y, vecPos.z) + direction;
	return posForward;
}