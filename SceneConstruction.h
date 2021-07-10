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
class AspectDispatcherCommands;
class BaseShell;
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
	GLuint last_VAO;

	bool test_isFerst = true;
	vec3 SavePos;
	int countObjects = 0;

	bool isTextureRepeat = false;
	bool m_isUpdateShaderProgramm = false;
	bool m_isUpdateTexture = false;
	bool m_isUpdateUV = false;
	bool m_isUpdateMesh = false;
	//bool m_isUpdate = false;
	bool m_isEnableGUI = false;

	bool m_isLastTransformMesh = false;
	string m_currCashShader = "-1";
	string m_lastCashShader = "-2";

	GLint indicesSize;

protected:
	//bool isUpdate = false;
	
public:
	shared_ptr<ObjectData> ObjectCurrent;
	shared_ptr<ModelData> ModelCurrent;
	shared_ptr<BaseShell> ShellCurrent;

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
	bool IsHaveShell = false;

	string DebugMessage = "";
	string DebugMessageAppend = "";
	string SymbolInput = "";
	//--------------

	GLFWwindow* Window;

	GLuint ShaderProgram;

	Lighting* Light;

	CreatorModelData* Storage;

	TransformModel* TransModel;

	Controllers* Contrl;

	vector<shared_ptr<SceneRoom>> Rooms;

	AspectFactoryObjects* factoryObjects;

	AspectDispatcherCommands* dispatcherCommands;

	//CommandPack* CurrentSceneCommand;
	CommandPack CurrentSceneCommand;

	SceneConstruction();

	SceneConstruction(GLFWwindow* Window);

	void Init();

	void LoadDataModel();

	void FillAlphabet();

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

	void SetInputTextEvents();

	void SetMouseButtonEvents();

	void GenMVP();

	void ClearScene();

	void DrawGraph();

	void Debug(string msg);

	void FactoryObjectsWork();

	float GetParamCase();

	bool ReadCommand(TypeCommand commandType);

	bool IsCurrentObjectBackgroundFrameGUI();

	void AddCommand(CommandPack command);

	void AddCommand(TypeCommand commandType, int sourceIndex = -1, int targetIndex = -1, string keyOptions = std::string(), int valueOptions = -1);

	void AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex, int valueI, float valueF = -1.0, vec4 valueV4 = vec4(), string valueS = std::string());

};

