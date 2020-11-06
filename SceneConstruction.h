#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

class SceneRoom;

using std::vector;
using std::shared_ptr;
using glm::vec3;


class SceneConstruction
{
public:
	vector<shared_ptr<SceneRoom>> Rooms;

	SceneConstruction();

	void AddRoom(SceneRoom* room);

	void WorkingRooms();
};

