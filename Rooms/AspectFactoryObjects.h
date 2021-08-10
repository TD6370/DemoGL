#pragma once

#include "../CoreSettings.h"
#include "AspectBase.h"

#include <fstream>

using std::string;
using std::shared_ptr;

class SceneConstructor;
class BaseShell;
class ObjectData;

struct CreateInfo {
	bool Init = false;
	string Message;
	vec3 Pos = vec3();
	vec2 Size = vec2();
};

class AspectFactoryObjects : public AspectBase
{
private:
	shared_ptr<BaseShell> m_lastShellCreated;
	shared_ptr<ObjectData> m_lastObjectCreated;
	bool m_startContructing = false;

public:
	AspectFactoryObjects(string Name, SceneConstructor* sceneConstructor)
		: AspectBase(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();

	void CreateTextBox(CreateInfo info);

	void CreateButton(CreateInfo info);

	void CreateEditBox(CreateInfo info);

	void CreateListBox(string nameListCommand, TypeObject typeListBox = ListBox, CreateInfo info = CreateInfo());
	
	void CreateObjectFieldsEdit(CreateInfo info = CreateInfo());
};




