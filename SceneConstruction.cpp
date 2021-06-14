#include "SceneConstruction.h"
//#include "SceneRoom.h"
//#include "CreatorModelData.h"
#include "Rooms/SceneRoom.h"
#include "Rooms/RoomMarkerPlane.h"
#include "Rooms/RoomSerializeScene.h"
#include "Rooms/RoomUseInterface.h"

#include "CreatorModelData.h"
#include "TransformModel.h"
#include "Controllers.h"
#include "ModelData.h"
#include "ObjectsTypes/ObjectData.h"
#include "ObjectsTypes/ObjectDynamic.h"

#include "CoreSettings.h"
#include "GeometryLib.h"
#include "Serialize/SceneSerialize.h"
#include "ConfigUniformArg.h"

#include "./ObjectsTypes/ObjectGUI.h"


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "..\SceneConstruction.h"
//#include "..\ObjectsTypes\ObjectData.h"

//#include "..\CreatorModelData.h"
//#include "..\ModelData.h"

//#include <iostream>
//#include <fstream>
//#include <string>
#include <vector>

using std::vector;
using std::string;

using glm::vec3;
using glm::vec4;
using glm::mat4;
using std::make_unique;

SceneConstruction::SceneConstruction() {
}

SceneConstruction::SceneConstruction(GLFWwindow* window) {
	
	Window = window;
	Init();
}

void SceneConstruction::Init() {

	Rooms = vector<shared_ptr<SceneRoom>>();
	

	Light = new Lighting();
	Light->positionLight = vec3(0, 0, 0);

	Storage = new CreatorModelData();

	TransModel = new TransformModel();
	Contrl = new Controllers();

	LoadDataModel();
	
	ConfigRoom();
}

void SceneConstruction::LoadDataModel()
{
	Storage = new CreatorModelData();
	Storage->Load();

	//bool isGen = true;
	/*if (isGen)
	{
		Storage->LoadModels();
		Storage->GenerateObjects();
	}
	else {
		Storage->Load();
	}*/
}

//*** introduction
void SceneConstruction::ConfigRoom() {

	//*** create Aspect
	SceneRoom* room = new SceneRoom("Base", this);
	room->Init();
	//*** join point
	AddRoom(room);

	RoomMarkerPlane* roomMarkers = new RoomMarkerPlane("Markers", this);
	roomMarkers->Init();
	//*** join point
	Rooms.push_back(make_unique<RoomMarkerPlane>(*roomMarkers));

	RoomSerializeScene* roomSerializator = new RoomSerializeScene("Serialize", this);
	roomSerializator->Init();
	Rooms.push_back(make_unique<RoomSerializeScene>(*roomSerializator));

	RoomUseInterface* roomInterface = new RoomUseInterface("Interface", this);
	roomInterface->Init();
	Rooms.push_back(make_unique<RoomUseInterface>(*roomInterface));
}

void SceneConstruction::AddRoom(SceneRoom* room) {
	Rooms.push_back(make_unique<SceneRoom>(*room));
}

void SceneConstruction::ResetRooms() {

	for (auto room : Rooms) {
		room->IsOnceComplete = false;
	}
}

//*** Pointcut
void SceneConstruction::WorkingRooms() {

	for(auto room : Rooms){
		//*** Advice
		room->Work();
	}
}

void SceneConstruction::PreparationDataFromShader() {
	
	//TEST
	auto bname = ObjectCurrent->Name;

	if (!m_isUpdate)
	{
		bool isTextRepeat = ObjectCurrent->IsTextureRepeat;
		if (isTextureRepeat && !isTextRepeat)
		{
			ObjectCurrent->ModelPtr->UpdateBufferUV();
			isTextureRepeat = false;
		}
		if (isTextRepeat) {
			isTextureRepeat = isTextRepeat;
			ObjectCurrent->UpdateTextureUV();
		}
	}

	if (m_isUpdate) {
		ShaderProgram = ModelCurrent->ShaderProgram;
		glUseProgram(ShaderProgram);
		last_VAO = ModelCurrent->VAO;
	}
}


void SceneConstruction::SetDataToShader() {
	
	//TEST
	//std::cout << ObjectCurrent->Name << "\n";
	auto nameTest =  ObjectCurrent->Name;

	bool isCubeModel = ObjectCurrent->ModelPtr->IsCubeModel;
	bool isDisabledGUI = Storage->SceneData->IsGUI != true && ObjectCurrent->IsGUI;
	bool isEnableGUI = Storage->SceneData->IsGUI == true && ObjectCurrent->IsGUI;
	
	if (isDisabledGUI)
		return;

	if (m_isEnableGUI != ObjectCurrent->IsGUI) {
		m_isEnableGUI = ObjectCurrent->IsGUI;
		m_isUpdate = true;
	}
	
	if (isCubeModel || m_isUpdate)
		ObjectCurrent->SetMesh();

	glBindVertexArray(ModelCurrent->VAO);

	if (m_isUpdate) {
		auto normals = ObjectCurrent->GetNormals();
		ModelCurrent->SetModelInBuffer(m_isUpdate, ObjectCurrent->Buffer, ObjectCurrent->TextureUV, normals);
	}

	//-------------------- Set color
	ModelCurrent->ConfUniform.SetColor(ObjectCurrent->Color);
	//---------------------- Set param Case
	ModelCurrent->ConfUniform.SetParamCase(Storage->Inputs->ParamCase);
	//---------------------- Set MVP
	ModelCurrent->ConfUniform.SetMVP(Storage->ConfigMVP->MVP);//(---)

	//VIEW param
	ModelCurrent->ConfUniform.SetView(Storage->ConfigMVP->View);

	//MODEL param
	ModelCurrent->ConfUniform.SetModel(Storage->ConfigMVP->Model);
}

