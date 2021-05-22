#pragma once

#include "SceneRoom.h"

#include <fstream>

//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

using std::string;

class SceneConstruction;
class SceneSerialize;

class RoomSerializeScene : public SceneRoom
{
private:

	SceneSerialize* _serializer;
	const int _saveKey = GLFW_KEY_F2;

public:
	RoomSerializeScene(string Name, SceneConstruction* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();
};
