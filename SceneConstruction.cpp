#include "SceneConstruction.h"
#include "SceneRoom.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <iostream>
//#include <fstream>
//#include <string>
#include <vector>

using std::vector;
using glm::vec3;

SceneConstruction::SceneConstruction() {
	Rooms = vector<shared_ptr<SceneRoom>>();
}

void SceneConstruction::AddRoom(SceneRoom* room) {
	Rooms.push_back(std::make_unique<SceneRoom>(*room));
}

void SceneConstruction::WorkingRooms() {
	for(auto room : Rooms){
		room->Work();
	}
}