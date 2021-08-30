
#pragma once

#include "../AspectBase.h"
#include "../../CoreSettings.h"

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using glm::vec3;
using glm::vec2;
using std::shared_ptr;

class SceneConstructor;
class RoomUseInterface;

class AspectTransformControlsGUI : public AspectBase
{
private:
	PaletteColors m_palette = PaletteColors();

	float m_sizeBorder = 0.2;
	vec2 m_startSizePanel = vec2(0);
	vec3 m_posCursor = vec3();
	vec2 m_sizeBackground = vec2(-1);
	RoomUseInterface* m_base;
	//shared_ptr<RoomUseInterface> m_base;

	vec3 SelectObjectOffsetPos = vec3(0);
	vec3 CursorMovePos = vec3(0);
	
public:

	AspectTransformControlsGUI(string Name, SceneConstructor* sceneConstructor)
		: AspectBase(Name, sceneConstructor) {};

	void Init(RoomUseInterface* base);
	void Init();

	void Config();
	void Work();

	//============= Event Transform Controls ==============
	//-------- Move control
	void EventStartMovingControl();
	void EventMovingControl();
	bool EventEndMovingControl();

	//-------- Resize control
	void EventStartResizeControl();
	void EventResizeControl();
	bool EventEndResizeControl();
};