#include "SceneConstructor.h"
//#include "AspectBase.h"
//#include "CreatorModelData.h"
#include "Rooms/AspectBase.h"
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
#include "./Components/RenderComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "..\SceneConstructor.h"
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
map<string, int> MapAlphabetEng;

SceneConstructor::SceneConstructor() {

	
}

SceneConstructor::SceneConstructor(GLFWwindow* window) {
	
	Window = window;
	Init();
}

void SceneConstructor::Init() {

	Aspects = vector<shared_ptr<AspectBase>>();
	
	Light = new Lighting();
	Light->positionLight = vec3(0, 0, 0);

	Storage = new CreatorModelData();

	TransModel = new TransformModel();

	Contrl = new Controllers(MapAlphabet, MapAlphabetEng);
	
	CurrentSceneCommand = CommandPack();

	CommandsAttribute = AttributeCommands();

	m_serializer = new SceneSerialize();

	LoadDataModel();
	
	ConfigRoom();

	RefreshGUI();
}

void SceneConstructor::LoadDataModel()
{
	Storage = new CreatorModelData();
	Storage->Load();
}

//*** introduction

void SceneConstructor::ConfigRoom() {

	//*** create Aspect
	AspectBase* aspect = new AspectBase("Base", this);
	aspect->Init();
	//*** join point
	AddAspect(aspect);

	RoomMarkerPlane* aspectMarkers = new RoomMarkerPlane("Markers", this);
	aspectMarkers->Init();
	Aspects.push_back(make_unique<RoomMarkerPlane>(*aspectMarkers));
		
	RoomSerializeScene* aspectSerializator = new RoomSerializeScene("Serialize", this);
	aspectSerializator->Init();
	Aspects.push_back(make_unique<RoomSerializeScene>(*aspectSerializator));

		
	AspectFamilyBonds* aspectFamilyBonds = new AspectFamilyBonds("FamilyBonds", this);
	aspectFamilyBonds->Init();
	Aspects.push_back(make_unique<AspectFamilyBonds>(*aspectFamilyBonds));

	RoomUseInterface* aspectInterface = new RoomUseInterface("Interface", this);
	Aspects.push_back(make_unique<RoomUseInterface>(*aspectInterface));
	Aspects.back()->Init();
		
	dispatcherCommands = new AspectDispatcherCommands("DispatcherCommands", this);
	dispatcherCommands->Init();
	Aspects.push_back(make_unique<AspectDispatcherCommands>(*dispatcherCommands));
	dispatcherCommands = dynamic_pointer_cast<AspectDispatcherCommands>(Aspects.back()).get();
	
		
	factoryObjects = new AspectFactoryObjects("FactoryObjects", this);
	factoryObjects->Init();
	Aspects.push_back(make_unique<AspectFactoryObjects>(*factoryObjects));
	factoryObjects = dynamic_pointer_cast<AspectFactoryObjects>(Aspects.back()).get();
}

void SceneConstructor::AddAspect(AspectBase* aspect) {
	Aspects.push_back(make_unique<AspectBase>(*aspect));
}

void SceneConstructor::ResetAspects() {

	for (auto aspect : Aspects) {
		aspect->IsOnceComplete = false;
	}
}

//*** Pointcut
void SceneConstructor::WorkingAspects() {
	for (auto spect : Aspects) {
		spect->Work();
	}
}

