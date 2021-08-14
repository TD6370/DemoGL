//#include "CoreSettings.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <map>

using std::vector;
using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::string;
using std::map;

#define M_PI 3.1415926535897932384626433832795
#define EmptyID 66666
#define STR_EMPTY "@?"
#define BACKGROUND_GUI_WIDTH_F 2.35

struct Material {
	string PathShaderVertex = "";
	string PathShaderFrag = "";
	string PathTexture = "";
	string PathTextureAtlas = "";
	vec3 Color = vec3(0);
};

struct Mesh {
	const char* PathModel3D = "";
	GLint TrianglesCount = 0;
	GLint IndicesSize = 0;
	vector< vec2 > UV;
	vector< vec3 > Normals;
	vector< vec3 > Vertices;
	vector <vec3> Buffer;
	vec3 Color = vec3(0);
	vector<unsigned int> Indices;
	float RadiusCollider = 1;
	bool IsSquareModel = false;
};

struct World {
	GLfloat Radius = 50; // 10;
	string PathObjects = "./Data/Objects.txt";
	string PathModels = "./Data/Models.txt";
	string PathShells = "./Data/Shells.txt";

};

struct SceneParam {
	int IndexCursorRayObj = -1;
	int IndexHeroObj = -1;
	int IndexBackgroundGUIObj = -1;
	int IndexCursorGUI = -1;
	int IndexBaseEditBox = -1;
	int IndexObjectFieldsEdit = -1;
	string NameSystemEditBox = "SystemRootEditBox";
	string NameSystemObjectFieldsEdit = "SystemObjectFieldsEdit";
	bool IsGUI = false;
	vec2 SizeBackgroungGUI = vec2(-1);
};

struct Camera {
	GLfloat camX = 4.0f;
	GLfloat camY = 3.0f;
	GLfloat camZ = 3.0f;
	GLfloat lookCamX = 0.0f;
	GLfloat lookCamY = 0.0f;
	GLboolean isPerspective = true;
	
};

struct Operator {

	GLfloat Far = 300.f;// 1000.f;;
	glm::vec3 m_direction;
	float VerticalAngle = 0;
	float HorizontalAngle = 0;
	//glm::vec3 m_directionCursor;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_position = glm::vec3(0, 0, 5);
	// Угол обзора
	float m_initialFoV = 45.0f;
	float m_FoV = glm::radians(45.0f);
	bool m_start = false;

	GLdouble MouseOffset_x, MouseOffset_y;
	GLdouble m_MouseX = 0, m_MouseY = 0;
	glm::vec3 PositionCursorWorld;
	

	bool IsFullscreen = false;
};

struct ControllerInput {

	int Key = -1;
	int Action = -1;
	int MBT = -1;
	int ActionMouse = -1;
	int Mode = -1;
	GLfloat ParamCase = 0;
};

struct Lighting {
	glm::vec3 positionLight;
};

struct CoreMVP {
	glm::mat4 View;
	// Матрица модели : единичная матрица (Модель находится в начале координат)
	glm::mat4 Model;
	// Итоговая матрица ModelViewProjection, которая является результатом перемножения наших трех матриц
	glm::mat4 MVP;

	glm::mat4 Projection;
};

enum ActionObject { Moving, Stay, Look, Search, Lock, Starting, Woking, Transforming, Clearing };

enum TypeObject { Polygon, Solid, Block, Tree, Terra, NPC, Bullet, Hero, BulletHero, CursorRay, GUI, TextBox, CursorGUI, Button, EditBox, ListBox, ListEditBox, ListTextBox, ObjectFieldsEdit
};

enum ColliseState  { COLLISE_UP, COLLISE_DOWN, COLLISE_NORMAL, UNKNOWN};

enum TypeCommand { None, CreateObject, ObjectCreated, DeleteObject, SelectPosForObject, SelectedPosForObject, EditGUI_OnOff, EditObjectCommand,
	CheckStateObjectCommand, StopWorking, KeyInputCommand, RenameObject, ObjectReading, SelectItemValue
};

enum TypeLayer {
	LayerBackground,
	LayerBack, 
	LayerNone, 
	LayerBase, 
	LayerFront, 
	LayerSystem
};

struct CommandPack {
	bool Enable;
	TypeCommand CommandType;
	int SourceIndex = -1;
	int TargetIndex = -1;
	string Description;
	string ValueS;
	int ValueI = -1;
	float ValueF = -1;
	vec4 ValueV4 = vec4(-1);
	map <string, int> Options = map <string, int>{};
	bool IsLongCommand = false;
};

struct WorldSectors{
	string NameTerraPlane;
	map <string, vector<int>> SectorsPlane;
	map <string, vector<int>> SectorsObjects;
	map <string, vector<int>> SectorsBlocks;
};

struct AnimationParamGUI
{
	int StartDefaultParamShaderID = 0;
	int StartFocusParamShaderID = 1;
	int StartMoveParamShaderID = 2;
	int StartResizeParamShaderID = 3;
	int StartCheckBorderParamShaderID = 4;
	int StartClickParamShaderID = 5;
	int StartPressedParamShaderID = 6;
}; // AnimationGUI;

extern struct AttributeCommands
{
	string BaseListCommand = "BaseListCommand";
	string TypesObjectListCommand = "TypesObjectListCommand";
	string ObjectFieldsListCommand = "ObjectFieldsListCommand";
	string ObjectFieldValuesListCommand = "ObjectFieldValuesListCommand";
	string TypeObjectAttr = "TypeObject";
	string SourceCommandObjIndexAttr = "SourceCommandObjIndex";
};

extern struct ModelNames
{
	string Hero = "homo";
	string NPC = "mon";
	string Bullet = "marker_Point";
	string Block = "box";
	string Terra = "box";
	string MarkerPoint = "marker_Point";
	string MarkerCross = "marker_Cross";
	string MarkerVector = "marker_Vector";
};



