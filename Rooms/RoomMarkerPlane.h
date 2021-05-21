#pragma once

#include "SceneRoom.h"

#include <fstream>

using std::string;

class SceneConstruction;

class RoomMarkerPlane: public SceneRoom
{
public:
	RoomMarkerPlane(string Name, SceneConstruction* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();
};
