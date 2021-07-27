#pragma once

#include "SceneRoom.h"

#include <fstream>

using std::string;

class SceneConstructor;

class AspectFactoryObjects : public SceneRoom
{
public:
	AspectFactoryObjects(string Name, SceneConstructor* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();

	void CreateButton();

	void CreateEditBox();

	void CreateListBox(string nameListCommand);
};




