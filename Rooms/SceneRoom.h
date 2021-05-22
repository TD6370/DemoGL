#pragma once

//#include "ModelData.h"
//#include "Controllers.h"
//#include "SceneConstruction.h"

#include <fstream>

using std::string;

class SceneConstruction;

class SceneRoom
{

public:
	SceneConstruction* Scene;

	string Name;
	bool IsOnceComplete;
	
	SceneRoom(string Name, SceneConstruction* sceneConstructor);

	void virtual Init();
	void virtual Config();
	void virtual Work();
};

