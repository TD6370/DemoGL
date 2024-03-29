#pragma once

#include <fstream>

using std::string;

class SceneConstructor;

//*** Acpect
class AspectBase
{

public:
	SceneConstructor* Scene;

	string Name;
	//*** Advice option
	bool IsOnceComplete;
	
	AspectBase(string Name, SceneConstructor* sceneConstructor);

	void virtual Init();
	void virtual Config();

	//*** Advice (after default)
	void virtual Work();

	//*** Advice before
	void virtual BeforeWork();

	//*** Advice after
	void virtual AfterWork();

	//*** Advice first
	void virtual FirstWork();

	//*** Advice last
	void virtual LastWork();
};

