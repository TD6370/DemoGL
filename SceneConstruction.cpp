#include "SceneConstruction.h"
//#include "SceneRoom.h"
//#include "CreatorModelData.h"
#include "Rooms/SceneRoom.h"
#include "Rooms/RoomMarkerPlane.h"
#include "Rooms/RoomSerializeScene.h"
#include "Rooms/RoomUseInterface.h"
#include "Rooms/AspectFactoryObjects.h"
#include "Rooms/AspectDispatcherCommands.h"

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
using std::shared_ptr;
using std::dynamic_pointer_cast;

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

	CurrentSceneCommand = CommandPack();

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

	//AspectDispatcherCommands* 
	dispatcherCommands = new AspectDispatcherCommands("DispatcherCommands", this);
	dispatcherCommands->Init();
	//Rooms.push_back(make_unique<AspectDispatcherCommands>(*dispatcherCommands));
	Rooms.push_back(make_unique<AspectDispatcherCommands>(*dispatcherCommands));
	//shared_ptr<ObjectButton> objBackGUI = std::dynamic_pointer_cast<ObjectButton>(objBackGUI_Data);
	dispatcherCommands = dynamic_pointer_cast<AspectDispatcherCommands>(Rooms[Rooms.size()-1]).get();
	
	factoryObjects = new AspectFactoryObjects("FactoryObjects", this);
	factoryObjects->Init();
	Rooms.push_back(make_unique<AspectFactoryObjects>(*factoryObjects));
	factoryObjects = dynamic_pointer_cast<AspectFactoryObjects>(Rooms[Rooms.size() - 1]).get();
	
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

	//~~~~~ Update speed ~~~~~~~~~
	if (ObjectCurrent->IsTransformable ||
		ObjectCurrent->IsNPC)
	{
		//float f = glm::max((float)0.3, DeltaTime);
		//f = glm::min((float)2., DeltaTime);
		float f = glm::clamp((float)0.3,(float)2., DeltaTime);

		//ObjectCurrent->Speed = 0.5f * glm::max((float)1., DeltaTime);
		ObjectCurrent->Speed = 0.5f * f;
	}
	
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
	auto nameTest =  ObjectCurrent->Name;
	auto nameTest2 = ObjectCurrent->ModelPtr->Name;

	bool IsSquareModel = ObjectCurrent->ModelPtr->IsSquareModel || ObjectCurrent->IsSquareModel;
	bool isSkipGUI = Storage->SceneData->IsGUI == false && ObjectCurrent->IsGUI;
	//bool isEnableGUI = Storage->SceneData->IsGUI == true && ObjectCurrent->IsGUI;
	
	if (isSkipGUI)
		return;

	if (m_isEnableGUI != ObjectCurrent->IsGUI) {
		m_isEnableGUI = ObjectCurrent->IsGUI;
		m_isUpdate = true;
	}

	if (IsSquareModel || m_isUpdate)
		ObjectCurrent->SetMesh();

	glBindVertexArray(ModelCurrent->VAO);

	ObjectCurrent->SetDataToShader(m_isUpdate);

	//-------------------- Set color
	ModelCurrent->ConfUniform.SetColor(ObjectCurrent->Color);
	//---------------------- Set param Case
	//ModelCurrent->ConfUniform.SetParamCase(Storage->Inputs->ParamCase);
	ModelCurrent->ConfUniform.SetParamCase(GetParamCase());
	//---------------------- Set MVP
	ModelCurrent->ConfUniform.SetMVP(Storage->ConfigMVP->MVP);//(---)

	//VIEW param
	ModelCurrent->ConfUniform.SetView(Storage->ConfigMVP->View);

	//MODEL param
	ModelCurrent->ConfUniform.SetModel(Storage->ConfigMVP->Model);

	//------ Set Mouse position
	//ModelCurrent->ConfUniform.SetModel(Storage->ConfigMVP->Model);


	//--- Set Width & height
}

float SceneConstruction::GetParamCase() {
	float paramCase = ObjectCurrent->ParamCase;
	if(paramCase == -1)
		paramCase = Storage->Inputs->ParamCase;
	return paramCase;
}

bool SceneConstruction::SetObject(int indNN) {

	bool isShowGUI = Storage->SceneData->IsGUI;

	int indObj = Storage->SortObjectIndex[indNN];

	ObjectCurrent = Storage->GetObjectPrt(indObj);
	bool isVisible = ObjectCurrent->GetVisible();
	if(!isVisible)
		return isVisible;

	ModelCurrent = ObjectCurrent->ModelPtr;
	IsFirstCurrentObject = indNN == 0;
	IsLastCurrentObject = countObjects == indNN;
	m_isUpdate = false;

	if (prevModelTexture != ModelCurrent->PathTexture) {
		prevModelTexture = ModelCurrent->PathTexture;
		m_isUpdate = true;
	}
	if (prevModel3D != ModelCurrent->PathModel3D) {
		prevModel3D = ModelCurrent->PathModel3D;
		m_isUpdate = true;
	}

	if (prevShaderVert != ModelCurrent->PathShaderVertex) {
		m_isUpdateShaderProgramm = true;
		prevShaderVert = ModelCurrent->PathShaderVertex;
	}
	if (prevShaderFrag != ModelCurrent->PathShaderFrag) {
		m_isUpdateShaderProgramm = true;
		prevShaderFrag = ModelCurrent->PathShaderFrag;
	}
	return isVisible;
}


