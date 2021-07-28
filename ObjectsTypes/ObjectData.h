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
	//--- Command:
	CommandPack* SceneCommand;
	
	//============== -----  Material data	//TODO: ????
	Material MaterialData;

	//vec3 Color = vec3(0);
	
	//==============  ---- Mesh data	//TODO: ????
	Mesh MeshData;

	//vector <vec3> Vertices;
	//vector <vec3> Normals;
	//vector <vec2> TextureUV; //UV : Model
	//vector <vec3> Buffer;
	//GLint TrianglesCount = 0;

	//----- Shape -- Components proxy (Geometry)
	ShapeBase* Shape;

	// -----------  Options ---------------------
	bool IsSelected = false;

	bool IsGUI = false;
	bool IsNPC = false;
	bool IsHexagonModel = false;
	bool IsSquareModel = false;

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

	//============== TODO: In Render component
	GLuint VAO = EmptyID;
	GLuint VBO = EmptyID;
	GLuint BufferUV_ID = EmptyID;
	GLuint BufferNormal_ID = EmptyID;
	bool IsLoadedIntoMem_Vertex = false;
	bool IsLoadedIntoMem_Normals = false;
	bool IsLoadedIntoMem_UV = false;
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

	void virtual SetDataToShader();

	string virtual GetCashStateUpdateDataToShader();

	void UpdateDataBufferToShader();

	void UpdateNormalsToShader();

	void virtual UpdateState(); //after fill options

	void virtual Refresh(); //in actions

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

	std::shared_ptr<Plane> GetPlaneFromVertIndex(int indexVertPlane);

	std::shared_ptr<Plane> GetPlanePrt(int indexPlane);

	void virtual SelectedEvent();
	void virtual UnselectedEvent();

	std::vector< glm::vec3 > virtual GetVertices();
	std::vector< glm::vec2 > virtual GetUV();
	std::vector< glm::vec3 > virtual GetNormals();

	void virtual SetMesh();

	void UpdateTextureUV();
	
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
	//-------- SaveFieldSpecific

	vector<ObjectFiledsSpecific> virtual GetSpecificFiels();

	//--- TODO: remove pop item from filedsSpecific
	void virtual SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific);

	string GetSpecifValue(vector<ObjectFiledsSpecific> filedsSpecific, int index);

};

#endif




