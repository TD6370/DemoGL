
//---------------------
//#include "../CreatorModelData.h"
#include "../WorldCollision.h" //--<< #include "CreatorModelData.h"  
//#include "../CoreSettings.h"
#include "../ModelData.h"
#include "../GeometryLib.h"
//------------------------
#include "ObjectData.h"
//#include "ObjectButton.h"

#include "../Serialize/SceneSerialize.h"
#include "../GeomertyShapes/ShapeBase.h"
#include "../GeomertyShapes/ShapeHexagon.h"
#include "../GeomertyShapes/ShapeSquare.h"
#include "../Components/RenderComponent.h"
#include "../Components/TextBoxComponent.h"


#include "../ShellObjects/BaseShell.h"

#include "../CreatorModelData.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <iostream>
//#include <fstream>
#include <string>
#include <vector>

using std::shared_ptr;

ObjectData::ObjectData() {
};

ObjectData::ObjectData(int p_index,
	std::shared_ptr<ModelData> p_model,
	TypeObject p_typeObj,
	vec3 p_pos)
{
	Index = p_index;
	TypeObj = p_typeObj;
	Postranslate = p_pos;
	ModelPtr = p_model;

	m_keyPosSectorStr = "";
	PlaneDownIndex = -1;

	Shape = new ShapeBase();
	Shape->UpdateShapeInfo(this);
	
	SceneCommand = new CommandPack();
	SceneCommand->Enable = false;

	StartColor = MaterialData.Color;
	Layer = TypeLayer::LayerNone;
	EngineData = new DataEngine();
	

}

ObjectData::~ObjectData()
{

}

void ObjectData::InitRender() {

	MeshData = Mesh();
	MaterialData = Material();
	//--- save name
	MeshData.PathModel3D = ModelPtr->MeshData.PathModel3D;

	Render = new RenderComponent(MaterialData, MeshData);
	Render->Clone(ModelPtr->Render);
}

void ObjectData::InitData()
{
	InitRender();

	if (IsTextBoxComponent)
	{
		TextBox = new TextBoxComponent();
		TextBox->Init(this);
	}

	switch (TypeObj)
	{
		case Block:
		case Tree:
		case Terra:
		case Bullet:
			ActionObjectCurrent = Stay;
			break;
		default:
			ActionObjectCurrent = Lock;
			break;
	}
}

void ObjectData::Action()
{
	RunAction();
}

void ObjectData::RunAction() {

	if (ActionObjectCurrent != Lock)
	{
		switch (ActionObjectCurrent)
		{
		case Starting:
			GenStartPosition();
			break;
		case Stay:
			break;
		default:
			break;
		}
	}
	ActionBase();
}

//----------- Actions

void ObjectData::ActionBase() {

	ControlsEvents();
	RunTransform();
	//CheckParentState();
}

void ObjectData::GenStartPosition() {
	World WorldSetting;
	int distanceSpawnZone = WorldSetting.Radius - 5;
	int x = linearRand(distanceSpawnZone * -1, distanceSpawnZone);
	int z = linearRand(distanceSpawnZone * -1, distanceSpawnZone);
	Postranslate = vec3(x, -50, z);
	NewPostranslate = vec3(x, -50, z);

	CheckStartPosition();
}


void ObjectData::RunTransform()
{
	glm::mat4 trans = Transform(1, EngineData->Inputs->ParamCase, false,
		glm::mat4(1.0f),
		Postranslate,
		TranslateAngle,
		Size);

	TransformResult = trans;
	//---------------------------------------------------
	ModelPtr->Render->ConfUniform->SetTransform(&trans);
}

void ObjectData::CheckedRefresh() { }

void ObjectData::Click() {

	if (IsTextBoxComponent)
	{
		TextBox->Click();
	}
}

void ObjectData::ActionWork() {

	if (IsTextBoxComponent)
	{
		TextBox->ActionWork();
	}
}

void ObjectData::CheckStartPosition() {}

//================================== To shader mem

string ObjectData::GetCashStateUpdateDataToShader() {

	if (IsTextBoxComponent)
	{
		return TextBox->GetCashStateUpdateDataToShader();
	}

	string cash(1, Index);
	return cash;
}

GLuint ObjectData::GetVAO() {

	if (Render->Buffers.VAO != EmptyID)
		return Render->Buffers.VAO;
	return ModelPtr->Render->Buffers.VAO;
}

void ObjectData::ToShader_OtherData() {

	Render->SetDataToShader(*this);
}

void ObjectData::ToShader_CustomBuffer() {
	Render->UpdateCustomBuffer();
}

void ObjectData::ToShader_Texture() {
	
	ModelPtr->Render->UpdateTexture();
}

void ObjectData::ToShader_Normals() {

	if (MeshData.Normals.size() != 0)
		Render->UpdateNormals();
	else
		ModelPtr->Render->UpdateNormals();
}

void ObjectData::ToShader_Mesh() {

	//---- Object Physic
	if (MeshData.Vertices.size() != 0) {
		Render->UpdateVAO();
	}
	else
		ModelPtr->Render->UpdateVAO();
}

void ObjectData::ToShader_UV() {


	if (MeshData.UV.size() == 0) {

		if (IsGUI) //FIX
		{
			if (!Render->IsLoadedIntoMem_Buffer) {
				Render->IsLoadedIntoMem_Buffer = true;
				ModelPtr->Render->IsLoadedIntoMem_UV = false;
			}
		}
		ModelPtr->Render->UpdateUV();
		return;
	}

	Render->UpdateUV();
}

void ObjectData::SetParamCase(float param) {

	ModelPtr->Render->ConfUniform->SetParamCase(param);
}

void ObjectData::SetVectorsParams(vec3 vectorsParams[10]) {

	ModelPtr->Render->ConfUniform->SetVectorsParams(vectorsParams);
}

