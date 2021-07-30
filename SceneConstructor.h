#pragma once

//#include "AspectBase.h"
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

class AspectBase;
class ObjectData;
class ModelData;
class CreatorModelData;
class ControllerInput;
class TransformModel;
class Controllers;
class AspectFactoryObjects;
class AspectDispatcherCommands;
class BaseShell;

using std::vector;
using std::shared_ptr;
using std::string;
using glm::vec3;

//*** Data Pipeline Aspects (DPA)
class SceneConstructor
{
private:
	int prevIndexModel = -1;
	string prevModelTexture;
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

public:
	shared_ptr<ObjectData> ObjectCurrent;
	shared_ptr<ModelData> ModelCurrent;
	shared_ptr<BaseShell> ShellCurrent;

	int m_widthWindow = 1024;
	int m_heightWindow = 768;

	float m_speed = 3.0f; //3.0f = 3  в секунду
	float m_deltaTime = 0.3f;//0.3f
	float m_lastFrame = 0.0f;
	bool IsDeltaUpdateLogic = false;
	float DeltaTime = 0;
	bool IsSpeedDeltaTime = false;
	int VersionUpdate = 2;
	//versionUpdate = 1;
	int CurrentIndexOrder = -1;
	
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

	//--- TODO:	--- vector<AspectBase> Rooms; 
	//vector<AspectBase> Aspects;
	vector<shared_ptr<AspectBase>> Aspects;

	AspectFactoryObjects* factoryObjects;

	AspectDispatcherCommands* dispatcherCommands;

	CommandPack CurrentSceneCommand;

	AttributeCommands CommandsAttribute;

	SceneConstructor();

	SceneConstructor(GLFWwindow* Window);

	void Init();

	void LoadDataModel();

	void FillAlphabet();

	//*** introduction
	void ConfigRoom();

	//*** add Aspect
	void AddAspect(AspectBase* aspect);

	bool IsBreakUpdate();

	void ResetAspects();

	//*** Pointcut
	void WorkingAspects();

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

	void AddCommand(TypeCommand commandType, bool isLong = false);

	void AddCommand(TypeCommand commandType, int sourceIndex = -1, int targetIndex = -1, vector<string> keyOptions = vector<string>(), vector<int> valueOptions = vector<int>(),
		int valueI = -1, float valueF = -1.0, vec4 valueV4 = vec4(), string valueS = std::string(), string description = std::string(), bool isLong = false);

	void AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex, 
		int valueI, float valueF = -1.0, vec4 valueV4 = vec4(), string valueS = std::string(), string description = std::string(), bool isLong = false);

	void RefreshGUI();
	
	vector<CommandPack> GetListCommand(string nameList);
};