bool SceneConstruction::SetObject(int indObj) {

	ObjectCurrent = Storage->GetObjectPrt(indObj);
	bool isVisible = ObjectCurrent->GetVisible();

	//-- test fail model
	/*if (ObjectCurrent->ModelPtr->Vertices.size() == 0 ||
		ObjectCurrent->ModelPtr->Normals.size() == 0 ||
		ObjectCurrent->ModelPtr->UV.size() == 0)
	{
		string failName = ObjectCurrent->Name;
		string failNameModel = ObjectCurrent->ModelPtr->Name;
	}*/

	ModelCurrent = ObjectCurrent->ModelPtr;
	IsFirstCurrentObject = indObj == 0;
	IsLastCurrentObject = countObjects == indObj;

	m_isUpdate = prevModelTexture != ModelCurrent->PathTexture || prevModel3D != ModelCurrent->PathModel3D;
	if (m_isUpdate) {
		prevModelTexture = ModelCurrent->PathTexture;
		prevModel3D = ModelCurrent->PathModel3D;
	}

	m_isUpdateShaderProgramm = prevShaderVert != ModelCurrent->PathShaderVertex || prevShaderFrag != ModelCurrent->PathShaderFrag;
	if (m_isUpdateShaderProgramm) {
		prevShaderVert = ModelCurrent->PathShaderVertex;
		prevShaderFrag = ModelCurrent->PathShaderFrag;
	}

	return isVisible;
}

void SceneConstruction::ObjectUpdate(int i) {

	bool isVisible = SetObject(i);
	if (!isVisible)
		return;

	PreparationDataFromShader();

	//TEST
	if (ObjectCurrent->Name == "CursorGUI")
	{
		auto t = true;
	}

	ObjectCurrent->Action();

	SetDataToShader();
}

void SceneConstruction::Update()
{
	ResetRooms();

	ClearScene();

	SetMouseEvents();

	GenMVP();

	countObjects = Storage->SceneObjectsLastIndex;

	if (IsBreakUpdate()) {
		DrawGraph();
		WorkingRooms();
		return;
	}

	for (int i = 0; i < countObjects + 1; i++)
	{
		ObjectUpdate(i);

		WorkingRooms();

		if (IsBreakUpdate())
			break;

		DrawGraph();
	}

	Debug("Clear Inputs");

	Storage->Inputs->MBT = -1;
	Storage->Inputs->Key = -1;
	Storage->Inputs->Action = -1;
	Storage->Inputs->ActionMouse = -1;
}

void SceneConstruction::Debug(string msg) {
	if (DebugMessage != msg)
	{
		DebugMessage = msg;
		DebugMessageAppend.append(msg);
		DebugMessageAppend.append("\n");
	}
}

bool SceneConstruction::IsBreakUpdate()
{
	if (Storage->SceneObjects.size() == 0)
		return true;
	if(countObjects != Storage->SceneObjectsLastIndex)
		return true;
	return false;
}

void SceneConstruction::SetMouseEvents() {

	Contrl->MouseEvents(Window, m_widthWindow, m_heightWindow, this);
	//bool isCursorClickEvent = Scene->Storage->Inputs->MBT == m_KeyPush && Scene->Storage->Inputs->ActionMouse == GLFW_PRESS;
}

void SceneConstruction::SetMouseButtonEvents() {

	Contrl->MouseButtonEvents(Window, this);
	//bool isCursorClickEvent = Scene->Storage->Inputs->MBT == m_KeyPush && Scene->Storage->Inputs->ActionMouse == GLFW_PRESS;
}



void SceneConstruction::GenMVP() {

	TransModel->GenMVP(m_widthWindow,
		m_heightWindow,
		Storage->Oper,
		Storage->Cam,
		Storage->ConfigMVP);
}

void SceneConstruction::ClearScene() {
	GLfloat red = 0.1f;
	GLfloat blue = 0.2f;
	GLfloat green = 0.3f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(red, blue, green, 1.0f);
}

void SceneConstruction::DrawGraph()
{
	//Mode fill 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	bool isIndex = ModelCurrent->IsIndex;

	
	GLint trianglesCount = ModelCurrent->TrianglesCount;
	if(ObjectCurrent->TrianglesCount > 0)
		trianglesCount = ObjectCurrent->TrianglesCount;

	if (isIndex) {
		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, trianglesCount * 3);
	//glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//--normal
	/*glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, &out_normals[0]);
	glDisableClientState(GL_NORMAL_ARRAY);*/

	glBindVertexArray(0);
}