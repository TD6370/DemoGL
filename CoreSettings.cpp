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

struct World {
	GLfloat Radius = 50; // 10;
	std::string PathObjects = "./Data/Objects.txt";
	std::string PathModels = "./Data/Models.txt";
	std::string PathShells = "./Data/Shells.txt";
};

struct SceneParam {
	int IndexCursorRayObj = -1;
	int IndexHeroObj = -1;
	int IndexBackgroundGUIObj = -1;
	int IndexCursorGUI = -1;
	int IndexBaseEditBox = -1;
	string NameSystemEditBox = "SystemRootEditBox";
	bool IsGUI = false;
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

enum ActionObject { Moving, Stay, Look, Search, Lock, Starting, Woking, Transforming };// , ReadingEvents }; // , EventControl };

enum TypeObject { Polygon, Solid, Block, Tree, Terra, NPC, Bullet, Hero, BulletHero, CursorRay, GUI, TextBox, CursorGUI, Button, EditBox
};

enum ColliseState  { COLLISE_UP, COLLISE_DOWN, COLLISE_NORMAL, UNKNOWN};

enum TypeCommand { None, CreateObject, ObjectCreated, DeleteObject, SelectPosForObject, SelectedPosForObject, EditGUI_OnOff, EditObjectCommand,
	CheckStateObjectCommand, StopWorking, KeyInputCommand, RenameObject, ObjectReading
};

enum TypeLayer {
	LayerBackground,
	LayerBack, 
	LayerNone, 
	LayerGUI, 
	LayerFront, 
	LayerBase, 
	LayerSystem
};

struct CommandPack {
	bool Enable;
	TypeCommand CommandType;
	int SourceIndex;
	int TargetIndex;
	string Description;
	string ValueS;
	int ValueI;
	float ValueF;
	vec4 ValueV4;
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

//map<string, int> MapAlphabet;

//map<string, int> MapAlphabet = map<string, int>{
//		{"а",0},
//		{"б",1},
//		{"в",2},
//		{"г",3},
//		{"д",4},
//		{"е",5},
//		{"ж",6},
//		{"з",7},
//		{"и",8},
//		{"к",9},
//		{"л",10},
//		{"м",11},
//		{"н",12},
//		{"о",13},
//		{"п",14},
//		{"р",15},
//		{"с",16},
//		{"т",17},
//		{"у",18},
//		{"ф",19},
//		{"х",20},
//		{"ц",21},
//		{"ч",22},
//		{"ш",23},
//		{"ш",24},
//		{"ь",25},
//		{"ы",26},
//		{"э",27},
//		{"ю",28},
//		{"я",29},
//		{" ",30},
//
//		{"1",31},
//		{"2",32},
//		{"3",33},
//		{"4",34},
//		{"5",35},
//		{"6",36},
//		{"7",37},
//		{"8",38},
//		{"9",39},
//		{"0",40},
//		{".",41},
//		{",",42},
//		{"!",43},
//		{"?",44},
//		{":",45},
//		{"-",46},
//		{"=",47},
//		{"<",48},
//		{">",49},
//};

