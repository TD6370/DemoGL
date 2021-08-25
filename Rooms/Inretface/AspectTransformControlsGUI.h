
#pragma once

#include "../AspectBase.h"

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using glm::vec3;
using glm::vec2;

class SceneConstructor;
class RoomUseInterface;

class AspectTransformControlsGUI : public AspectBase
{
private:
	vec3 m_posCursor = vec3();
	vec2 m_sizeBackground = vec2(-1);
	RoomUseInterface& m_state;

public:
	AspectTransformControlsGUI(RoomUseInterface& baseGUI, string Name, SceneConstructor* sceneConstructor)
		: AspectBase(Name, sceneConstructor) , m_state(baseGUI) {};

	void Init();
	void Config();
	void Work();
};