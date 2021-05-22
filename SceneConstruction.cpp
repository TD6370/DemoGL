#include "SceneConstruction.h"
//#include "SceneRoom.h"
//#include "CreatorModelData.h"
#include "Rooms\SceneRoom.h"
#include "Rooms\RoomMarkerPlane.h"
#include "Rooms\RoomSerializeScene.h"

#include "CreatorModelData.h"
#include "TransformModel.h"
#include "Controllers.h"
#include "ModelData.h"
#include "ObjectsTypes/ObjectData.h"
#include "ObjectsTypes/ObjectDynamic.h"

#include "CoreSettings.h"
//#include "WorldCollision.h"
#include "GeometryLib.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

//void SceneConstruction::Init(Operator* p_oper, Lighting* p_light, SceneParam* p_scene, CreatorModelData* p_storage, ControllerInput* p_inputs) {
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
	//bool isGen = true;
	bool isGen = false;
	Storage = new CreatorModelData();
	//Storage->SceneParam = Scene; //(---)
	if (isGen)
	{
		Storage->LoadModels();
		Storage->GenerateObjects();
	}
	else {
		Storage->Load();
	}
}

void SceneConstruction::ConfigRoom() {

	SceneRoom* room = new SceneRoom("Base", this);
	room->Init();
	AddRoom(room);

	RoomMarkerPlane* roomMarkers = new RoomMarkerPlane("Markers", this);
	roomMarkers->Init();
	Rooms.push_back(make_unique<RoomMarkerPlane>(*roomMarkers));

	RoomSerializeScene* roomSerializator = new RoomSerializeScene("Serialize", this);
	roomSerializator->Init();
	Rooms.push_back(make_unique<RoomSerializeScene>(*roomSerializator));
}

void SceneConstruction::AddRoom(SceneRoom* room) {
	Rooms.push_back(make_unique<SceneRoom>(*room));
}

void SceneConstruction::WorkingRooms() {

	for(auto room : Rooms){
		room->Work();
	}
}

void SceneConstruction::PreparationDataFromShader(bool isUpdate) {
	
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

	if (isUpdate) {
		ShaderProgram = ModelCurrent->ShaderProgram;
		glUseProgram(ShaderProgram);
		last_VAO = ModelCurrent->VAO;
	}
}

void SceneConstruction::SetDataToShader(bool isUpdate) {
	
	bool isCubeModel = ObjectCurrent->ModelPtr->IsCubeModel;

	if (isCubeModel || isUpdate)
		ObjectCurrent->SetMesh();

	glBindVertexArray(ModelCurrent->VAO);

	if (isUpdate)
		ModelCurrent->SetModelInBuffer(isUpdate); // isUpdate -- ??????

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

bool SceneConstruction::SetObject(int indObj, bool& isUpdate) {

	ObjectCurrent = Storage->GetObjectPrt(indObj);
	bool isShow = ObjectCurrent->IsShow();
	ModelCurrent = ObjectCurrent->ModelPtr;
	IsFirstCurrentObject = indObj == 0;
	IsLastCurrentObject = countObjects == indObj;
	
	//----- Start
	if (prevModelTexture != ModelCurrent->PathTexture || prevModelModel3D != ModelCurrent->PathModel3D)
	{
		isUpdate = true;
		prevModelTexture = ModelCurrent->PathTexture;
		prevModelModel3D = ModelCurrent->PathModel3D;
	}
	
	return isShow;
}

void SceneConstruction::ObjectUpdate(int i) {

	bool isUpdate = false;
	bool isShow = SetObject(i, isUpdate);
	if (!isShow)
		return;

	PreparationDataFromShader(isUpdate);

	ObjectCurrent->Action();

	SetDataToShader(isUpdate);
}

void SceneConstruction::Update()
{
	ClearScene();

	SetMouseEvents();

	GenMVP();

	countObjects = Storage->SceneObjectsLastIndex;

	for (int i = 0; i < countObjects + 1; i++)
	{
		ObjectUpdate(i);

		WorkingRooms();

		DrawGraph();
	}

	Storage->Inputs->MBT = -1;
	Storage->Inputs->Key = -1;
	Storage->Inputs->Action = -1;
}

void SceneConstruction::SetMouseEvents() {

	Contrl->MouseEvents(Window, m_widthWindow, m_heightWindow, this);
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