#include "Controllers.h"
#include "CoreSettings.h"
#include "SceneConstructor.h"

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

void Controllers::MouseButtonEvents(GLFWwindow* window, SceneConstructor* Scene)
{
	m_stateMBE = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (m_stateMBE == GLFW_PRESS)
	{
		Scene->Storage->Inputs->MBT = GLFW_MOUSE_BUTTON_2;
		Scene->Storage->Inputs->ActionMouse = m_stateMBE;
		Scene->Storage->Inputs->IsReading = false;
	}
	else {
		m_stateMBE = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (m_stateMBE == GLFW_PRESS)
		{
			Scene->Storage->Inputs->MBT = GLFW_MOUSE_BUTTON_1;
			Scene->Storage->Inputs->ActionMouse = m_stateMBE;
			Scene->Storage->Inputs->IsReading = false;
		}
	}
}

void Controllers::MouseEvents(
	GLFWwindow* window,
	int m_widthWindow, int m_heightWindow,
	SceneConstructor* Scene)
{
	glfwGetCursorPos(window, &m_xpos, &m_ypos);
	Scene->Storage->Oper->m_MouseX = m_xpos;
	Scene->Storage->Oper->m_MouseY = m_ypos;

	//----------------------------
	if (!Scene->Storage->SceneData->IsGUI) {
		if (m_xpos >= m_widthWindow - 2) {
			Scene->Storage->Oper->MouseOffset_x += m_widthWindow / 2;
			glfwSetCursorPos(window, m_widthWindow / 2, m_ypos);
			return;
		}
		if (m_xpos <= 0) {
			Scene->Storage->Oper->MouseOffset_x -= m_widthWindow / 2;
			glfwSetCursorPos(window, m_widthWindow / 2, m_ypos);
			return;
		}

		float limitVert = m_heightWindow / 1.5;
		if (m_ypos >= m_heightWindow - 1) {
			
			//--- DOWN clip
			/*if (Scene->Storage->Oper->MouseOffset_y > (m_heightWindow/2))
				Scene->Storage->Oper->MouseOffset_y -= (m_heightWindow * 5);
			else
				Scene->Storage->Oper->MouseOffset_y += m_heightWindow / 2;
			glfwSetCursorPos(window, xpos, m_heightWindow / 2);
			return;
			*/

			//--- DOWN limit
			Scene->Storage->Oper->MouseOffset_y += m_heightWindow / 2;
			if (Scene->Storage->Oper->MouseOffset_y > (limitVert)) {
				Scene->Storage->Oper->MouseOffset_y = (limitVert);
				glfwSetCursorPos(window, m_xpos, m_heightWindow);
			}else
				glfwSetCursorPos(window, m_xpos, m_heightWindow / 2);

		}
		if (m_ypos <= 0) {

			//--- UP clip
			/*if (Scene->Storage->Oper->MouseOffset_y < -(m_heightWindow/2))
				Scene->Storage->Oper->MouseOffset_y += m_heightWindow * 5;
			else
				Scene->Storage->Oper->MouseOffset_y -= m_heightWindow / 2;
			glfwSetCursorPos(window, xpos, m_heightWindow / 2);
			return;
			*/

			//--- UP limit
			Scene->Storage->Oper->MouseOffset_y -= m_heightWindow / 2;
			if (Scene->Storage->Oper->MouseOffset_y < -(limitVert)) {
				Scene->Storage->Oper->MouseOffset_y = -(limitVert);
				glfwSetCursorPos(window, m_xpos, 0);
			}else
				glfwSetCursorPos(window, m_xpos, m_heightWindow / 2);
		}
	}
	//----------------------------

	// Двигаем мышь в центр экрана
	if (!Scene->Storage->Oper->m_start)
		glfwSetCursorPos(window, m_widthWindow / 2, m_heightWindow / 2);
		
	if (!Scene->Storage->SceneData->IsGUI) {

		//вычислим необходимые углы :
		m_horizontalAngle = m_mouseSpeed * Scene->m_deltaTime * float(m_widthWindow / 2 - m_xpos - Scene->Storage->Oper->MouseOffset_x);
		m_verticalAngle = m_mouseSpeed * Scene->m_deltaTime * float(m_heightWindow / 2 - m_ypos - Scene->Storage->Oper->MouseOffset_y);

		Scene->Storage->Oper->VerticalAngle = m_verticalAngle;
		Scene->Storage->Oper->HorizontalAngle = m_horizontalAngle;

		// направление : Преобразовываем сферические координаты в декартовы
		Scene->Storage->Oper->m_direction = glm::vec3(
			cos(m_verticalAngle) * sin(m_horizontalAngle),
			sin(m_verticalAngle),
			cos(m_verticalAngle) * cos(m_horizontalAngle)
		);

		// Вектор «вправо»
		Scene->Storage->Oper->m_right = glm::vec3(
			sin(m_horizontalAngle - 3.14f / 2.0f),
			0,
			cos(m_horizontalAngle - 3.14f / 2.0f)
		);

		// Вектор «вверх»: перпендикуляр к направлению и к «вправо»
		Scene->Storage->Oper->m_up = glm::cross(Scene->Storage->Oper->m_right, Scene->Storage->Oper->m_direction);

		Scene->Storage->Oper->m_start = true;
	}
}

