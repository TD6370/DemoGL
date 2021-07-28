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

struct Material {
	//---- Material Data
	string PathShaderVertex = "basic.vert";
	string PathShaderFrag = "basic.frag";
	string PathTexture = "./Textures/testTexture.bmp";
	string PathTextureAtlas = "";	//@**
};

struct Mesh {
	//---- Mesh data
	const char* PathModel3D = "./Models3D/monkey.obj";
	GLint TrianglesCount = 0;
	GLint IndicesSize = 0;
	std::vector< glm::vec2 > UV;
	std::vector< glm::vec3 > Normals;
	std::vector< glm::vec3 > Vertices;
	std::vector<unsigned int> Indices;
	float RadiusCollider = 1;
	bool IsSquareModel = false;
};

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
	// ���� ������
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
	// ������� ������ : ��������� ������� (������ ��������� � ������ ���������)
	glm::mat4 Model;
	// �������� ������� ModelViewProjection, ������� �������� ����������� ������������ ����� ���� ������
	glm::mat4 MVP;

	glm::mat4 Projection;
};

enum ActionObject { Moving, Stay, Look, Search, Lock, Starting, Woking, Transforming, Clearing };

enum TypeObject { Polygon, Solid, Block, Tree, Terra, NPC, Bullet, Hero, BulletHero, CursorRay, GUI, TextBox, CursorGUI, Button, EditBox, ListBox
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

extern struct AttributeCommands
{
	string BaseListCommand = "BaseListCommand";
	string TypesObjectListCommand = "TypesObjectListCommand";
	string TypeObjectAttr = "TypeObject";
};



