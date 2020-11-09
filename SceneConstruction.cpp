#include "SceneConstruction.h"
//#include "SceneRoom.h"
//#include "CreatorModelData.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <iostream>
//#include <fstream>
//#include <string>
#include <vector>

using std::vector;
using std::string;
using glm::vec3;

SceneConstruction::SceneConstruction() {
}

SceneConstruction::SceneConstruction(GLFWwindow* window) {
	
	Window = window;
	//Init();
}

//void SceneConstruction::Init(Operator* p_oper, Lighting* p_light, SceneParam* p_scene, CreatorModelData* p_storage, ControllerInput* p_inputs) {
void SceneConstruction::Init() {

	Rooms = vector<shared_ptr<SceneRoom>>();
	Cam = new Camera();
	Oper = new Operator();

	Light = new Lighting();
	Light->positionLight = vec3(0, 0, 0);

	Scene = new SceneParam;
	Storage = new CreatorModelData;
	Inputs = new ControllerInput;
	ConfigMVP = new CoreMVP();

	TransModel = new TransformModel();
	ControlInput = new Controllers();

	countObjects = Storage->SceneObjectsLastIndex;

	LoadDataModel();

	ConfigRoom();
}

void SceneConstruction::LoadDataModel()
{
	//bool isGen = true;
	bool isGen = false;
	Storage = new CreatorModelData();
	Storage->SceneParam = Scene;
	if (isGen)
	{
		Storage->LoadModels();
		Storage->GenerateObjects();
	}
	else {
		Storage->Load();
	}
}

void SceneConstruction::ConfigRoom() {

	SceneRoom* room = new SceneRoom("Base", this);
	AddRoom(room);
}

void SceneConstruction::AddRoom(SceneRoom* room) {
	Rooms.push_back(std::make_unique<SceneRoom>(*room));
}

void SceneConstruction::WorkingRooms() {

	//MouseEvents(window, m_widthWindow, m_heightWindow, m_deltaTime, &m_OperatorG, &Inputs);

	for(auto room : Rooms){
		room->Work();
	}
}