#include "SceneConstruction.h"
//#include "SceneRoom.h"
//#include "CreatorModelData.h"
#include "SceneRoom.h"
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

SceneConstruction::SceneConstruction() {
}

SceneConstruction::SceneConstruction(GLFWwindow* window) {
	
	Window = window;
	Init();
}

//void SceneConstruction::Init(Operator* p_oper, Lighting* p_light, SceneParam* p_scene, CreatorModelData* p_storage, ControllerInput* p_inputs) {
void SceneConstruction::Init() {

	Rooms = vector<shared_ptr<SceneRoom>>();
	Cam = new Camera();
	Oper = new Operator();

	Light = new Lighting();
	Light->positionLight = vec3(0, 0, 0);

	Scene = new SceneParam;
	Storage = new CreatorModelData;
	Inputs = new ControllerInput;
	ConfigMVP = new CoreMVP();

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
	Storage->SceneParam = Scene;
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
	AddRoom(room);
}

void SceneConstruction::AddRoom(SceneRoom* room) {
	Rooms.push_back(std::make_unique<SceneRoom>(*room));
}

void SceneConstruction::WorkingRooms() {

	for(auto room : Rooms){
		room->Work();
	}
}

void SceneConstruction::SetDataToShaderAfter(bool isUpdate) {
	
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

	Storage->Camera = Cam;
	Storage->MVP = ConfigMVP;
	Storage->Operator = Oper;
	Storage->Inputs = Inputs;
	Storage->SceneParam = Scene;
}

void SceneConstruction::SetDataToShaderBefore(bool isUpdate) {
	
	bool isCubeModel = ObjectCurrent->ModelPtr->IsCubeModel;
	/*
	if(isCubeModel)
		ObjectCurrent->SetMesh();
	else if (isUpdate)
		ObjectCurrent->SetMesh();
		*/
	if (isCubeModel || isUpdate)
		ObjectCurrent->SetMesh();

	glBindVertexArray(ModelCurrent->VAO);

	if (isUpdate)
		ModelCurrent->SetModelInBuffer(isUpdate); // isUpdate -- ??????

	//-------------------- Set color
	//model->ConfUniform.SetColor(vec3(0, 0, 0), true);
	ModelCurrent->ConfUniform.SetColor(ObjectCurrent->Color);
	//---------------------- Set param Case
	//model->ConfUniform.SetParamCase(m_cameraG.paramCase);
	ModelCurrent->ConfUniform.SetParamCase(Inputs->ParamCase);
	//---------------------- Set MVP
	ModelCurrent->ConfUniform.SetMVP(ConfigMVP->MVP);

	//VIEW param
	ModelCurrent->ConfUniform.SetView(ConfigMVP->View);

	//MODEL param
	ModelCurrent->ConfUniform.SetModel(ConfigMVP->Model);
}

