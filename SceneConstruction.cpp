#include "SceneConstruction.h"
//#include "SceneRoom.h"
//#include "CreatorModelData.h"
#include "Rooms/SceneRoom.h"
#include "Rooms/RoomMarkerPlane.h"
#include "Rooms/RoomSerializeScene.h"
#include "Rooms/RoomUseInterface.h"
#include "Rooms/AspectFactoryObjects.h"
#include "Rooms/AspectDispatcherCommands.h"
#include "Rooms/AspectFamilyBonds.h"

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
#include "ShellObjects/BaseShell.h"

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

map<string, int> MapAlphabet;

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

	FillAlphabet();

	LoadDataModel();
	
	ConfigRoom();

	RefreshGUI();
}

void SceneConstruction::LoadDataModel()
{
	Storage = new CreatorModelData();
	Storage->Load();
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

	
	AspectFamilyBonds* aspectFamilyBonds = new AspectFamilyBonds("FamilyBonds", this);
	aspectFamilyBonds->Init();
	Rooms.push_back(make_unique<AspectFamilyBonds>(*aspectFamilyBonds));

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

void SceneConstruction::FillAlphabet() {
	
	MapAlphabet = map<string, int>{
		{"à",0},
		{"á",1},
		{"â",2},
		{"ã",3},
		{"ä",4},
		{"å",5},
		{"æ",6},
		{"ç",7},
		{"è",8},
		{"ê",9},
		{"ë",10},
		{"ì",11},
		{"í",12},
		{"î",13},
		{"ï",14},
		{"ð",15},
		{"ñ",16},
		{"ò",17},
		{"ó",18},
		{"ô",19},
		{"õ",20},
		{"ö",21},
		{"÷",22},
		{"ø",23},
		{"ø",24},
		{"ü",25},
		{"û",26},
		{"ý",27},
		{"þ",28},
		{"ÿ",29},
		{" ",30},

		{"1",31},
		{"2",32},
		{"3",33},
		{"4",34},
		{"5",35},
		{"6",36},
		{"7",37},
		{"8",38},
		{"9",39},
		{"0",40},
		{".",41},
		{",",42},
		{"!",43},
		{"?",44},
		{":",45},
		{"-",46},
		{"=",47},
		{"<",48},
		{">",49},
	};

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
		float f = glm::clamp((float)0.3,(float)2., DeltaTime);
		ObjectCurrent->Speed = 0.5f * f;
	}
	
	if (m_isUpdateShaderProgramm) {
		ShaderProgram = ModelCurrent->ShaderProgram;
		glUseProgram(ShaderProgram);
		//last_VAO = ModelCurrent->VAO;
	}

	bool isTextRepeat = ObjectCurrent->IsTextureRepeat;
	if (isTextureRepeat && !isTextRepeat)
	{
		isTextureRepeat = false;
		m_isUpdateUV = true;

	}
	if (isTextRepeat) {
		isTextureRepeat = isTextRepeat;
		m_isUpdateUV = true;
	}
}

void SceneConstruction::SetDataToShader() {
	
	//TEST
	auto nameTest =  ObjectCurrent->Name;
	auto nameTest2 = ObjectCurrent->ModelPtr->Name;

	bool IsSquareModel = ObjectCurrent->ModelPtr->IsSquareModel || ObjectCurrent->IsSquareModel;
	bool IsHexagonModel = ObjectCurrent->IsHexagonModel;
	bool isSkipGUI = Storage->SceneData->IsGUI == false && ObjectCurrent->IsGUI;
	bool isSkipDynamic = Storage->SceneData->IsGUI && !ObjectCurrent->IsGUI;
	//if (isSkipGUI || isSkipNotGUI) //Lite mode
	if (isSkipGUI) //Hard mode
		return;


	if (ObjectCurrent->IsGUI != m_isEnableGUI ||
		ObjectCurrent->IsGUI != Storage->SceneData->IsGUI) 
	{
		m_isEnableGUI = ObjectCurrent->IsGUI;
		m_isUpdateShaderProgramm = true;
	}

	if (m_isUpdateShaderProgramm) {
		m_isUpdateTexture = true;
		m_isUpdateMesh = true;
		m_isUpdateUV = true;
	}

	//-------------------- Set color
	ModelCurrent->ConfUniform->SetColor(ObjectCurrent->Color);
	//---------------------- Set param Case
	ModelCurrent->ConfUniform->SetParamCase(GetParamCase());

	//TEST
	/*if (ObjectCurrent->TypeObj == EditBox)
		ObjectCurrent->ParamValue = 1;*/

	ModelCurrent->ConfUniform->SetParamValue(ObjectCurrent->ParamValue);

	ModelCurrent->ConfUniform->SetStartTime(ObjectCurrent->StartTimer);

	//---------------------- Set MVP
	ModelCurrent->ConfUniform->SetMVP(Storage->ConfigMVP->MVP);

	//VIEW param
	ModelCurrent->ConfUniform->SetView(Storage->ConfigMVP->View);

	//MODEL param
	ModelCurrent->ConfUniform->SetModel(Storage->ConfigMVP->Model);


	bool isTransformMesh = IsHexagonModel || IsSquareModel || ObjectCurrent->IsTransformable;

	//if (!isSkipDynamic && (m_isUpdateMesh || isTransformMesh))  //Lite mode
	bool liteMode = !isSkipDynamic && (m_isUpdateMesh || IsSquareModel);
	if (liteMode)  //Lite mode
	//if (m_isUpdateMesh || IsSquareModel)
	{

		//m_currCashShader !!!
		if (isTransformMesh || isTransformMesh != m_isLastTransformMesh) {
			m_isLastTransformMesh = isTransformMesh;
			ObjectCurrent->SetMesh();
		}

		glBindVertexArray(ModelCurrent->VAO);

		if (!isSkipDynamic) 
		{
			ObjectCurrent->UpdateTextureUV();

			m_currCashShader = ObjectCurrent->GetCashStateUpdateDataToShader();
			if (m_currCashShader != m_lastCashShader) {
				m_lastCashShader = m_currCashShader;

				ObjectCurrent->SetDataToShader();
			}

			ObjectCurrent->UpdateNormalsToShader();

			ObjectCurrent->UpdateDataBufferToShader();
		}
		//-----------
	
	}
	else {
		glBindVertexArray(ModelCurrent->VAO);
	}

	if (m_isUpdateTexture)
	{
		ObjectCurrent->ModelPtr->SetTextureModel();
	}
	if (m_isUpdateUV)
	{
		ObjectCurrent->UpdateTextureUV();
	}


	//--- Set Width & height
}

