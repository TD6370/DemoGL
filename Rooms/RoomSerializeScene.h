#pragma once

#include "AspectBase.h"

#include <fstream>

//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

using std::string;
using std::shared_ptr;

class SceneConstructor;
class SceneSerialize;
struct ObjectFiledsSpecific;
class ObjectData;

class RoomSerializeScene : public AspectBase
{
private:

	SceneSerialize* _serializer;
	const int m_saveKey = GLFW_KEY_F2;
	const int m_loadKey = GLFW_KEY_F4;

public:
	RoomSerializeScene(string Name, SceneConstructor* sceneConstructor)
		: AspectBase(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();

	void Save();
	void Load();
	void LoadObjects();

	bool IsValidSave(shared_ptr<ObjectData> object);
};