void SceneConstructor::PreparationDataFromShader() {

	//~~~~~ Update speed ~~~~~~~~~
	if ((ObjectCurrent->IsTransformable ||
		ObjectCurrent->IsNPC) && IsSpeedDeltaTime)
	{
		if (VersionUpdate == 1)
		{
			float f = glm::clamp((float)0.3, (float)1.2, DeltaTime);
			ObjectCurrent->Speed = 0.5f * f;
		}
	}
	
	if (m_isUpdateShaderProgramm) {
		ShaderProgram = ModelCurrent->Render->ShaderProgram;
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

void SceneConstructor::SetDataToShader() {

	bool IsSquareModel = ObjectCurrent->ModelPtr->MeshData.IsSquareModel || ObjectCurrent->IsSquareModel;
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

	ConfigUniform* uniform = ModelCurrent->Render->ConfUniform;

	//-------------------- Set Uniforms
	uniform->SetColor(ObjectCurrent->MaterialData.Color);
	uniform->SetParamCase(GetParamCase());
	uniform->SetParamValue(ObjectCurrent->ParamValue);
	uniform->SetStartTime(ObjectCurrent->StartTimer);

	if (VersionUpdate == 2)
		uniform->SetDeltaStartTime(DeltaTime);
	else
		uniform->SetDeltaStartTime(1.);

	//---------------------- Set MVP
	uniform->SetMVP(Storage->ConfigMVP->MVP);

	//VIEW param
	uniform->SetView(Storage->ConfigMVP->View);

	//MODEL param
	uniform->SetModel(Storage->ConfigMVP->Model);

	bool isTransformMesh = IsHexagonModel || IsSquareModel || ObjectCurrent->IsTransformable;

	//if (!isSkipDynamic && (m_isUpdateMesh || isTransformMesh))  //Lite mode
	bool liteMode = !isSkipDynamic && (m_isUpdateMesh || IsSquareModel);
	if (liteMode)  //Lite mode
	{
		if (isTransformMesh || isTransformMesh != m_isLastTransformMesh) {
			m_isLastTransformMesh = isTransformMesh;
			ObjectCurrent->ToShader_Mesh();
		}

		glBindVertexArray(ObjectCurrent->GetVAO());

		if (!isSkipDynamic) 
		{
			m_isUpdateUV = true;

			m_currCashShader = ObjectCurrent->GetCashStateUpdateDataToShader();
			if (m_currCashShader != m_lastCashShader) {
				m_lastCashShader = m_currCashShader;

				ObjectCurrent->ToShader_OtherData();
			}

			ObjectCurrent->ToShader_Normals();

			ObjectCurrent->ToShader_CustomBuffer();
		}
	}
	else {
		glBindVertexArray(ObjectCurrent->GetVAO());
	}

	if (m_isUpdateTexture)
	{
		ObjectCurrent->ToShader_Texture();
	}
	if (m_isUpdateUV)
	{
		ObjectCurrent->ToShader_UV();
	}
}

float SceneConstructor::GetParamCase() {
	float paramCase = ObjectCurrent->ParamCase;
	if(paramCase == -1)
		paramCase = Storage->Inputs->ParamCase;
	return paramCase;
}

bool SceneConstructor::SetObject(int indNN) {

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
		//ShellCurrent = Storage->GetObjectShellPrt(ObjectCurrent->ShellIndex);
		ShellCurrent = ObjectCurrent->Shell; // Storage->GetObjectShellPrt(ObjectCurrent->ShellIndex);
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

	if (prevModelTexture != ModelCurrent->MaterialData.PathTexture) {
		prevModelTexture = ModelCurrent->MaterialData.PathTexture;
		m_isUpdateTexture = true;
	}
	if (prevModel3D != ModelCurrent->MeshData.PathModel3D) {
		prevModel3D = ModelCurrent->MeshData.PathModel3D;
		m_isUpdateMesh = true;
	}

	if (ShaderProgram != ModelCurrent->Render->ShaderProgram)
	{
		m_isUpdateShaderProgramm = true;
	}
	return isVisible;
}


void SceneConstructor::ObjectUpdate(int i) {

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
	if (isPause) //Lite mode
		return;
	
	if (isDraw || isBase) 
	{
		PreparationDataFromShader();
		
		//---------------------------------
		if(!isPause)
			ObjectCurrent->Action(); // --- Only calculate transform geometry

		SetDataToShader();
	}
}

void SceneConstructor::Update()
{
	bool isShowGUI = Storage->SceneData->IsGUI;
	bool IsDraw = !IsDeltaUpdateLogic;
	bool isBase = VersionUpdate == 0;
	
	if (!IsDraw || isBase)
		ResetAspects();

	if(IsDraw || isBase)
		ClearScene();

	SetMouseEvents();

	//SetInputTextEvents();

	if (IsDraw || isBase)
		GenMVP();

	countObjects = Storage->SceneObjectsLastIndex;
	if (IsBreakUpdate()) {
		if (IsDraw || isBase)
			DrawGraph();
		if (!IsDraw || isBase)
			WorkingAspects();
		return;
	}

	if (!IsDraw || isBase)
		FactoryObjectsWork();

	
	Storage->Inputs->IsReading = true;
	SetInputTextEvents();

	for (int i = 0; i < countObjects + 1; i++)
	{
		//---- TEST DEBUG
		assert(Storage->SceneObjectsLastIndex < 1000);

		ObjectUpdate(i); //--- Calculate geometry
		//===========================================

		if (!IsDraw || isBase || (IsDraw && !isShowGUI))
			WorkingAspects(); //--- Calculate logic

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

	//---- AfterBuild
	factoryObjects->LastWork();

	
	if (Storage->Inputs->IsReading || isBase)
	{
		Storage->Inputs->IsReading = false;
		Storage->Inputs->Mode = -1;
		Storage->Inputs->MBT = -1;
		Storage->Inputs->Key = -1;
		Storage->Inputs->Action = -1;
		Storage->Inputs->ActionMouse = -1;
	}
}


void SceneConstructor::Debug(string msg) {
	if (DebugMessage != msg)
	{
		DebugMessage = msg;
		DebugMessageAppend.append(msg);
		DebugMessageAppend.append("\n");
	}
}

bool SceneConstructor::IsBreakUpdate()
{
	if (Storage->SceneObjectsSize() == 0)
		return true;
	if(countObjects != Storage->SceneObjectsLastIndex)
		return true;
	return false;
}

void SceneConstructor::SetMouseEvents() {

	Contrl->MouseEvents(Window, m_widthWindow, m_heightWindow, this);
}

void SceneConstructor::SetInputTextEvents() {
	SymbolInput = "";
	int keyIndex = Storage->Inputs->Key;
	if (keyIndex == -1)
		return;
	SymbolInput = Contrl->GetSymbol(keyIndex);
}

void SceneConstructor::SetMouseButtonEvents() {

	Contrl->MouseButtonEvents(Window, this);
}

void SceneConstructor::GenMVP() {

	TransModel->GenMVP(m_widthWindow,
		m_heightWindow,
		Storage->Oper,
		Storage->Cam,
		Storage->ConfigMVP);
}

void SceneConstructor::ClearScene() {
	GLfloat red = 0.1f;
	GLfloat blue = 0.2f;
	GLfloat green = 0.3f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(red, blue, green, 1.0f);
}

void SceneConstructor::DrawGraph()
{
	//Mode fill 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	bool isIndex = ModelCurrent->Render->IsIndex;

	
	GLint trianglesCount = ModelCurrent->MeshData.TrianglesCount;
	if(ObjectCurrent->MeshData.TrianglesCount > 0)
		trianglesCount = ObjectCurrent->MeshData.TrianglesCount;

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

void SceneConstructor::FactoryObjectsWork() {

	factoryObjects->Work();
}

bool SceneConstructor::ReadCommand(TypeCommand commandType)
{
	if(!CurrentSceneCommand.Enable || CurrentSceneCommand.CommandType != commandType)
		return false;

	CommandPack* command = &CurrentSceneCommand;
	dispatcherCommands->DebugReadCommand(command, "ReadCommand");
	command->Enable = false;
	command->CommandType = None;
	return true;
}

bool SceneConstructor::IsCurrentObjectBackgroundFrameGUI() {
	return ObjectCurrent->Index == Storage->SceneData->IndexBackgroundGUIObj;
}

void SceneConstructor::AddCommand(CommandPack command) {
	dispatcherCommands->AddCommand(command);
}


void SceneConstructor::AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex, vector<string> keyOptions, vector<int> valueOptions,
	int valueI, float valueF, vec4 valueV4, string valueS, string description, bool isLong)
{
	dispatcherCommands->AddCommand(commandType, sourceIndex, targetIndex, keyOptions, valueOptions, valueI, valueF, valueV4, valueS, description, isLong);
}

void SceneConstructor::AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex,
	int valueI, float valueF, vec4 valueV4, string valueS, string description, bool isLong) {

	dispatcherCommands->AddCommand(commandType, sourceIndex, targetIndex, valueI, valueF, valueV4, valueS, description, isLong);

}

void SceneConstructor::AddCommand(TypeCommand commandType, bool isLong) {

	dispatcherCommands->AddCommand(commandType, -1, -1, -1, -0.1, vec4(), "", "", isLong);
}

void SceneConstructor::RefreshGUI() {

	AddCommand(TypeCommand::CheckStateObjectCommand, true);
}


void SceneConstructor::RunCommandCreateObject(TypeObject typeCreate, string typeObjectText, vec3 pos, bool isGuiEdit) {

	string typeObjectAttr = CommandsAttribute.TypeObjectAttr;
	string nameCommandList = "";
	int isInitPos = -1;
	if(pos != vec3(-1))
		isInitPos = 1;

	if (typeCreate == TypeObject::ListBox ||
		typeCreate == TypeObject::ListTextBox ||
		typeCreate == TypeObject::ListEditBox)
	{
		string nameListCommands = CommandsAttribute.BaseListCommand;
		nameListCommands = CommandsAttribute.TypesObjectListCommand; //--default : TYPES
		if (typeCreate == TypeObject::ListTextBox || typeCreate == TypeObject::ListEditBox)
			nameListCommands = CommandsAttribute.ObjectFieldsListCommand; // -- default : FIELDS

		AddCommand(TypeCommand::CreateObject, -1, isGuiEdit, { typeObjectAttr }, { typeCreate },
			-1,
			-1,
			vec4(pos.x, pos.y, pos.z, isInitPos),
			nameListCommands);
	}
	else {
		AddCommand(TypeCommand::CreateObject, -1, isGuiEdit, { typeObjectAttr }, { typeCreate },
			typeCreate,
			0.0,
			vec4(pos.x, pos.y, pos.z, isInitPos),
			nameCommandList,
			typeObjectText);
	}
}

//====================== = Get Value by Object

void SceneConstructor::CreateObjectListFieldValue(shared_ptr<ObjectData> obj) {
	m_serializer->Save(obj, false, true);
}
 
void SceneConstructor::SaveObjectFieldValueFromList(shared_ptr<ObjectData> obj, string steamFields) {
		
	ObjectFileds objFields;
	vector<ObjectFiledsSpecific> objectDataSpecific;
	std::stringstream ifstramFields(steamFields);

	m_serializer->CreateStructFileds(ifstramFields, objFields);
	Storage->SaveAndInitObject(&objFields, objectDataSpecific, obj);
}

string SceneConstructor::GetObjectValueByFieldName(string typeFieldName) {

	string value = m_serializer->GetFieldValueByName(typeFieldName);
	return value;
}

map<string, string> SceneConstructor::GetObjectListFieldValue(shared_ptr<ObjectData> obj) {

	m_serializer->Save(obj, false, true);

	return m_serializer->GetObjectListFieldValue();
}

vector<string> SceneConstructor::GetObjectListFields() {
	return m_serializer->GetObjectListFields();
}

//========================

vector<CommandPack> SceneConstructor::GetListCommand(string nameList) {

	return dispatcherCommands->StaticListCommand[nameList];
}

//-----

shared_ptr<ObjectData> SceneConstructor::CreateObjectNull()
{
	return Storage->CreateObjectNull();
}

bool SceneConstructor::IsValidTypeObjectWorld(TypeObject typeObj) {
	return typeObj == TypeObject::Solid || 
			typeObj == TypeObject::Block || 
			typeObj == TypeObject::Tree || 
			typeObj == TypeObject::NPC;
}