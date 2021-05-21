#pragma once

//#include "SceneRoom.h"
//#include "CreatorModelData.h"
//#include "TransformModel.h"
//#include "Controllers.h"
#include "CoreSettings.h"

//---------
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
//-----------

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

class SceneRoom;
class ObjectData;
class ModelData;
class CreatorModelData;
class ControllerInput;
class TransformModel;
class Controllers;
//class GLFWwindow;

using std::vector;
using std::shared_ptr;
using std::string;
using glm::vec3;

class SceneConstruction
{
private:
	int prevIndexModel = -1;
	const char* prevModelTexture;
	const char* prevModelModel3D;
	GLuint last_VAO;
	
	bool test_isFerst = true;
	vec3 SavePos;

	int countObjects = 0;
	bool isTextureRepeat = false;

	const GLchar* pathShaderVertex = "basic.vert";
	const GLchar* pathShaderFrag = "basic.frag";
	const char* pathTextureTest = "./Textures/testTexture.bmp";
	const char* pathModel3D = "./Models3D/monkey.obj";
	GLint indicesSize;

protected:
	//bool isUpdate = false;

public:
	shared_ptr<ObjectData> ObjectCurrent;
	shared_ptr<ModelData> ModelCurrent;

	int m_widthWindow = 1024;
	int m_heightWindow = 768;

	float m_speed = 1.0f; //3.0f = 3  в секунду
	float m_deltaTime = 0.3f;//0.3f
	float m_lastFrame = 0.0f;
	//--------------

	GLFWwindow* Window;

	GLuint ShaderProgram;

	Camera* Cam;

	Operator* Oper;

	Lighting* Light;

	SceneParam* Scene;

	CreatorModelData* Storage;

	ControllerInput* Inputs;

	CoreMVP* ConfigMVP;

	TransformModel* TransModel;

	Controllers* Contrl;

	vector<shared_ptr<SceneRoom>> Rooms;

	SceneConstruction();

	SceneConstruction(GLFWwindow* Window);

	void Init();

	void LoadDataModel();

	void ConfigRoom();

	void AddRoom(SceneRoom* room);

	void WorkingRooms();

	bool SetObject(int i, bool& isUpdate);
	
	
	void SetDataToShaderAfter(bool isUpdate);

	void SetDataToShaderBefore(bool isUpdate);


	void Update();

	void ObjectUpdate(int i);

	//void Update_1();

	void SetMouseEvents();

	void GenMVP();

	void ClearScene();

	void DrawGraph();
};

