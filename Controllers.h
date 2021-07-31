#pragma once
#include "CoreSettings.h"
#include "SceneConstructor.h"

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//OpenGL Mathematics
#include <glm/glm.hpp>
//using glm::vec3;
using std::map;

struct ControllerInput;

class SceneConstructor;


glm::vec3 GetPositionModelCursor(glm::mat4 p_projection, glm::mat4  p_view, glm::mat4 p_model, GLdouble mouseX, GLdouble mouseY, int m_widthWindow, int m_heightWindow);

enum Laguages { LangNone, LangRus, LangEng };

class Controllers {
private:

		map<string, int>& m_mapAlphabetInputs;
		map<string, int>& m_mapAlphabetInputsEng;

		map<int, string>* mapAlphabetRusKeys;
		map<int, string>* mapAlphabetEngKeys;
		map<int, string>* mapCurrentAlphabetKeys = nullptr;
		Laguages m_currentLaguage = LangNone;

public:



	Controllers(map<string, int>& mapAlphabetInputs, map<string, int>& mapAlphabetInputsEng) :
		m_mapAlphabetInputs(mapAlphabetInputs), m_mapAlphabetInputsEng(mapAlphabetInputsEng)
	{
		FillAlphabet();
	};

	//glm::vec3 GetPositionModelCursor(glm::mat4 p_projection, glm::mat4  p_view, glm::mat4 p_model, GLdouble mouseX, GLdouble mouseY, int m_widthWindow, int m_heightWindow);

	//void MouseEvents(GLFWwindow* window,
	//	int m_widthWindow, int m_heightWindow,
	//	float m_deltaTime,
	//	Operator* p_Operator,
	//	ControllerInput* p_inputs);
	void MouseEvents(GLFWwindow* window,
		int m_widthWindow, int m_heightWindow,
		SceneConstructor* Scene);

	void MouseButtonEvents(GLFWwindow* window, SceneConstructor* Scene);

	//void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mode,
	//	Operator* m_operator,
	//	//GLfloat* paramCase,
	//	float m_speed,
	//	float m_deltaTime,
	//	ControllerInput* inputs);
	void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mode,
		SceneConstructor* Scene);

	void FillAlphabet();

	void SwitchLanguage();

	string GetSymbol(int keyIndex);
};

