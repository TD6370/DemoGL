#pragma once

//#include "SceneRoom.h"
//#include "CreatorModelData.h"
//#include "TransformModel.h"
//#include "Controllers.h"
#include "CoreSettings.h"
//#include "ConfigUniformArg.h"

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
class AspectFactoryObjects;
//class GLFWwindow;

using std::vector;
using std::shared_ptr;
using std::string;
using glm::vec3;

//*** Data Pipeline Aspects (DPA)
class SceneConstruction
{
private:
	int prevIndexModel = -1;
	const char* prevModelTexture;
	const char* prevModel3D;
	string prevShaderVert;
	string prevShaderFrag;
	GLuint last_VAO;

	bool test_isFerst = true;
	vec3 SavePos;
	int countObjects = 0;

	bool isTextureRepeat = false;
	bool m_isUpdateShaderProgramm = false;
	bool m_isUpdate = false;
	bool m_isEnableGUI = false;

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
	bool IsDeltaUpdateLogic = false;
	float DeltaTime = 0;
	int VersionUpdate = 1;
	//versionUpdate = 1;

	bool IsFirstCurrentObject;
	bool IsLastCurrentObject;

	string DebugMessage = "";
	string DebugMessageAppend = "";
	//--------------

	GLFWwindow* Window;

	GLuint ShaderProgram;

	Lighting* Light;

	CreatorModelData* Storage;

	TransformModel* TransModel;

	Controllers* Contrl;

	vector<shared_ptr<SceneRoom>> Rooms;

	AspectFactoryObjects* factoryObjects;

	//CommandPack* CurrentSceneCommand;
	CommandPack CurrentSceneCommand;

	SceneConstruction();

	SceneConstruction(GLFWwindow* Window);

	void Init();

	void LoadDataModel();

	//*** introduction
	void ConfigRoom();

	//*** add Aspect
	void AddRoom(SceneRoom* room);

	bool IsBreakUpdate();

	void ResetRooms();

	//*** Pointcut
	void WorkingRooms();

	bool SetObject(int i);
	
	void SetDataToShader();

	void PreparationDataFromShader();

	void Update();

	void ObjectUpdate(int i);

	void SetMouseEvents();

	void SetMouseButtonEvents();

	void GenMVP();

	void ClearScene();

	void DrawGraph();

	void Debug(string msg);

	void FactoryObjectsWork();

	float GetParamCase();

	bool ReadCommand(TypeCommand commandType);

	bool IsCurrentObjectBackgroundFrameGUI();

	//----------- TODO:
	//CoreMVP* GetMVP();
	//ControllerInput* GetInputs();
	//Operator* GetOperator();
	//Camera* GetCamera();
	//SceneParam* GetSceneData();
};

