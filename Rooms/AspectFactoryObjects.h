#pragma once

#include "SceneRoom.h"

#include <fstream>

using std::string;

class SceneConstruction;

class AspectFactoryObjects : public SceneRoom
{
public:
	AspectFactoryObjects(string Name, SceneConstruction* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();

	void CreateButton();

	void CreateEditBox();
};