//	Macros
//#define 	GLFW_MOD_SHIFT   0x0001
//#define 	GLFW_MOD_CONTROL   0x0002
//#define 	GLFW_MOD_ALT   0x0004
//#define 	GLFW_MOD_SUPER   0x0008
//#define 	GLFW_MOD_CAPS_LOCK   0x0010
//#define 	GLFW_MOD_NUM_LOCK   0x0020

void Controllers::KeyInput(GLFWwindow* window, int key, int scancode, int action, int mode,
	SceneConstructor* Scene)
{
	if (action != GLFW_REPEAT && action != GLFW_PRESS && mode == 0)
		return;
		
	float m_speed = Scene->m_speed;
	float m_deltaTime = Scene->m_deltaTime;
	switch (key)
	{
		// Двигаемся вперед
		case GLFW_KEY_UP:
		case GLFW_KEY_W:
			if (!Scene->Storage->SceneData->IsGUI)
				Scene->Storage->Oper->m_position += Scene->Storage->Oper->m_direction * m_deltaTime * m_speed;
			break;
		// Двигаемся назад
		case GLFW_KEY_DOWN:
		case GLFW_KEY_S:
			if (!Scene->Storage->SceneData->IsGUI)
				Scene->Storage->Oper->m_position -= Scene->Storage->Oper->m_direction * m_deltaTime * m_speed;	
			break;
		// Шаг вправо
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_D:
			if (!Scene->Storage->SceneData->IsGUI)
				Scene->Storage->Oper->m_position += Scene->Storage->Oper->m_right * m_deltaTime * m_speed;
			break;
		// Шаг влево
		case GLFW_KEY_LEFT:
		case GLFW_KEY_A:
			if (!Scene->Storage->SceneData->IsGUI)
				Scene->Storage->Oper->m_position -= Scene->Storage->Oper->m_right * m_deltaTime * m_speed;
			break;

		case GLFW_KEY_ESCAPE:
			//ESC - Exit
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_TAB:
			//Tab - user interface
			Scene->Storage->SceneData->IsGUI = !Scene->Storage->SceneData->IsGUI; 
			break;
		case GLFW_KEY_LEFT_ALT:
			// -- swith language
			if (mode == GLFW_MOD_SHIFT)
				SwitchLanguage();
			break;
		case GLFW_KEY_0:
			Scene->Storage->Inputs->ParamCase = 0;
			break;
		case GLFW_KEY_1:
			Scene->Storage->Inputs->ParamCase = 1;
			break;
		case GLFW_KEY_2:
			Scene->Storage->Inputs->ParamCase = 2;
			break;
		case GLFW_KEY_3:
			Scene->Storage->Inputs->ParamCase = 3;
			break;
		case GLFW_KEY_4:
			Scene->Storage->Inputs->ParamCase = 4;
			break;
		case GLFW_KEY_5:
			Scene->Storage->Inputs->ParamCase = 5;
			break;
		case GLFW_KEY_6:
			Scene->Storage->Inputs->ParamCase = 6;
			break;
		case GLFW_KEY_7:
			Scene->Storage->Inputs->ParamCase = 7;
			break;
		case GLFW_KEY_8:
			Scene->Storage->Inputs->ParamCase = 8;
			break;
		case GLFW_KEY_9:
			Scene->Storage->Inputs->ParamCase = 9;
			break;
		default:
			break;
	}

	Scene->Storage->Inputs->Key = key;
	Scene->Storage->Inputs->Action = action;
	Scene->Storage->Inputs->Mode = mode;
	Scene->Storage->Inputs->IsReading = false;
}

