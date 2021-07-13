
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
#include "SceneConstruction.h"

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

SceneConstruction* Scene;


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
	Scene = new SceneConstruction();

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

	Scene = new SceneConstruction(window);

	//double currentTime = 0, m_deltaTime = 0, m_lastFrame = 0;
	//float FPS = 60;
	//int32_t tickInteval = 1000 / FPS; // frequency in Hz to period in ms
	//double currentTime = 0, lastFrame = 0;

	///DWORD thisTick = GetTickCount();
	//float dt = float(thisTick - lastTick) * 0.001f;
	
	//---------------------
	//=========================
	/*while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		Scene->Update();

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;*/
	//====================
	//----------------------
	static double FPS = 30.0;// 60.0;
	static double limitFPS = 1.0 / FPS;
	double lastTime = glfwGetTime(), timer = lastTime;
	double deltaTime = 0, nowTime = 0;
	int frames = 0, updates = 0;

	while (!glfwWindowShouldClose(window))
	{
		if (Scene->VersionUpdate == 1)
		{
			//====	DT
			  // - Measure time
			nowTime = glfwGetTime();
			deltaTime += (nowTime - lastTime) / limitFPS;
			Scene->DeltaTime = deltaTime;
			lastTime = nowTime;

			// ******
			//update input events
			//glfwPollEvents();	//-- (hard) (Это лучший выбор при непрерывном рендеринге, как и в большинстве игр.)
			//glfwWaitEvents();	//-- (low) Если вам нужно только обновить содержимое окна при получении нового ввода, лучше выбрать 
			//glfwWaitEventsTimeout(1); /1

			while (deltaTime >= 1.0) {

				//TEST&&1
				glfwPollEvents();

				Scene->IsDeltaUpdateLogic = true;

				Scene->Update();

				updates++;
				deltaTime--;
			}

			// - Render at maximum possible frames
			Scene->IsDeltaUpdateLogic = false;
			Scene->Update();
			
			frames++;

			// - Reset after one second
			if (glfwGetTime() - timer > 1.0) {
				timer++;
				//std::cout << "FPS: " << frames << " Updates:" << updates << " DeltaTime:" << Scene->DeltaTime << std::endl;
				updates = 0, frames = 0;
			}

			glfwSwapBuffers(window);
		}
		//==========================
		if (Scene->VersionUpdate == 0)
		{
			nowTime = glfwGetTime();
			deltaTime += (nowTime - lastTime) / limitFPS;
			Scene->DeltaTime = deltaTime;
			lastTime = nowTime;

			Scene->DeltaTime = 1.;

			//update input events
			glfwPollEvents();

			Scene->Update();

			glfwSwapBuffers(window);
		}
		//==========================
	}

	glfwTerminate();

	return 0;
}
