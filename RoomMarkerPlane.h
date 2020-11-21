//#pragma once
//class RoomMarkerPlane
//{
//};

#pragma once

#include "SceneRoom.h"
//#include "Controllers.h"
//#include "SceneConstruction.h"

#include <fstream>

using std::string;

class SceneConstruction;

class RoomMarkerPlane: SceneRoom
{
	void Init();
	void Config();
	void Work();
};