void Controllers::FillAlphabet() {

	map<string, int> m_mapAlphabetNumInputs = map<string, int>{
		{" ",30},
		{"1",31},
		{"2",32},
		{"3",33},
		{"4",34},
		{"5",35},
		{"6",36},
		{"7",37},
		{"8",38},
		{"9",39},
		{"0",40},
		{"+",41},
		{"-",42},
		{"*",43},
		{"/",44},
		{"=",45},
		{"<,",46},
		{">",47},
		{"?",48}
	};

	m_mapAlphabetInputs = map<string, int>{
		{"а",0},
		{"б",1},
		{"в",2},
		{"г",3},
		{"д",4},
		{"е",5},
		{"ж",6},
		{"з",7},
		{"и",8},
		{"й",8},
		{"к",9},
		{"л",10},
		{"м",11},
		{"н",12},
		{"о",13},
		{"п",14},
		{"р",15},
		{"с",16},
		{"т",17},
		{"у",18},
		{"ф",19},
		{"х",20},
		{"ц",21},
		{"ч",22},
		{"ш",23},
		{"ш",24},
		{"ь",25},
		{"ы",26},
		{"э",27},
		{"ю",28},
		{"я",29},
	};

	m_mapAlphabetInputsEng = map<string, int>{
		{"q",0},
		{"w",1},
		{"e",2},
		{"r",3},
		{"t",4},
		{"y",5},
		{"u",6},
		{"i",7},
		{"o",8},
		{"p",9},
		{"a",10},
		{"s",11},
		{"d",12},
		{"f",13},
		{"g",14},
		{"h",15},
		{"j",16},
		{"k",17},
		{"l",18},
		{"z",19},
		{"x",20},
		{"c",21},
		{"v",22},
		{"b",23},
		{"n",24},
		{"m",25},
		{".",26},
		{",",27},
		{"!",28},
		{":",29},
	};
	
	for (auto item : m_mapAlphabetNumInputs)
	{
		m_mapAlphabetInputs.insert(item);
		m_mapAlphabetInputsEng.insert(item);
	}

//#define 	GLFW_KEY_KP_DECIMAL   330
//#define 	GLFW_KEY_KP_DIVIDE   331
//#define 	GLFW_KEY_KP_MULTIPLY   332
//#define 	GLFW_KEY_KP_SUBTRACT   333
//#define 	GLFW_KEY_KP_ADD   334
//#define GLFW_KEY_LEFT_BRACKET   91 /* [ */
//#define GLFW_KEY_BACKSLASH   92 /* \ */
//#define GLFW_KEY_RIGHT_BRACKET   93 /* ] */
//#define GLFW_KEY_GRAVE_ACCENT   96 /* ` */

	map<int, string> mapAlphabetNumKeys = map<int, string>{
		{GLFW_KEY_KP_0 ,"0"},
		{GLFW_KEY_KP_1 ,"1"},
		{GLFW_KEY_KP_2 ,"2"},
		{GLFW_KEY_KP_3 ,"3"},
		{GLFW_KEY_KP_4 ,"4"},
		{GLFW_KEY_KP_5 ,"5"},
		{GLFW_KEY_KP_6 ,"6"},
		{GLFW_KEY_KP_7 ,"7"},
		{GLFW_KEY_KP_8 ,"8"},
		{GLFW_KEY_KP_9 ,"9"},
		{GLFW_KEY_0 ,"0"},
		{GLFW_KEY_1 ,"1"},
		{GLFW_KEY_2 ,"2"},
		{GLFW_KEY_3 ,"3"},
		{GLFW_KEY_4 ,"4"},
		{GLFW_KEY_5 ,"5"},
		{GLFW_KEY_6 ,"6"},
		{GLFW_KEY_7 ,"7"},
		{GLFW_KEY_8 ,"8"},
		{GLFW_KEY_9 ,"9"},
		{GLFW_KEY_KP_ADD ,"+"},
		{GLFW_KEY_KP_SUBTRACT ,"-"},	//GLFW_KEY_MINUS
		{GLFW_KEY_KP_MULTIPLY ,"*"},
		{GLFW_KEY_KP_DIVIDE ,"/"},
		{GLFW_KEY_EQUAL ,"="},
		//{GLFW_KEY_COMMA ,"<"}, //GLFW_KEY_COMMA	","
		//{GLFW_KEY_PERIOD ,">"},	//GLFW_KEY_PERIOD  "."
		{GLFW_KEY_SLASH  ,"?"},	//GLFW_KEY_SLASH   "/"
		{GLFW_KEY_SPACE ," "},
		//{ ,""},
	};

	mapAlphabetRusKeys = new map<int, string>{
		{GLFW_KEY_Q ,"й"},
		{GLFW_KEY_W ,"ц"},
		{GLFW_KEY_E ,"у"},
		{GLFW_KEY_R ,"к"},
		{GLFW_KEY_T ,"е"},
		{GLFW_KEY_Y ,"н"},
		{GLFW_KEY_U ,"г"},
		{GLFW_KEY_I ,"ш"},
		{GLFW_KEY_P ,"з"},
		{GLFW_KEY_LEFT_BRACKET ,"х"},
		{GLFW_KEY_A ,"ф"},
		{GLFW_KEY_S ,"ы"},
		{GLFW_KEY_D ,"в"},
		{GLFW_KEY_F ,"а"},
		{GLFW_KEY_G ,"п"},
		{GLFW_KEY_H ,"р"},
		{GLFW_KEY_J ,"о"},
		{GLFW_KEY_K ,"л"},
		{GLFW_KEY_L ,"д"},
		{GLFW_KEY_SEMICOLON ,"ж"},	//GLFW_KEY_APOSTROPHE   39 /* ' */	GLFW_KEY_SEMICOLON   59 /* ; */
		{GLFW_KEY_APOSTROPHE ,"э"},
		{GLFW_KEY_Z ,"я"},
		{GLFW_KEY_X ,"ч"},
		{GLFW_KEY_C ,"с"},
		{GLFW_KEY_V ,"м"},
		{GLFW_KEY_B ,"и"},
		{GLFW_KEY_N ,"т"},
		{GLFW_KEY_M ,"ь"},
		{GLFW_KEY_COMMA ,"б"},	//GLFW_KEY_COMMA   44 /* , */	GLFW_KEY_PERIOD   46 /* . */
		{GLFW_KEY_PERIOD ,"ю"},
		//{GLFW_KEY_SPACE ," "},
	};
		
	mapAlphabetEngKeys = new map<int, string>{
		{GLFW_KEY_Q ,"q"},
		{GLFW_KEY_W ,"w"},
		{GLFW_KEY_E ,"e"},
		{GLFW_KEY_R ,"r"},
		{GLFW_KEY_T ,"t"},
		{GLFW_KEY_Y ,"y"},
		{GLFW_KEY_U ,"u"},
		{GLFW_KEY_I ,"i"},
		{GLFW_KEY_O ,"o"},
		{GLFW_KEY_P ,"p"},
		{GLFW_KEY_A ,"a"},
		{GLFW_KEY_S ,"s"},
		{GLFW_KEY_D ,"d"},
		{GLFW_KEY_F ,"f"},
		{GLFW_KEY_G ,"g"},
		{GLFW_KEY_H ,"h"},
		{GLFW_KEY_J ,"j"},
		{GLFW_KEY_K ,"k"},
		{GLFW_KEY_L ,"l"},
		{GLFW_KEY_Z ,"z"},
		{GLFW_KEY_X ,"x"},
		{GLFW_KEY_C ,"c"},
		{GLFW_KEY_V ,"v"},
		{GLFW_KEY_B ,"b"},
		{GLFW_KEY_N ,"n"},
		{GLFW_KEY_M ,"m"},
		/*{".",26},
		{",",27},
		{"!",28},
		{":",29},*/
		{GLFW_KEY_PERIOD ,"."},  //GLFW_KEY_PERIOD   46 /* . */
		{GLFW_KEY_COMMA ,","},	//GLFW_KEY_COMMA   44 /* , */	
		{GLFW_KEY_SEMICOLON ,":"},	//	GLFW_KEY_SEMICOLON   59 /* ; */
		//{GLFW_KEY_ ,"!"},

		/*{GLFW_KEY_ ,""},
		{GLFW_KEY_ ,""},*/

	};

	for (auto item : mapAlphabetNumKeys)
	{
		mapAlphabetRusKeys->insert(item);
		mapAlphabetEngKeys->insert(item);
	}

	SwitchLanguage();
}

void Controllers::SwitchLanguage() {

	if (m_currentLaguage == LangRus) 
	{
		m_currentLaguage = LangEng;
		mapCurrentAlphabetKeys = mapAlphabetEngKeys;
	}
	else 
	{
		m_currentLaguage = LangRus;
		mapCurrentAlphabetKeys = mapAlphabetRusKeys;
	}
}

string Controllers::GetSymbol(int keyIndex) {

	map <int, string> ::iterator it;
	it = mapCurrentAlphabetKeys->find(keyIndex);
	if (it != mapCurrentAlphabetKeys->end())
	{
		return (*mapCurrentAlphabetKeys)[keyIndex];
	}
	return "";
}