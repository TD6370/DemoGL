//#pragma once
//class AspectFamilyBonds
//{
//};

#pragma once

#include "SceneRoom.h"

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using glm::vec3;

class SceneConstruction;

class AspectFamilyBonds : public SceneRoom
{
private:
	vec3 m_posCursor = vec3();
	float m_lenghtLineBackground = -1;

public:
	AspectFamilyBonds(string Name, SceneConstruction* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();
};
