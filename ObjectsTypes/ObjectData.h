#pragma once

#ifndef ObjectData_H
#define ObjectData_H

#include "..\CoreSettings.h"
#include "..\ConfigUniformArg.h"
#include "..\TransformModel.h"
//#include "../GeomertyShapes/ShapeBase.h"

//#include "ObjectPhysic.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>
#include <map>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::linearRand;
using glm::mat4;
using std::string;
using std::shared_ptr;
using std::map;

class Plane;
class ModelData;
class CreatorModelData;
class WorldCluster;
struct ObjectFiledsSpecific;
class BaseShell;
class RenderComponent;
class TextBoxComponent;
class ButtonComponent;
class GUIComponent;

class ShapeBase;
class ShapeHexagon;
class ShapeSquare;

//TODO: EngineData
struct DataEngine {
	CoreMVP* ConfigMVP;
	ControllerInput* Inputs;
	Operator* Oper;
	Camera* Cam;
	SceneParam* SceneData;
	WorldCluster* Clusters;
};

class ObjectData
{
private:
	GLfloat m_nextWayTime = 0;
	GLfloat m_stepMove = 0;
	std::string m_keyPosSectorStr;

protected:
	GLfloat m_speedRotate = 0.1f;
	GLfloat m_angleModel = 0.0;
	vec3 StartColor = vec3(-1);
public:
	int Index = 0;
	//-------- Info
	string Name;
	TypeObject TypeObj;
	ActionObject ActionObjectCurrent;
	TypeLayer Layer;
	ColliseState CollisionPolygonState; //????
	GLfloat Speed = 0.5f;
	GLfloat ParamCase = -1; //???

	AnimationParamGUI* AnimationParam; //TODO: in static param

	//--- Command:
	CommandPack* SceneCommand;
	
	//-----  Material data
	Material MaterialData;
	
	//---- Mesh data
	Mesh MeshData;

	//----- Shape -- Components proxy (Geometry)
	ShapeBase* Shape;

	// -----------  Options ---------------------
	bool IsSelected = false;

	bool IsGUI = false;
	bool IsNPC = false;
	bool IsHexagonModel = false;
	bool IsSquareModel = false;
	bool IsTextBoxComponent = false;
	//B#
	bool IsButtonComponent = false;
	bool IsGUIComponent = false;
	bool IsPhisicComponent = false;

	bool IsVisible = true;
	bool IsFocusable = false;
	bool IsTransformable = false;
	bool IsUsable = false;
	bool IsChecked = false;
	bool IsAbsolutePosition = true;
	bool IsTextureRepeat = false;
	bool IsGravity = false;
	
	
	//-------- Links
	shared_ptr<ModelData> ModelPtr; 	//TODO: ????
	shared_ptr<ObjectData> OwnerObj;
	shared_ptr<ObjectData> NextItemShellObj;
	shared_ptr<BaseShell> Shell;
	int IndexObjectOwner = -1;
	int NextItemShellIndex = -1;
	int ShellIndex = -1;

	//-------- ENGINE:
	DataEngine* EngineData;	//TODO: private static 
	
	//============== proxy manager --- TODO: Aspect
	WorldCluster* Clusters; //TODO: private static 
	
	//-- animation component ???
	GLfloat StartTimer = -1;
					
	//---- Render component
	RenderComponent* Render;

	//---- Text box component

	TextBoxComponent* TextBox;

	ButtonComponent* ComponentButton;

	GUIComponent* ComponentGUI;

	//-- Render ??
	float TextureRepeat = 1;
	GLint ParamValue = -1;
		
	//==============  Phisic component (Rigidbody / Solid)
	vec3 TranslateAngle = vec3(0);
	vec3 TargetAngle = vec3(0);
	vec3 Size = vec3(0);
	vec3 Postranslate = vec3(0);
	vec3 NewPostranslate = vec3(0);
	vec3 Move = vec3(0);
	vec3 Target = vec3(0);
	vector<int> tramsformTypes;
	mat4 TransformResult;
	int minDist = 5;
	int DistanceTarget = 20;
	//GUI
	vec3 StartPos = vec3(0);
	vec2 SizePanel = vec2(0);
	// Phisic collision
	string ZonaPolygon;
	GLfloat PolygonPointY = 0;
	int PlaneDownIndex = -1;
	vec3 PlaneDownPosition = vec3(0);
	vector<vec3> TempVectors;
	
	ObjectData();

	ObjectData(int p_index,
		shared_ptr<ModelData> p_model,
		TypeObject p_typeObj,
		vec3 p_pos = vec3(0));

	~ObjectData();

	void virtual InitData();

	void InitRender();

	//--------------------------- Update data to Shader from Render
	string virtual GetCashStateUpdateDataToShader();

	void virtual ToShader_OtherData();

	void ToShader_CustomBuffer();

	void ToShader_Normals();

	void ToShader_Mesh();

	void ToShader_UV();

	void ToShader_Texture();

	//-----------------------------------------------

	void virtual UpdateState(); //after fill options

	void virtual Refresh(); //in actions

	void Reset(); //in actions

	void CheckedRefresh();

	void GenStartPosition();

	void virtual CheckStartPosition();

	void virtual RunAction();

	void ActionBase();
	
	void RunTransform();

	void Action();

	//---------------
	void virtual SetOwnerObject(shared_ptr<ObjectData> p_ownerObj);
	//void SetOwnerObject(ObjectData* p_ownerObj);
	void SetNextItemShellObject(shared_ptr<ObjectData> p_nextItemShellObj);
	void SetShell(shared_ptr<BaseShell> p_shell);

	Plane& GetPlaneFromVertIndex(int indexVertPlane);

	//---- FIX.Plane
	//std::shared_ptr<Plane> GetPlanePrt(int indexPlane);
	Plane& GetPlanePrt(int indexPlane);

	//---- FIX.Plane
	std::tuple<vec3, vec3, vec3> GetCase_V_W(int indexPlane);

	void virtual SelectedEvent();
	void virtual UnselectedEvent();

	std::vector< glm::vec3 > virtual GetVertices();
	std::vector< glm::vec2 > virtual GetUV();
	std::vector< glm::vec3 > virtual GetNormals();
	
	void virtual ControlsEvents();

	bool virtual GetVisible();

	string virtual GetInfo();

	void virtual DefaultView();

	void virtual Click();
	
	//void virtual Work();
	void virtual ActionWork();
	
	void SetStartTimer();
	
	//-----------------------------

	ShapeHexagon* GetShapeHexagon();

	ShapeSquare* GetShapeSquare();

	//-----------------------------
	
	void virtual MeshTransform(); //------- geometric ???

	float virtual  GetTopLayer();

	float virtual  GetZ();

	void virtual SetZ(float z);

	int virtual GetRightBorderVertexIndex();

	GLuint GetVAO();

	void SetParamCase(float param);

	void SetVectorsParams(vec3 vectorsParams[10]);

	void SetLight(vec3 posLight);
	
	//-------- SaveFieldSpecific

	vector<ObjectFiledsSpecific> virtual GetSpecificFiels();

	//--- TODO: remove pop item from filedsSpecific
	void virtual SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);

	string GetSpecifValue(vector<ObjectFiledsSpecific> filedsSpecific, int index);

	void SizeControlUpdate();

	bool IsNeedUodateMeshToShader();

};

#endif




