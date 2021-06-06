#pragma once

//#include "ModelData.h"
//#include "Controllers.h"
//#include "SceneConstruction.h"

#include <fstream>

using std::string;

class SceneConstruction;

//*** Acpect
class SceneRoom
{

public:
	SceneConstruction* Scene;

	string Name;
	//*** Advice option
	bool IsOnceComplete;
	
	SceneRoom(string Name, SceneConstruction* sceneConstructor);

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

