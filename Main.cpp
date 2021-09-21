
#include "Main.h"

#include "ModelData.h"
#include "ObjectsTypes/ObjectData.h"
#include "Shaders.h"
#include "CreatorModelData.h"

#include "ConfigBuffers.h"
#include "Controllers.h"
#include "TransformModel.h"
#include "ConfigUniformArg.h"
#include "CoreSettings.h"
#include "WorldCollision.h"
#include "GeometryLib.h"
#include "SceneConstructor.h"

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
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

using glm::vec4;
using glm::vec3;
using glm::mat4;
using std::shared_ptr;

SceneConstructor* Scene;

struct LoopGame {
	double FPS = 30.0;// 60.0;
	double LimitFPS = 1.0 / FPS;
	double LastTime = glfwGetTime();
	double Timer = LastTime;
	double DeltaTime = 0;
	double NowTime = 0;
	int Frames = 0;
	int Updates = 0;
	double MS_PER_UPDATE = 0.05;// 0.1;
	double Lag = 0.0;
};


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Scene->Storage->Oper->m_FoV = glm::radians(Scene->Storage->Oper->m_initialFoV += yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Scene->Contrl->KeyInput(window, key, scancode, action, mode,
		Scene);
}

//The callback function receives the mouse button, button actionand modifier bits.
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//Scene->SetMouseEvents();
	Scene->SetMouseButtonEvents();
}

void DebugLoopGame(LoopGame& loopGame);
void SwitchLoopGameVersion(LoopGame& loopGame, float& lastParamCase);

void GetVersionOpenGl()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion =
		glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version : %s\n", glslVersion);
}

int main()
{
	Scene = new SceneConstructor();

	//Инициализация GLFW
	glfwInit();
	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//bool isFullscreen = true;
	bool isFullscreen = false;
	GLFWmonitor* monitor = isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
	GLFWwindow* window = glfwCreateWindow(Scene->m_widthWindow, Scene->m_heightWindow, "LearnOpenGL", monitor, nullptr);
	
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	
	//key events
	glfwSetKeyCallback(window, key_callback);
	//mouse events
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	//-- Cursor
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//Version OpenGL
	GetVersionOpenGl();

	//----------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	// Принимать фрагменты которые ближе к камере
	glDepthFunc(GL_LESS);
	// Не показываем треугольники которые смотрят не на камеру (отсечение обратных поверхностей)
	glEnable(GL_CULL_FACE);
	//----------------------------------------------------
	// Включаем блендинг
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_BLEND);
	//----------------------------------------------------

	Scene = new SceneConstructor(window);
	
	LoopGame loopGame;

	Scene->DeltaTime = 3.;
	Scene->VersionUpdate = 1;
	Scene->IsSpeedDeltaTime = true;
	float lastParamCase = Scene->Storage->Inputs->ParamCase;
	bool isDebugFPS = false;
	//bool 
	isDebugFPS = true;

	bool isNeedUpdate = false;

	while (!glfwWindowShouldClose(window))
	{
		if (isDebugFPS)
		{
			glfwPollEvents();
			SwitchLoopGameVersion(loopGame, lastParamCase);
		}

		//==========================
		if (Scene->VersionUpdate == 2)
		{
			loopGame.NowTime = glfwGetTime();
			loopGame.DeltaTime = loopGame.NowTime - loopGame.LastTime;
			loopGame.Lag += loopGame.DeltaTime;
			loopGame.LastTime = loopGame.NowTime;
			//Scene->DeltaTime = loopGame.DeltaTime;
			//Scene->DeltaTime = (loopGame.Lag / loopGame.MS_PER_UPDATE);

			//glfwPollEvents();

			while (loopGame.Lag >= loopGame.MS_PER_UPDATE) {
				
				glfwPollEvents();

				Scene->IsDeltaUpdateLogic = true;
				Scene->Update();

				if(isDebugFPS)
					loopGame.Updates++;

				loopGame.Lag -= loopGame.MS_PER_UPDATE;
			}
				
			// - Render at maximum possible frames
			Scene->DeltaTime = (loopGame.Lag / loopGame.MS_PER_UPDATE);
			Scene->IsDeltaUpdateLogic = false;
			Scene->Update();

			if (isDebugFPS)
				DebugLoopGame(loopGame);

			glfwSwapBuffers(window);
			continue;
		}
		//==========================
		if (Scene->VersionUpdate == 1) //==== NEW
		{
			// - Measure time
			loopGame.NowTime = glfwGetTime();
			loopGame.DeltaTime = (loopGame.NowTime - loopGame.LastTime) / loopGame.LimitFPS;
			loopGame.Lag += loopGame.DeltaTime;
			loopGame.LastTime = loopGame.NowTime;
			Scene->DeltaTime = loopGame.DeltaTime;

			// ******
			//update input events
			//glfwPollEvents();	//-- (hard) (Это лучший выбор при непрерывном рендеринге, как и в большинстве игр.)
			//glfwWaitEvents();	//-- (low) Если вам нужно только обновить содержимое окна при получении нового ввода, лучше выбрать 
			//glfwWaitEventsTimeout(1); /1

			//--- DEBUG freeze
			if (loopGame.Lag > 10) {
				loopGame.Lag = 0;
				std::cout << "GAME LOOP FREEZE LAG: " << loopGame.Lag << "\n";
			}
			if (loopGame.DeltaTime > 10) {
				loopGame.DeltaTime = 0;
				std::cout << "GAME LOOP FREEZE DeltaTime: " << loopGame.DeltaTime << "\n";
			}

			isNeedUpdate = false;

			while (loopGame.Lag >= 1.0) {
				
				if (!isNeedUpdate) {
					isNeedUpdate - true;
					Scene->IsDeltaUpdateLogic = true;
					Scene->Update();
				}

				loopGame.Updates++;
				loopGame.Lag--;
			}

			// - Render at maximum possible frames
			Scene->IsDeltaUpdateLogic = false;
			Scene->Update();

			if (isDebugFPS)
				DebugLoopGame(loopGame);

			glfwSwapBuffers(window);
			continue;
		}
		 
		if (Scene->VersionUpdate == 11) //==== Origin
		{
			  // - Measure time
			loopGame.NowTime = glfwGetTime();
			loopGame.DeltaTime = (loopGame.NowTime - loopGame.LastTime) / loopGame.LimitFPS;
			loopGame.Lag += loopGame.DeltaTime;
			loopGame.LastTime = loopGame.NowTime;
			Scene->DeltaTime = loopGame.DeltaTime;

			//--- DEBUG freeze
			if (loopGame.Lag > 50) {
				loopGame.Lag = 0;
				std::cout << "GAME LOOP FREEZE LAG: " << loopGame.Lag << "\n";
			}
			if (loopGame.DeltaTime > 10) {
				loopGame.DeltaTime = 0;
				std::cout << "GAME LOOP FREEZE DeltaTime: " << loopGame.DeltaTime << "\n";
			}
			//---------------

			// ******
			//update input events
			//glfwPollEvents();	//-- (hard) (Это лучший выбор при непрерывном рендеринге, как и в большинстве игр.)
			//glfwWaitEvents();	//-- (low) Если вам нужно только обновить содержимое окна при получении нового ввода, лучше выбрать 
			//glfwWaitEventsTimeout(1); /1

			while (loopGame.Lag >= 1.0) {

				glfwPollEvents();

				Scene->IsDeltaUpdateLogic = true;
				Scene->Update();

				loopGame.Updates++;
				loopGame.Lag--;
			}

			// - Render at maximum possible frames
			Scene->IsDeltaUpdateLogic = false;
			Scene->Update();
			
			if (isDebugFPS)
				DebugLoopGame(loopGame);

			glfwSwapBuffers(window);
			continue;
		}
		//==========================
		if (Scene->VersionUpdate == 0)
		{
			//update input events
			glfwPollEvents();

			Scene->Update();

			if (isDebugFPS) {
				loopGame.Updates++;
				DebugLoopGame(loopGame);
			}

			glfwSwapBuffers(window);
		}
		//==========================
	}

	glfwTerminate();

	return 0;
}