void SceneConstruction::ObjectUpdate(int i) {

	bool isShowGUI = Storage->SceneData->IsGUI;
	bool isBase = VersionUpdate == 0;
	bool isDraw = !IsDeltaUpdateLogic;
	bool isPause = false; 

	bool isVisible = SetObject(i);
	if (!isVisible)
		return;
	
	bool isUpdate = (isDraw || isBase || isShowGUI);
	if (!isUpdate)
		return;

	//-------------- Start next ObjectCurrent ---------------------
	if (isShowGUI && !ObjectCurrent->IsGUI && countObjects > 50)
		isPause = true;

	if (isDraw || isBase) 
	{
		PreparationDataFromShader();
		
		if(!isPause)
			ObjectCurrent->Action();

		SetDataToShader();
	}
}

void SceneConstruction::Update()
{
	bool isShowGUI = Storage->SceneData->IsGUI;
	bool IsDraw = !IsDeltaUpdateLogic;
	bool isBase = VersionUpdate == 0;
	
	if (!IsDraw || isBase)
		ResetRooms();

	if(IsDraw || isBase)
		ClearScene();

	SetMouseEvents();

	if (IsDraw || isBase)
		GenMVP();

	countObjects = Storage->SceneObjectsLastIndex;
	if (IsBreakUpdate()) {
		if (IsDraw || isBase)
			DrawGraph();
		if (!IsDraw || isBase)
			WorkingRooms();
		return;
	}

	if (!IsDraw || isBase)
		FactoryObjectsWork();

	for (int i = 0; i < countObjects + 1; i++)
	{
		//if (IsDraw || isBase || isShowGUI) //TEST&&1
			ObjectUpdate(i);

		//if (!IsDraw || isBase)
		if (!IsDraw || isBase || (IsDraw && !isShowGUI))
			WorkingRooms();

		if (IsBreakUpdate())
			break;

		if (IsDraw || isBase)
			DrawGraph();
	}
	
	
	bool checkClearInputs;
	if (isShowGUI)
		checkClearInputs = !IsDraw;
	else
		checkClearInputs = IsDraw;

	if (checkClearInputs || isBase) {
		Storage->Inputs->MBT = -1;
		Storage->Inputs->Key = -1;
		Storage->Inputs->Action = -1;
		Storage->Inputs->ActionMouse = -1;
	}
}

/*
void UpdateDraw() {
	bool isBase = VersionUpdate == 0;

	ClearScene();

	SetMouseEvents();

	GenMVP();

	countObjects = Storage->SceneObjectsLastIndex;
	if (IsBreakUpdate()) {
		DrawGraph();
		return;
	}

	for (int i = 0; i < countObjects + 1; i++)
	{
		//if (!IsDraw || isBase)
		ObjectUpdate(i);

		if (IsBreakUpdate())
			break;

		DrawGraph();
	}

	
	Storage->Inputs->MBT = -1;
	Storage->Inputs->Key = -1;
	Storage->Inputs->Action = -1;
	Storage->Inputs->ActionMouse = -1;
}

void UpdateLogac()
{
	ResetRooms();

	SetMouseEvents();

	countObjects = Storage->SceneObjectsLastIndex;
	if (IsBreakUpdate()) {
		WorkingRooms();
		return;
	}

	FactoryObjectsWork();

	for (int i = 0; i < countObjects + 1; i++)
	{
		ObjectUpdate(i);

		WorkingRooms();

		if (IsBreakUpdate())
			break;

	}
}
*/

/*
void SceneConstruction::ObjectUpdate(int i) {

	bool isVisible = SetObject(i);
	if (!isVisible)
		return;

	PreparationDataFromShader();

	ObjectCurrent->Action();

	SetDataToShader();
}

void SceneConstruction::Update(bool IsDraw)
{

	ResetRooms();

	ClearScene();

	SetMouseEvents();
	
	GenMVP();

	FactoryObjectsWork();
	countObjects = Storage->SceneObjectsLastIndex;

	for (int i = 0; i < countObjects + 1; i++)
	{
		ObjectUpdate(i);

		WorkingRooms();
	
		DrawGraph();
	}

	//Debug("Clear Inputs");

	Storage->Inputs->MBT = -1;
	Storage->Inputs->Key = -1;
	Storage->Inputs->Action = -1;
	Storage->Inputs->ActionMouse = -1;
}
*/

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
}

void SceneConstruction::SetMouseButtonEvents() {

	Contrl->MouseButtonEvents(Window, this);
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

void SceneConstruction::FactoryObjectsWork() {

	factoryObjects->Work();
}

bool SceneConstruction::ReadCommand(TypeCommand commandType)
{
	CommandPack* command = &CurrentSceneCommand;
	if (command->Enable && command->CommandType == commandType) {
		command->Enable = false;
		command->CommandType = None;
		return true;
	}

	return false;
}


bool SceneConstruction::IsCurrentObjectBackgroundFrameGUI() {

	return ObjectCurrent->Index == Storage->SceneData->IndexGUIObj;
}

void SceneConstruction::AddCommand(CommandPack command) {
	dispatcherCommands->AddCommand(command);
}

void SceneConstruction::AddCommand(TypeCommand commandType, int targetIndex, int sourceIndex, string keyOptions, int valueOptions) {

	dispatcherCommands->AddCommand(commandType, targetIndex, sourceIndex, keyOptions, valueOptions);
}
