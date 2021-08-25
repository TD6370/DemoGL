#pragma once

#include "AspectBase.h"
#include "../GeometryLib.h"
#include "..\CoreSettings.h"

#include <sstream>

//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat4;
using glm::perspective;
using std::string;
using std::shared_ptr;

class ObjectData;
class ObjectGUI;
class SceneConstructor;
enum TypeCommand;
struct AnimationParamGUI;

class RoomUseInterface : public AspectBase
{
private:
	vec3 color_selected = vec3(0, 0, 0);
	PaletteColors m_palette = PaletteColors();

	vec2 m_tempMousePos = vec2(0);
	vec3 m_tempMousePosWorld = vec3(0);
	const int m_endEditKey = GLFW_KEY_ENTER;

	float m_sizeBorder = 0.2;
	string m_stringDebug = "";

	AnimationParamGUI* AnimationParams;

	float m_CurrentStartedEventID = -1;
		
	bool m_isDebug = false;
	mat4 m_projectionPerspective;
	int m_KeyPush = GLFW_MOUSE_BUTTON_1;

	bool IsFocused = false;
	bool IsCursorClickEvent = false;
	bool IsCheckBorder = false;
	bool IsEditControls = false;
	bool IsBackgroundFrame = false;
	bool IsUpdatingStateObjects = true;
	bool IsCreatingObject = false ;
	
	int IndexObjectFocused = -1;
	int IndexObjectSelected = -1;
	int IndexObjectCreating = -1;

	int IndexObjectSelectedEdit = -1;

	float FocusedOrder = -1;
	vec3 CursorMovePos = vec3(0);
	vec3 CursorRayPos = vec3(0);
	vec3 SelectObjectOffsetPos = vec3(0);
	vec2 m_startSizePanel = vec2(0);

	float StartTimer = 0;
	float EndTimer = 0;

	std::shared_ptr<ObjectData> ObjectSelectedEdit = nullptr;

public:
		
	RoomUseInterface(string Name, SceneConstructor* sceneConstructor)
		: AspectBase(Name, sceneConstructor) {};

	~RoomUseInterface();

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

	//-------- Focus control
	void CheckFocusBoxAndBorderControl();
	void EventFocusControl();

	//============= Event Create Controls ==============

	//------- CreateObject
	void EventSelectedInfoCreateObject(); // Event selected info create object
	void EventStartCreateObject();
	void EventEndCreateObject();

	//---------- Fill fields selected object
	void EventFillFieldsEdit();
	bool EventSaveFieldsEdit();
	void SaveEditObjectWoldInfo();

	//============== Event Use Controls ========================================

	//--- Use = Edit on/off
	void ModeEditControls();

	//--- Use = Click control
	void EventStartClickControl();

	//--- Edit = Name control
	void EventStartRenameObject();

	//--- Use = Edit box controls
	void EventEditTextControl();
		
	//--- Use = Message - EditBox & TextBox
	void EventReadKeyInput();

	//=================================================
	
	void CalculateMousePosWorld();

	//--- call refresh visible
	void CheckStateObjects();

	// -- save event status animation
	void SetCurrentEventParam(shared_ptr<ObjectData> obj, int value);
	void SetTimeAnimate(shared_ptr<ObjectData> obj, float time);
	
	
};

