#pragma once

#include "../CoreSettings.h"
#include "SceneRoom.h"

#include <fstream>

using std::string;
using std::shared_ptr;

class SceneConstructor;
class BaseShell;

class AspectFactoryObjects : public SceneRoom
{
private:
	shared_ptr<BaseShell> m_lastShellCreated;

public:
	AspectFactoryObjects(string Name, SceneConstructor* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();

	void CreateButton();

	void CreateEditBox();

	void CreateListBox(string nameListCommand, TypeObject typeListBox = ListBox);

	void CreateListEditBox(string nameListCommand);
	
	void CreateObjectFieldsEdit();
};




