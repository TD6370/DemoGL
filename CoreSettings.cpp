//#include "CoreSettings.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>


#include <string>
#include <vector>
#include <map>

#define M_PI 3.1415926535897932384626433832795

struct World {
	GLfloat Radius = 50; // 10;
	std::string PathObjects = "./Data/Objects/Objects.txt";
	std::string PathModels = "./Data/Models/Models.txt";
};

struct SceneParam {
	int IndexCursorRayObj;
	int IndexHeroObj;
	int IndexGUIObj;
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

	GLfloat Far = 1000.f;// 1000.f;;
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
	glm::vec3 PositionCursorModel;

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

enum ActionObject { Moving, Stay, Look, Search, Lock, Starting, Woking }; // , EventControl };

enum TypeObject { Polygon, Solid, Block, Tree, Terra, NPC, Bullet, Hero, BulletHero, CursorRay, GUI};

//enum TramsformType { RotateX, RotateY, RotateZ };

enum ColliseState  { COLLISE_UP, COLLISE_DOWN, COLLISE_NORMAL, UNKNOWN};

struct WorldSectors{
	std::string NameTerraPlane;
	std::map <std::string, std::vector<int>> SectorsPlane;
	std::map <std::string, std::vector<int>> SectorsObjects;
	std::map <std::string, std::vector<int>> SectorsBlocks;
};