void ObjectData::SetLight(vec3 posLight) {
	ModelPtr->Render->ConfUniform->SetPositionLight(posLight);
}


//==================================

void ObjectData::UpdateState() {
	
	if (MaterialData.Color == vec3(-1))
		MaterialData.Color = vec3(0.117, 0.351, 0.950);

	StartColor = MaterialData.Color;
}

void ObjectData::Refresh() {
	
	if (IndexObjectOwner != -1)
	{
		if (IndexObjectOwner == EngineData->SceneData->IndexBackgroundGUIObj)
			return;

		if (IsVisible != OwnerObj->IsVisible)
			IsVisible = OwnerObj->IsVisible;
	}

	if (IsTextBoxComponent)
	{
		TextBox->Refresh();
	}

	//CheckedRefresh();
}
//=========================================

Plane& ObjectData::GetPlaneFromVertIndex(int indexVertPlane) {
	
	return Shape->Planes[(int)indexVertPlane / 3];
}

//---- FIX.Plane
//shared_ptr<Plane> ObjectData::GetPlanePrt(int indexPlane) {
Plane& ObjectData::GetPlanePrt(int indexPlane) {

	return Shape->Planes[indexPlane];
}

//---- FIX.Plane
std::tuple<vec3, vec3, vec3> ObjectData::GetCase_V_W(int indexPlane) {
	return std::tuple<vec3, vec3, vec3>( 
		Shape->Planes[indexPlane].V0_W,
		Shape->Planes[indexPlane].V1_W,
		Shape->Planes[indexPlane].V2_W
	);
}


std::vector<glm::vec3> ObjectData::GetVertices() {
	return ModelPtr->MeshData.Vertices;
}

std::vector<glm::vec2> ObjectData::GetUV() {
	return ModelPtr->MeshData.UV;
}

std::vector<glm::vec3> ObjectData::GetNormals() {

	if (MeshData.Normals.size() != 0)
		return MeshData.Normals;
	else
		return ModelPtr->MeshData.Normals;
}

void ObjectData::MeshTransform() {

	if (IsTextBoxComponent)
	{
		TextBox->MeshTransform();
	}
}

void ObjectData::SelectedEvent() {
}

void ObjectData::UnselectedEvent() {
}

//===============================

ShapeHexagon* ObjectData::GetShapeHexagon() {

	ShapeHexagon* shapeCast = static_cast<ShapeHexagon*>(Shape);
	return shapeCast;
}

ShapeSquare* ObjectData::GetShapeSquare() {

	ShapeSquare* shapeCast = static_cast<ShapeSquare*>(Shape);
	return shapeCast;
}

//============================

void ObjectData::SetStartTimer() {
	StartTimer = glfwGetTime();
}

void ObjectData::ControlsEvents() {
}

bool ObjectData::GetVisible() {
	return IsVisible;
}

void ObjectData::DefaultView()
{
	MaterialData.Color = StartColor;

	//--- TEST
	ParamValue = 0;

	if (IsTextBoxComponent && TypeObj == TypeObject::EditBox)
	{
		TextBox->DefaultView();
	}
}

string ObjectData::GetInfo() {

	SceneSerialize* sceneSerialize = new SceneSerialize();
	string typeName = sceneSerialize->GetNameType(TypeObj);
	string layerName = sceneSerialize->GetNameType(Layer);

	std::stringstream ss;
	//ss << "N: " << Name << "  T: " + typeName << "  L: " << layerName;
	//ss << "T: " + typeName << "  L: " << layerName << "  N: " << Name;
	ss << "L: " << layerName << "  T: " + typeName << "  N: " << Name << " I:" << Index;
	return ss.str();
}

float ObjectData::GetTopLayer() {
	return Postranslate.y;
}

float ObjectData::GetZ() {
	return Postranslate.z;
}
void ObjectData::SetZ(float z)
{

}

int ObjectData::GetRightBorderVertexIndex() {

	if (IsTextBoxComponent)
	{
		return TextBox->GetRightBorderVertexIndex();
	}
	return -1;
}


void ObjectData::SetOwnerObject(shared_ptr<ObjectData> p_ownerObj)
//void ObjectData::SetOwnerObject(ObjectData* p_ownerObj)
{
	OwnerObj = p_ownerObj;
	IndexObjectOwner = p_ownerObj->Index;
}
void ObjectData::SetNextItemShellObject(shared_ptr<ObjectData> p_nextItemShellObj)
{
	NextItemShellObj = p_nextItemShellObj;
	NextItemShellIndex = p_nextItemShellObj->Index;
}
void ObjectData::SetShell(shared_ptr<BaseShell> p_shell)
{
	if (ShellIndex != -1)
	{
		SceneCommand->SourceIndex = p_shell->Index;
		return;
	}
	Shell = p_shell;
	ShellIndex = p_shell->Index;
}

//------ #SaveFieldSpecific

vector<ObjectFiledsSpecific> ObjectData::GetSpecificFiels() {

	if (IsTextBoxComponent)
	{
		return TextBox->GetSpecificFiels();
	}

	vector<ObjectFiledsSpecific> result;
	return result;
}

void ObjectData::SetSpecificFiels(vector<ObjectFiledsSpecific> filedsSpecific) {

	if (IsTextBoxComponent)
	{
		return TextBox->SetSpecificFiels(filedsSpecific);
	}
}

//Get Last By Specific Field Value
string ObjectData::GetSpecifValue(vector<ObjectFiledsSpecific> filedsSpecific, int index) {
	return filedsSpecific[filedsSpecific.size() - index].Value;
}

void ObjectData::SizeControlUpdate()
{
	GetShapeSquare()->SetSizeControl(vec3(SizePanel.x, SizePanel.y, 1));
}