void DebugLoopGame(LoopGame& loopGame) {

	loopGame.Frames++;
	// - Reset after one second
	if (glfwGetTime() - loopGame.Timer > 1.0) {
		loopGame.Timer++;
		std::cout << Scene->VersionUpdate << 
			"FPS: " << loopGame.Frames << 
			" Updates:" << loopGame.Updates << 
			" DeltaTime:" << Scene->DeltaTime << 
			" MPU: " << loopGame.MS_PER_UPDATE << 
			std::endl;
		loopGame.Updates = 0;
		loopGame.Frames = 0;
	}
}

void SwitchLoopGameVersion(LoopGame& loopGame, float& lastParamCase) {

	int KeyUpTopVertex = GLFW_KEY_KP_ADD;
	int KeyDownTopVertex = GLFW_KEY_KP_SUBTRACT;
	if (Scene->Storage->Inputs->Action == GLFW_PRESS) {
		//double MS_PER_UPDATE = 0.1;
		if (Scene->Storage->Inputs->Key == KeyUpTopVertex)
			loopGame.MS_PER_UPDATE += 0.01;
		if (Scene->Storage->Inputs->Key == KeyDownTopVertex)
			loopGame.MS_PER_UPDATE -= 0.01;
	}

	if (lastParamCase == Scene->Storage->Inputs->ParamCase)
		return;
	lastParamCase = Scene->Storage->Inputs->ParamCase;

	if (Scene->Storage->Inputs->ParamCase == 1) {
		Scene->VersionUpdate = 0;
	}
	if (Scene->Storage->Inputs->ParamCase == 2) {
		Scene->VersionUpdate = 1;
		loopGame.Lag = 0.0;
	}
	if (Scene->Storage->Inputs->ParamCase == 3)
		Scene->VersionUpdate = 2;

	loopGame.Updates = 0;
	loopGame.Frames = 0;
	loopGame.Timer = loopGame.LastTime = glfwGetTime();
	loopGame.DeltaTime = 1;
	std::cout << "Scene->VersionUpdate = " << Scene->VersionUpdate << "\n----------------------\n";
}