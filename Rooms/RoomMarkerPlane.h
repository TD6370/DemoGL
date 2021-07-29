#pragma once

#include "AspectBase.h"

#include <fstream>

using std::string;

class SceneConstructor;

class RoomMarkerPlane: public AspectBase
{
public:
	RoomMarkerPlane(string Name, SceneConstructor* sceneConstructor)
		: AspectBase(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();
};
