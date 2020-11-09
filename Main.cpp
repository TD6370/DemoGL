
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


//--- CS
const GLchar* pathShaderVertex = "basic.vert";
const GLchar* pathShaderFrag = "basic.frag";
const char* pathTextureTest = "./Textures/testTexture.bmp";
const char* pathModel3D = "./Models3D/monkey.obj";

int m_widthWindow = 1024;
int m_heightWindow = 768;

Camera m_cameraG;

GLint indicesSize;

float m_speed = 1.0f; //3.0f = 3  в секунду
float m_deltaTime = 0.3f;//0.3f
float m_lastFrame = 0.0f;

Operator m_OperatorG;

Lighting m_Lighting;

CreatorModelData* Storage;

ControllerInput Inputs;

SceneConstruction Scene;

SceneParam m_SceneParam;

TransformModel m_transformModel;

Controllers m_controllers;

//---E_CS

void LoadDataModel()
{
	//bool isGen = true;
	bool isGen = false;
	//Storage = CreatorModelData();
	Storage = new CreatorModelData();
	Storage->SceneParam = &m_SceneParam;
	if (isGen)
	{
		Storage->LoadModels();
		Storage->GenerateObjects();
	}
	else {
		Storage->Load();
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_OperatorG.m_FoV = glm::radians(m_OperatorG.m_initialFoV += yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	m_controllers.KeyInput(window, key, scancode, action, mode,
		&m_OperatorG,
		&m_cameraG.paramCase,
		m_speed, m_deltaTime, 
		&Inputs);
}


void SetMouseEvents(GLFWwindow* window)
{
	m_controllers.MouseEvents(window,	m_widthWindow, m_heightWindow, m_deltaTime, &m_OperatorG, &Inputs);
}


void CreateMVP(CoreMVP* ConfigMVP) {
	
	m_transformModel.GenMVP(m_widthWindow,
		m_heightWindow,
		&m_OperatorG,
		&m_cameraG,
		ConfigMVP);
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

void ClearScene() {
	GLfloat red = 0.1f;
	GLfloat blue = 0.2f;
	GLfloat green = 0.3f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(red, blue, green, 1.0f);
}

void DrawGraph(GLuint shaderProgram, std::shared_ptr<ModelData> model)
{
	//Mode fill 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	bool isIndex = model->IsIndex;
	GLint trianglesCount = model->TrianglesCount;

	if (isIndex) {
		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, trianglesCount * 3);
	//glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//--normal
	/*glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, &out_normals[0]);
	glDisableClientState(GL_NORMAL_ARRAY);*/

	glBindVertexArray(0);
}

int main()
{
	//----------------------------- SC
	m_cameraG = Camera();
	m_OperatorG = Operator();
	m_Lighting = Lighting();
	//Scene = SceneConstruction();
	m_transformModel = TransformModel();
	m_controllers = Controllers();
	//--------- E_CS

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
	bool isFullscreen = m_OperatorG.IsFullscreen;
	GLFWmonitor* monitor = isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
	GLFWwindow* window = glfwCreateWindow(m_widthWindow, m_heightWindow, "LearnOpenGL", monitor, nullptr);
	
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
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//----------------------------------------------------
	//m_OperatorG,
	//m_cameraG
	//-----------


	Scene = SceneConstruction(window);

	//----------------------------- SC
	LoadDataModel();

	m_Lighting.positionLight = vec3(0, 0, 0);

	CoreMVP ConfigMVP = CoreMVP();

	int prevIndexModel = -1;
	const char* prevModelTexture = "";
	const char* prevModelModel3D = "";
	GLuint shaderProgram;
	bool isUpdate = false;
	GLuint last_VAO = 0;

	std::shared_ptr <ObjectData> object;
	std::shared_ptr<ModelData> model;

	bool test_isFerst = true;
	vec3 SavePos;

	int countObjects = Storage->SceneObjectsLastIndex;
	bool isTextureRepeat = false;
	//----------------------------- E_SC

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ClearScene();

		SetMouseEvents(window);

		CreateMVP(&ConfigMVP);
		
		for (int i = 0; i < countObjects + 1; i++)
		{
			object = Storage->GetObjectPrt(i);
			model = object->ModelPtr;

			//----- Start
			if (prevModelTexture != model->PathTexture || prevModelModel3D != model->PathModel3D)
			{
				isUpdate = true;
				prevModelTexture = model->PathTexture;
				prevModelModel3D = model->PathModel3D;
			}
			else {
				//printf("");
			}

			//-------------------- Update texture UV
			bool isTextRepeat = object->IsTextureRepeat;
			if (isTextureRepeat && !isTextRepeat)
			{
				object->ModelPtr->UpdateBufferUV();
				isTextureRepeat = false;
			}
			if (isTextRepeat) {
				isTextureRepeat = isTextRepeat;
				object->UpdateTextureUV();
			}
			//--------------------

			if (isUpdate) {
				shaderProgram = model->ShaderProgram;
				glUseProgram(shaderProgram);
				last_VAO = model->VAO;
			}

			Storage->Camera = &m_cameraG;
			Storage->MVP = &ConfigMVP;
			Storage->Operator = &m_OperatorG;
			Storage->Inputs = &Inputs;
			Storage->SceneParam = &m_SceneParam;
			object->Action();

			//TEST
			if (object->TypeObj == Block) 
				object->SetMesh();
			else if (isUpdate)
				object->SetMesh();

			//if (isUpdate)
			//	object->SetMesh();

			glBindVertexArray(model->VAO);

			if (isUpdate)
				model->SetModelInBuffer(isUpdate);

			//-------------------- Set color
			//model->ConfUniform.SetColor(vec3(0, 0, 0), true);
			model->ConfUniform.SetColor(object->Color);
			//---------------------- Set param Case
			model->ConfUniform.SetParamCase(m_cameraG.paramCase);
			//---------------------- Set MVP
			model->ConfUniform.SetMVP(ConfigMVP.MVP);

			//VIEW param
			model->ConfUniform.SetView(ConfigMVP.View);

			//MODEL param
			model->ConfUniform.SetModel(ConfigMVP.Model);


			//---------------- Marker Plane
			if (object->Name == "Plane") {
				vec3 vectorsParams[10];

				model->ConfUniform.SetParamCase(55);

				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				vector<string> checkedZona;
				//vector<int> indexesVert = Storage.Clusters->GetVertexPolygonFromObject(objectObserver->Index, checkedZona);
				//int indP = 0;
				//for (const int vertInd : indexesVert)
				//{
				//	//vec3 pos = object->GetVertexPosition(vertInd);
				//	vec3 pos = object->ModelPtr->Vertices[vertInd];
				//	vectorsParams[indP++] = pos;
				//	if (indP == 10)
				//		break;
				//}

				/*vector<int> indexesPlanes = Storage.Clusters->GetIndexPlanePolygonFromObject(objectObserver->Index, checkedZona);
				int indP = 0;
				for (const int planeInd : indexesPlanes)
				{
					shared_ptr<Plane> planeV = object->GetPlanePrt(planeInd);
					vectorsParams[indP++] = planeV->V0;
					vectorsParams[indP++] = planeV->V1;
					vectorsParams[indP++] = planeV->V2;
					if (indP == 9)
						break;
				}

				model->ConfUniform.SetVectorsParams(vectorsParams);*/

				if(objectObserver->PlaneDownIndex != -1)
				{
					shared_ptr<Plane> planeV = object->GetPlanePrt(objectObserver->PlaneDownIndex);
					vectorsParams[0] = planeV->V0;
					vectorsParams[1] = planeV->V1;
					vectorsParams[1] = planeV->V2;
					model->ConfUniform.SetVectorsParams(vectorsParams);
				}
			}

			//--- Target - cursor
			/*if (object->Name == "Mon") {
				std::shared_ptr <ObjectData> objectObserver = Storage.GetObjectPrt("Box");
				vec3 target = objectObserver->Postranslate;
				object->Target = vec3(target.x, object->Postranslate.y, target.z);
			}*/
			
			//--- Object Mon - cursor -- !!!!!!
			if (object->Name == "Mon" && m_cameraG.paramCase == 2) {
				vec4 vecPos = glm::inverse(ConfigMVP.View) * vec4(1);
				//vec3 posCursorObject = vec3(vecPos.x, vecPos.y, vecPos.z) + m_OperatorG.m_direction * 25.f;
				vec3 posCursorObject = vec3(vecPos.x, object->Postranslate.y, vecPos.z) + m_OperatorG.m_direction * 20.f;
				object->Postranslate = posCursorObject;
			}

			//--- Mouse camera object
			/*if (object->Name == "Box") {
				vec4 vecPos = glm::inverse(ConfigMVP.View) * vec4(1);
				vec3 posCursorObject = vec3(vecPos.x, vecPos.y, vecPos.z) + m_OperatorG.m_direction * 25.f;
				object->Postranslate = posCursorObject;
			}*/

			//--- Target
			/*if (object->Name == "Box2") {
				std::shared_ptr <ObjectData> objectObserver = Storage.GetObjectPrt("Mon");
				object->Postranslate = objectObserver->Target;
			}*/

			//object->TempVectors.push_back(nearestSphereIntersectionPoint);
			//object->TempVectors.push_back(nearestPolygonIntersectionPoint);
			//if (object->Name == "M_P_1") {
			//	std::shared_ptr <ObjectData> objectObserver = Storage.GetObjectPrt("Mon");
			//	if(objectObserver->TempVectors.size() > 1)
			//		object->Postranslate = objectObserver->TempVectors[0]; //nearestSphereIntersectionPoint - green
			//}
			if (object->Name == "M_V_1") {
				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				if (objectObserver->TempVectors.size() > 0)
					object->Postranslate = objectObserver->TempVectors[0];
			}
			if (object->Name == "M_V_2") {
				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				if (objectObserver->TempVectors.size() > 1)
					object->Postranslate = objectObserver->TempVectors[1];
			}
			if (object->Name == "M_V_3") {
				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				if (objectObserver->TempVectors.size() > 2)
					object->Postranslate = objectObserver->TempVectors[2];
			}
			if (object->Name == "M_C_2") {
				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				object->Postranslate = objectObserver->PlaneDownPosition; 
			}

			
			if (object->Name == "M_C_1") {
				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				object->Postranslate = objectObserver->Postranslate; //nearestPolygonIntersectionPoint - blue
			}

			//----- Light position
			std::shared_ptr <ObjectData> objectLight = Storage->GetObjectPrt("Mon"); //Box2
			m_Lighting.positionLight = vec3(objectLight->Postranslate.x, objectLight->Postranslate.y + 15.f, objectLight->Postranslate.z);
			model->ConfUniform.SetPositionLight(m_Lighting.positionLight);
			
			//------ Set Mouse position
			model->ConfUniform.SetPositionMouse(m_OperatorG.PositionCursorModel);

			DrawGraph(shaderProgram, model);

			isUpdate = false;

			
		}

		Storage->Inputs->MBT = -1;
		Storage->Inputs->Key = -1;

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
