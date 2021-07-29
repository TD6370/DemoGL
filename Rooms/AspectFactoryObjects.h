#pragma once

#include "../CoreSettings.h"
#include "AspectBase.h"

#include <fstream>

using std::string;
using std::shared_ptr;

class SceneConstructor;
class BaseShell;

class AspectFactoryObjects : public AspectBase
{
private:
	shared_ptr<BaseShell> m_lastShellCreated;

public:
	AspectFactoryObjects(string Name, SceneConstructor* sceneConstructor)
		: AspectBase(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();

	void CreateButton();

	void CreateEditBox();

	void CreateListBox(string nameListCommand, TypeObject typeListBox = ListBox);

	void CreateListEditBox(string nameListCommand);
	
	void CreateObjectFieldsEdit();
};