bool SceneConstruction::SetObject(int indObj, bool& isUpdate) {

	ObjectCurrent = Storage->GetObjectPrt(indObj);
	bool isShow = ObjectCurrent->IsShow();
	ModelCurrent = ObjectCurrent->ModelPtr;
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

	//bool isUpdate = SetObject(i);
	bool isUpdate = false;
	bool isShow = SetObject(i, isUpdate);
	if (!isShow)
		return;

	SetDataToShaderAfter(isUpdate);

	ObjectCurrent->Action();

	SetDataToShaderBefore(isUpdate);
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

		//====================== WorkingRooms

			//---------------- Marker Plane
			if (ObjectCurrent->Name == "Plane") {
				vec3 vectorsParams[10];

				ModelCurrent->ConfUniform.SetParamCase(55);

				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				vector<string> checkedZona;
			
				if (objectObserver->PlaneDownIndex != -1)
				{
					shared_ptr<Plane> planeV = ObjectCurrent->GetPlanePrt(objectObserver->PlaneDownIndex);
					vectorsParams[0] = planeV->V0;
					vectorsParams[1] = planeV->V1;
					vectorsParams[1] = planeV->V2;
					ModelCurrent->ConfUniform.SetVectorsParams(vectorsParams);
				}
			}

			//--- Target - cursor
			/*if (object->Name == "Mon") {
				std::shared_ptr <ObjectData> objectObserver = Storage.GetObjectPrt("Box");
				vec3 target = objectObserver->Postranslate;
				object->Target = vec3(target.x, object->Postranslate.y, target.z);
			}*/

			//--- Object Mon - cursor -- !!!!!!
			if (ObjectCurrent->Name == "Mon" && Inputs->ParamCase == 2) {
				vec4 vecPos = glm::inverse(ConfigMVP->View) * vec4(1);
				//vec3 posCursorObject = vec3(vecPos.x, vecPos.y, vecPos.z) + m_OperatorG.m_direction * 25.f;
				vec3 posCursorObject = vec3(vecPos.x, ObjectCurrent->Postranslate.y, vecPos.z) + Oper->m_direction * 20.f;
				ObjectCurrent->Postranslate = posCursorObject;
			}

			//--- Mouse camera object
			/*if (object->Name == "Box") {
				vec4 vecPos = glm::inverse(ConfigMVP.View) * vec4(1);
				vec3 posCursorObject = vec3(vecPos.x, vecPos.y, vecPos.z) + m_OperatorG.m_direction * 25.f;
				object->Postranslate = posCursorObject;
			}*/

			//--- Target
			/*if (object->Name == "Box2") {
				std::shared_ptr <ObjectData> objectObserver = Storage.GetObjectPrt("Mon");
				object->Postranslate = objectObserver->Target;
			}*/

			//object->TempVectors.push_back(nearestSphereIntersectionPoint);
			//object->TempVectors.push_back(nearestPolygonIntersectionPoint);
			//if (object->Name == "M_P_1") {
			//	std::shared_ptr <ObjectData> objectObserver = Storage.GetObjectPrt("Mon");
			//	if(objectObserver->TempVectors.size() > 1)
			//		object->Postranslate = objectObserver->TempVectors[0]; //nearestSphereIntersectionPoint - green
			//}
			if (ObjectCurrent->Name == "M_V_1") {
				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				if (objectObserver->TempVectors.size() > 0)
					ObjectCurrent->Postranslate = objectObserver->TempVectors[0];
			}
			if (ObjectCurrent->Name == "M_V_2") {
				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				if (objectObserver->TempVectors.size() > 1)
					ObjectCurrent->Postranslate = objectObserver->TempVectors[1];
			}
			if (ObjectCurrent->Name == "M_V_3") {
				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				if (objectObserver->TempVectors.size() > 2)
					ObjectCurrent->Postranslate = objectObserver->TempVectors[2];
			}
			if (ObjectCurrent->Name == "M_C_2") {
				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				ObjectCurrent->Postranslate = objectObserver->PlaneDownPosition;
			}


			if (ObjectCurrent->Name == "M_C_1") {
				std::shared_ptr <ObjectData> objectObserver = Storage->GetObjectPrt("Mon");
				ObjectCurrent->Postranslate = objectObserver->Postranslate; //nearestPolygonIntersectionPoint - blue
			}

			//----- Light position
			std::shared_ptr <ObjectData> objectLight = Storage->GetObjectPrt("Mon"); //Box2
			Light->positionLight = vec3(objectLight->Postranslate.x, objectLight->Postranslate.y + 15.f, objectLight->Postranslate.z);
			ModelCurrent->ConfUniform.SetPositionLight(Light->positionLight);

			//------ Set Mouse position
			ModelCurrent->ConfUniform.SetPositionMouse(Oper->PositionCursorModel);

		//=================== end WorkingRooms

		DrawGraph();
	}

	Storage->Inputs->MBT = -1;
	Storage->Inputs->Key = -1;
}

void SceneConstruction::SetMouseEvents() {

	Contrl->MouseEvents(Window, m_widthWindow, m_heightWindow, this);
}

void SceneConstruction::GenMVP() {

	TransModel->GenMVP(m_widthWindow,
		m_heightWindow,
		Oper,
		Cam,
		ConfigMVP);
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