float SceneConstruction::GetParamCase() {
	float paramCase = ObjectCurrent->ParamCase;
	if(paramCase == -1)
		paramCase = Storage->Inputs->ParamCase;
	return paramCase;
}

bool SceneConstruction::SetObject(int indNN) {

	IsFirstCurrentObject = indNN == 0;
	IsLastCurrentObject = countObjects == indNN;
	CurrentIndexOrder = indNN;
	bool isShowGUI = Storage->SceneData->IsGUI;
	int indObj = Storage->GetIndexObjBySortInd(indNN);
	ObjectCurrent = Storage->GetObjectPrt(indObj);
	ModelCurrent = ObjectCurrent->ModelPtr;
	

	//TEST -----------------
	/*glDisable(GL_BLEND);
	if (ObjectCurrent->TypeObj == TypeObject::CursorGUI ||
		ObjectCurrent->TypeObj == TypeObject::EditBox ||
		ObjectCurrent->TypeObj == TypeObject::TextBox) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}*/
	//----------------------

	if (ObjectCurrent->ShellIndex != -1) {
		ShellCurrent = Storage->GetObjectShellPrt(ObjectCurrent->ShellIndex);
		IsHaveShell = true;
	}
	else {
		ShellCurrent = nullptr;
		IsHaveShell = false;
	}

	bool isVisible = ObjectCurrent->GetVisible();
	if(!isVisible)
		return isVisible;

	m_isUpdateShaderProgramm = false;
	m_isUpdateTexture = false;
	m_isUpdateMesh = false;
	m_isUpdateUV = false;

	if (prevModelTexture != ModelCurrent->PathTexture) {
		prevModelTexture = ModelCurrent->PathTexture;
		m_isUpdateTexture = true;
	}
	if (prevModel3D != ModelCurrent->PathModel3D) {
		prevModel3D = ModelCurrent->PathModel3D;
		m_isUpdateMesh = true;
	}

	if (ShaderProgram != ModelCurrent->ShaderProgram)
	{
		m_isUpdateShaderProgramm = true;
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
				
	if (isShowGUI && !ObjectCurrent->IsGUI && countObjects > 50) //Lite mode
		isPause = true;
	if (!isShowGUI && ObjectCurrent->IsGUI && countObjects > 50) //Lite mode
		isPause = true;

	//Hard mode
	//isPause = false;
	if (isPause) //Lite mode
		return;

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

	SetInputTextEvents();

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
		//===========================================

		//if (!IsDraw || isBase)
		if (!IsDraw || isBase || (IsDraw && !isShowGUI))
			WorkingRooms();

		if (isShowGUI && !ObjectCurrent->IsGUI && countObjects > 50) //Lite mode
			continue;

		if (IsBreakUpdate())
			break;

		bool isVisible = ObjectCurrent->GetVisible();
		if (!isVisible)
			continue;

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
	if (Storage->SceneObjectsSize() == 0)
		return true;
	if(countObjects != Storage->SceneObjectsLastIndex)
		return true;
	return false;
}

void SceneConstruction::SetMouseEvents() {

	Contrl->MouseEvents(Window, m_widthWindow, m_heightWindow, this);
}

void SceneConstruction::SetInputTextEvents() {
	SymbolInput = "";
	int keyIndex = Storage->Inputs->Key;
	if (keyIndex == -1)
		return;
	SymbolInput = Contrl->GetSymbol(keyIndex);
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

		dispatcherCommands->DebugReadCommand(command, "ReadCommand");

		command->Enable = false;
		command->CommandType = None;
		return true;
	}

	return false;
}

bool SceneConstruction::IsCurrentObjectBackgroundFrameGUI() {
	return ObjectCurrent->Index == Storage->SceneData->IndexBackgroundGUIObj;
}

void SceneConstruction::AddCommand(CommandPack command) {
	dispatcherCommands->AddCommand(command);
}

void SceneConstruction::AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex, string keyOptions, int valueOptions) {

	dispatcherCommands->AddCommand(commandType, sourceIndex, targetIndex, keyOptions, valueOptions);
}

void SceneConstruction::AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex,
	int valueI, float valueF, vec4 valueV4, string valueS, bool isLong) {

	dispatcherCommands->AddCommand(commandType, sourceIndex, targetIndex, valueI, valueF, valueV4, valueS, isLong);

}

void SceneConstruction::AddCommand(TypeCommand commandType, bool isLong) {

	dispatcherCommands->AddCommand(commandType, -1, -1, -1, -0.1, vec4(), "", isLong);

}

void SceneConstruction::RefreshGUI() {
	AddCommand(TypeCommand::CheckStateObjectCommand, true);
}