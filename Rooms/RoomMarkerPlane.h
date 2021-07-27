#pragma once

#include "SceneRoom.h"

#include <fstream>

using std::string;

class SceneConstructor;

class RoomMarkerPlane: public SceneRoom
{
public:
	RoomMarkerPlane(string Name, SceneConstructor* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();
};
