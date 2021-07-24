//@@@
#include "CreatorModelData.h"
#include "WorldCollision.h"
#include "ModelData.h"
#include "ObjectsTypes/ObjectData.h"
#include "ObjectsTypes/ObjectPhysic.h"
#include "ObjectsTypes/ObjectDynamic.h"
#include "ObjectsTypes/ObjectPolygon.h"
#include "ObjectsTypes/ObjectHero.h"
#include "ObjectsTypes/ObjectNPC.h"
#include "ObjectsTypes/ObjectBullet.h"
#include "ObjectsTypes/ObjectCursorRay.h"
#include "ObjectsTypes/ObjectBlock.h"
#include "ObjectsTypes/ObjectGUI.h"
#include "ObjectsTypes/ObjectTextBox.h"
#include "ObjectsTypes/ObjectCursorGUI.h"
#include "ObjectsTypes/ObjectButton.h"
#include "ObjectsTypes/ObjectEditBox.h"
#include "ObjectsTypes/ObjectListBox.h"
#include "GeomertyShapes/ShapeBase.h"
#include "GeomertyShapes/ShapeSquare.h"
#include "Rooms/AspectDispatcherCommands.h"
#include "ShellObjects/BaseShell.h"
#include "SceneLayer.h"

#include "Serialize/SceneSerialize.h"
#include "Rooms/RoomSerializeScene.h"

#include "LoadBmp.h"
#include "ConfigBuffers.h"

#include "ConfigUniformArg.h"
#include "CoreSettings.h"
#include "OperationString.h"

#include <string>
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/random.hpp>

#include "WorldCollision.h"

#include <typeinfo>

using std::shared_ptr;

CreatorModelData::CreatorModelData() {
	
	Cam = new Camera();
	ConfigMVP = new CoreMVP();
	Oper = new Operator();
	Inputs = new ControllerInput;
	SceneData = new SceneParam;
	ShaderPrograms = map<string, GLuint>();
	LayerScene = new SceneLayer(this);
	Clusters = new WorldCluster();
}

CreatorModelData::~CreatorModelData() {
}

void CreatorModelData::ClearObjects() {

	SceneData->IndexCursorRayObj = -1;
	SceneData->IndexHeroObj = -1;
	SceneData->IndexBackgroundGUIObj = -1;
	SceneData->IndexCursorGUI = -1;
	SceneData->IndexBaseEditBox = -1;

	if(SceneObjects.size() != 0)
		SceneObjects.clear();
	//TEST^^
	/*if (SceneObjectsV.size() != 0)
		SceneObjectsV.clear();*/
	
	if (MapSceneObjects.size() != 0)
		MapSceneObjects.clear();
	//if (ObjectsShells.size() != 0)
	//	ObjectsShells.clear();

	LayerScene->Clear();
}

void CreatorModelData::AddModel(shared_ptr<ModelData> newModel, std::string name) {

	int index = Models.size();
	index--;
	newModel->Name = name;

	bool isGenName = name.length() == 0;
	if (!isGenName) {
		map <string, int> ::iterator it;
		it = MapModels.find(name);
		if (it != MapModels.end()) {
			//isGenName = true;
			name += "_" + std::to_string(index);
		}
	}

	if (isGenName) {
		string file = GetFile(newModel->PathModel3D);
		name += file + "_" + std::to_string(index);
	}

	MapModels.insert(std::pair<string, int>(name, index));
}

void CreatorModelData::ClearModels() {

}

void CreatorModelData::LoadModels(vector<shared_ptr<ModelFileds>> filedsModels) 
{
	if (filedsModels.size() == 0)
		return;

	ClearModels();

	int i = 0;

	for (auto fieldsModel : filedsModels) {

		std::shared_ptr<ModelData> nextModelPrt;

		string typeModel = fieldsModel->TypeName;
		if (typeModel == "ModelFrame") {
			auto modelFrame = ModelFrame();
			Models.push_back(std::make_unique<ModelFrame>(modelFrame));
			nextModelPrt = GetModelPrt(Models.size() - 1);
		}
		if (typeModel == "ModelTextBox") {
			auto modelTextBox = ModelTextBox();
			Models.push_back(std::make_unique<ModelTextBox>(modelTextBox));
			nextModelPrt = GetModelPrt(Models.size() - 1);
		}
		if (typeModel == "ModelEditBox") {
			auto modelEditBox = ModelEditBox();
			Models.push_back(std::make_unique<ModelEditBox>(modelEditBox));
			nextModelPrt = GetModelPrt(Models.size() - 1);
		}
		
		if(nextModelPrt == nullptr){
			ModelData nextModel = ModelData();
			Models.push_back(std::make_unique<ModelData>(nextModel));
			nextModelPrt = GetModelPrt(Models.size() - 1);
		}

		nextModelPrt->PathShaderVertex = fieldsModel->PathShaderVertex.c_str();
		nextModelPrt->PathShaderFrag = fieldsModel->PathShaderFrag.c_str();
		nextModelPrt->PathTexture = fieldsModel->PathTexture.c_str();
		nextModelPrt->PathModel3D = fieldsModel->PathModel3D.c_str();
		nextModelPrt->RadiusCollider = std::stof(fieldsModel->RadiusCollider);
		nextModelPrt->IsSquareModel = StrToBool(fieldsModel->IsSquareModel);
		nextModelPrt->Init(ShaderPrograms);

		AddModel(nextModelPrt, fieldsModel->Name);

		i++;
	}
}

std::shared_ptr<ModelData> CreatorModelData::GetModelPrt(int index)
{
	std::shared_ptr<ModelData> prt_model = Models[index];
	return prt_model;
}

std::shared_ptr<ModelData> CreatorModelData::GetModelPrt(string key)
{
	int index = MapModels[key];
	std::shared_ptr<ModelData> prt_model = Models[index];
	return prt_model;
}

bool CreatorModelData::IsExistObject(TypeObject typeObj)
{
	if ((typeObj == TypeObject::CursorRay ||
		 typeObj == TypeObject::Hero) // ||	typeObj == TypeObject::GUI
			&& IsExistObjectByType(typeObj))
				return true;
	return false;
}

std::shared_ptr<ObjectData> CreatorModelData::AddObject(
	string name, std::shared_ptr<ModelData> modelPtr, TypeObject p_typeObj, vec3 p_pos, vec3 p_color, int p_index, 
	TypeLayer p_layer, bool isLoading) {

	if (IsExistObject(p_typeObj))
		return NULL;

	SceneObjectsLastIndex = SceneObjects.size();
	//TEST^^
	//SceneObjectsLastIndex = SceneObjectsV.size();

	//if (p_index == -1)
		p_index = SceneObjectsLastIndex;

	bool isGenName = name.length() == 0;
	if (!isGenName) {
		map <string, int> ::iterator it;
		it = MapSceneObjects.find(name);
		if (it != MapSceneObjects.end()) {
			//isGenName = true;
			name += "_" + std::to_string(p_index);
		}
	}

	if (isGenName) {
		string fileModel = GetFile(modelPtr->PathModel3D);
		string fileTexture = GetFile(modelPtr->PathTexture);
		name += fileModel + "_" + std::to_string(p_index);
	}

	std::shared_ptr<ObjectData> object;
	switch (p_typeObj)
	{
		case GUI: {
			ObjectGUI obj = ObjectGUI(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectGUI>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case TextBox: {
			ObjectTextBox obj = ObjectTextBox(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectTextBox>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case EditBox: {
			ObjectEditBox obj = ObjectEditBox(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectEditBox>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case Button: {
			ObjectButton obj = ObjectButton(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectButton>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case ListBox: {
			ObjectListBox obj = ObjectListBox(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectListBox>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		case CursorGUI: {
			ObjectCursorGUI obj = ObjectCursorGUI(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectCursorGUI>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case Polygon: {
			ObjectPolygon obj = ObjectPolygon(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectPolygon>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			if(!CurrentPolygonObject)
				CurrentPolygonObject = object;
			break;
		}
		case Solid:
		case Terra: {
			ObjectPhysic obj = ObjectPhysic(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectPhysic>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case Block: {
			ObjectBlock obj = ObjectBlock(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectBlock>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case NPC:
		{
			ObjectNPC obj = ObjectNPC(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectNPC>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case Bullet:
		{
			ObjectDynamic obj = ObjectDynamic(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectDynamic>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case CursorRay:
		{
			ObjectCursorRay obj = ObjectCursorRay(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectCursorRay>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case BulletHero:
		{
			ObjectBullet obj = ObjectBullet(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectBullet>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		case Hero: {
			ObjectHero obj = ObjectHero(p_index, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectHero>(obj));
			//SceneObjectsV.push_back(obj); //TEST^^
			object = GetObjectPrt(obj.Index);
			break;
		}
		default:
			break;
	}

	if (object != NULL) {
		object->Clusters = Clusters;
		
		object->EngineData->Cam = Cam;
		object->EngineData->ConfigMVP = ConfigMVP;
		object->EngineData->Inputs = Inputs;
		object->EngineData->Oper = Oper;
		object->EngineData->SceneData = SceneData;

		object->Color = p_color;
		object->Name = name;
		if(p_layer != LayerNone)
			object->Layer = p_layer;
		object->InitData();
	}

	MapSceneObjects.insert(std::pair<string, int>(name, p_index));

	//--------------------------------- Save Order Index 
	/*if(isLoading)
		LayerScene->SaveOrderIndex(object);*/

	return object;
}

int CreatorModelData::GetIndexObjBySortInd(int indSort) 
{
	return LayerScene->SortObjectIndex[indSort];
}

int  CreatorModelData::SceneObjectsSize() {
	return SceneObjects.size();
	//TEST^^
	//return SceneObjectsV.size();
}

std::shared_ptr<ObjectData> CreatorModelData::GetObjectPrt(int index)
{
	std::shared_ptr<ObjectData> prt_objData = SceneObjects[index];
	//TEST^^
	//std::shared_ptr<ObjectData> prt_objData(&SceneObjectsV[index]);

	return prt_objData;
}

std::shared_ptr<ObjectData> CreatorModelData::GetObjectPrt(string key)
{
	int index = MapSceneObjects[key];
	std::shared_ptr<ObjectData> prt_objData = SceneObjects[index];
	//TEST^^
	//std::shared_ptr<ObjectData> prt_objData(&SceneObjectsV[index]);

	return prt_objData;
}

shared_ptr<BaseShell> CreatorModelData::AddShell(string name, int rootIndex, int captionIndex, bool isLoading,
	vector<int> items) {

	int nextIndex = ObjectsShells.size();
	
	//--- head Item
	int headIndex = -1;
	if (items.size() > 0)
		headIndex = items[0];

	BaseShell objShell(name, rootIndex);
	shared_ptr<BaseShell> prt_objShell = std::make_unique <BaseShell>(objShell);

	prt_objShell->CaptionObjIndex = captionIndex;
	prt_objShell->Index = nextIndex;
	prt_objShell->HeadIndexList = headIndex;
	

	if (!isLoading)
	{
		objShell.RootObj = GetObjectPrt(rootIndex);
		if (captionIndex != -1)
			objShell.CaptionObj = GetObjectPrt(captionIndex);
		if (headIndex != -1)
			objShell.HeadObj = GetObjectPrt(headIndex);
	}

	if(!isLoading)
	{
		GetObjectPrt(rootIndex)->SetShell(prt_objShell);
		if (captionIndex != -1)
			GetObjectPrt(captionIndex)->SetShell(prt_objShell);
		for (auto item : items)
		{
			GetObjectPrt(item)->SetShell(prt_objShell);
		}
	}
	ObjectsShells.push_back(prt_objShell);

	return prt_objShell;
}

void CreatorModelData::FillShellComponents(){
	int indNext = 0;
	while (indNext <= SceneObjectsLastIndex)
	{
		FillShellComponents(indNext++);
	}
}

void CreatorModelData::FillShellComponents(int indexObj) {

	int indShell, indComp;
	bool isEndList = false;
	shared_ptr<BaseShell> shell;
	shared_ptr<ObjectData> itemNextObj;

	itemNextObj = GetObjectPrt(indexObj);
	indShell = itemNextObj->ShellIndex;
	if (indShell == -1)
		return;
	shell = GetObjectShellPrt(indShell);
	if (shell->HeadIndexList == -1)
		return;
	indComp = shell->HeadIndexList;
	shell->Components.clear();
	while (!isEndList)
	{
		itemNextObj = GetObjectPrt(indComp);
		shell->AddComponent(itemNextObj->Name, itemNextObj->Index);
		indComp = itemNextObj->NextItemShellIndex;
		isEndList = indComp == -1;
	}
}


shared_ptr<BaseShell> CreatorModelData::GetObjectShellPrt(int index)
{
	shared_ptr<BaseShell> prt_objShell = ObjectsShells[index];
	return prt_objShell;
}

bool CreatorModelData::IsExistObjectByName(string key)
{
	map <string, int> ::iterator it;
	it = MapSceneObjects.find(key);
	return it != MapSceneObjects.end();
}

bool CreatorModelData::IsExistObjectByType(TypeObject type)
{
	for (auto item : SceneObjects) {
		if(item->TypeObj == type)
			return true;
	}
	//TEST^^
	/*for (auto item : SceneObjectsV) {
		if (item.TypeObj == type)
			return true;
	}*/

	return false;
}


void CreatorModelData::ObjectAction(int index)
{
	std::shared_ptr<ObjectData> prt_objData = SceneObjects[index];
	prt_objData->Action();
	//TEST^^
	/*ObjectData objData = SceneObjectsV[index];
	objData.Action();*/
}

void CreatorModelData::SaveObject(ObjectData* objData)
{
	SceneObjects[objData->Index] = std::make_unique<ObjectData>(*objData);
	//TEST^^
	//SceneObjectsV[objData->Index] = *objData;
}


//#SaveFieldSpecific
void CreatorModelData::LoadObjects(vector<shared_ptr<ObjectFileds>> objectsData, vector<vector<ObjectFiledsSpecific>> objectsDataSpecific) {

	if (objectsData.size() == 0)
		return;

	ClearObjects();

	SceneSerialize* serializer = new SceneSerialize();
	vector<ObjectFiledsSpecific> specificFields;
	int i = 0;

	for (auto objFields : objectsData) {

		shared_ptr<ModelData> model = GetModelPrt(objFields->Model);
		TypeObject typeObj = serializer->GetTypeObject(objFields->Type);
		TypeLayer layer = serializer->GetTypeLayer(objFields->Layer);

		int indexObj = std::stoi(objFields->Index);

		auto newObj = AddObject(objFields->Name, model, typeObj, objFields->PostranslateValue,vec3(0), indexObj, layer, true);
	
		if (newObj == NULL)
			continue;

		newObj->Target = objFields->TargetValue;
		newObj->Layer = layer;

		ActionObject typeAction = serializer->GetTypeAction(objFields->ActionObjectCurrent);
		newObj->ActionObjectCurrent = typeAction;

		newObj->IndexObjectOwner = stoi(objFields->IndexObjectOwner);
		newObj->ShellIndex = stoi(objFields->ShellIndex);
		newObj->NextItemShellIndex = stoi(objFields->NextItemShellIndex);

		newObj->Color = objFields->ColorValue;

		TypeCommand typeCommand = serializer->GetTypeCommands(objFields->Command);
		int sourceIndex = stoi(objFields->CommandSourceIndex);
		int targetIndex = stoi(objFields->CommandTargetIndex);
		int valueI = stoi(objFields->CommandValueI);
		float valueF = stof(objFields->CommandValueF);
		vec4 valueV4 = vec4();
		string valueS = serializer->GetStrValue(objFields->CommandValueS);
		string description = serializer->GetStrValue(objFields->CommandDescription);
		
		if (typeCommand != TypeCommand::None) {
			//SetCommand(newObj, typeCommand);
			SetCommand(newObj, typeCommand, 
				sourceIndex, targetIndex, 
				description, valueI,  
				valueI, valueF, valueV4, valueS, 
				description);
		}

		OptionsObject opt = objFields->Options;
		newObj->IsVisible = StrToBool(opt.IsVisible);
		newObj->IsGravity = StrToBool(opt.IsGravity);
		newObj->IsGUI = StrToBool(opt.IsGUI);
		newObj->IsTextureRepeat = StrToBool(opt.IsTextureRepeat);
		newObj->IsNPC = StrToBool(opt.IsNPC);
		newObj->IsHexagonModel = StrToBool(opt.IsHexagonModel);
		newObj->IsSquareModel = StrToBool(opt.IsSquareModel);
		newObj->IsAbsolutePosition = StrToBool(opt.IsAbsolutePosition);
		newObj->IsFocusable = StrToBool(opt.IsFocusable);
		newObj->IsTransformable = StrToBool(opt.IsTransformable);
		newObj->IsUsable = StrToBool(opt.IsUsable);
		newObj->IsChecked = StrToBool(opt.IsChecked);

		//#SaveFieldSpecific
		specificFields = objectsDataSpecific[i];
		newObj->SetSpecificFiels(specificFields);

		//--- After init
		newObj->UpdateState();
		i++;
	}

	UpdateObjectsOrders();

	UpdateFamilBonds();
}

void CreatorModelData::UpdateFamilBonds() {

	shared_ptr<ObjectData> objOther;

	for (auto obj : SceneObjects) {
		if (obj->IndexObjectOwner != -1) {
			objOther = GetObjectPrt(obj->IndexObjectOwner);
			obj->SetOwnerObject(objOther);
		}
		if (obj->NextItemShellIndex != -1) {
			objOther = GetObjectPrt(obj->NextItemShellIndex);
			obj->SetNextItemShellObject(objOther);
		}
		if (obj->ShellIndex != -1) {
			auto shell = ObjectsShells[obj->ShellIndex];
			obj->SetShell(shell);
		}
	}
}

void CreatorModelData::LoadModels() {

	SceneSerialize* serializer = new SceneSerialize();
	serializer->Load();
	LoadModels(serializer->FiledsModels);

	if (Models.size() > 0)
		return;

	std::shared_ptr<ModelData> nextModelPrt;

	ModelData nextModel = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModel));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathTexture = "./Textures/testTexture.bmp";
	nextModelPrt->PathModel3D = "./Models3D/monkey.obj";
	nextModelPrt->RadiusCollider = 1;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "mon");

	ModelData nextModel2 = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModel2));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathTexture = "./Textures/planet.bmp";
	nextModelPrt->PathModel3D = "./Models3D/polygonPlane.obj";
	nextModelPrt->RadiusCollider = 0.2;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "plane");

	ModelData nextModel3 = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModel3));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/kub.obj";
	nextModelPrt->PathTexture = "./Textures/testTexture.bmp";
	nextModelPrt->RadiusCollider = 1;
	nextModelPrt->IsDebug = true;
	nextModelPrt->IsSquareModel = true;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "box");

	ModelData nextModel4 = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModel4));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/Marker_Vector.obj";
	nextModelPrt->PathTexture = "./Textures/testTexture.bmp";
	nextModelPrt->RadiusCollider = 0.1;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "marker_Vector");

	ModelData nextModel5 = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModel5));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/Marker_Cross.obj";
	nextModelPrt->PathTexture = "./Textures/testTexture.bmp";
	nextModelPrt->RadiusCollider = 0.1;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "marker_Cross");

	ModelData nextModel6 = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModel6));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/Marker_Point.obj";
	nextModelPrt->PathTexture = "./Textures/testTexture.bmp";
	nextModelPrt->RadiusCollider = 0.1;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "marker_Point");

	ModelData nextModelHomo = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModelHomo));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/Marker_Point.obj";
	nextModelPrt->PathTexture = "./Textures/future.bmp";
	nextModelPrt->RadiusCollider = 1;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "homo");

	ModelData nextModelCursorRay = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModelCursorRay));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/Marker_Cross.obj";
	nextModelPrt->PathTexture = "./Textures/future.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "cursorRay");

	//---GUI -- control -- Background frame
	ModelFrame contextFrame = ModelFrame();
	Models.push_back(std::make_unique<ModelFrame>(contextFrame));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "FrameUI.vert";
	//nextModelPrt->PathShaderFrag = "FrameUI.frag";
	nextModelPrt->PathShaderFrag = "ContextUI.frag";
	nextModelPrt->PathModel3D = "./Models3D/Frame.obj";
	//nextModelPrt->PathTexture = "./Textures/future.bmp";
	//nextModelPrt->PathTexture = "./Textures/Frame.bmp";
	nextModelPrt->PathTexture = "./Textures/testTexture2.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->IsSquareModel = true;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "ConextFrameModel");

	//---GUI -- control -- Frame
	//ModelFrame modelFrame = ModelFrame();
	ModelFrame modelFrame = ModelFrame();
	Models.push_back(std::make_unique<ModelFrame>(modelFrame));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "FrameUI.vert";
	nextModelPrt->PathShaderFrag = "FrameUI.frag";
	nextModelPrt->PathModel3D = "./Models3D/Frame.obj";
	//nextModelPrt->PathTexture = "./Textures/syzanna.bmp";
	//nextModelPrt->PathTexture = "./Textures/testTexture3.bmp";
	//nextModelPrt->PathTexture = "./Textures/testTexture.bmp";
	nextModelPrt->PathTexture = "./Textures/InventCase2.bmp";
	//nextModelPrt->PathTexture = "./Textures/testTexture2.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->IsSquareModel = true;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "FrameModel");


	//------ Button EditBox Model --------------------------
	modelFrame = ModelFrame();
	Models.push_back(std::make_unique<ModelFrame>(modelFrame));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "FrameUI.vert";
	nextModelPrt->PathShaderFrag = "FrameUI.frag";
	nextModelPrt->PathModel3D = "./Models3D/Frame.obj";
	nextModelPrt->PathTexture = "./Textures/EditBox2.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->IsSquareModel = true;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "ButtonEditBoxModel");
	

	//------ Button Model --------------------------
	modelFrame = ModelFrame();
	Models.push_back(std::make_unique<ModelFrame>(modelFrame));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "FrameUI.vert";
	nextModelPrt->PathShaderFrag = "FrameUI.frag";
	nextModelPrt->PathModel3D = "./Models3D/Frame.obj";
	//nextModelPrt->PathTexture = "./Textures/syzanna.bmp";
	//nextModelPrt->PathTexture = "./Textures/testTexture3.bmp";
	//nextModelPrt->PathTexture = "./Textures/testTexture.bmp";
	nextModelPrt->PathTexture = "./Textures/Button.bmp";
	//nextModelPrt->PathTexture = "./Textures/testTexture2.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->IsSquareModel = true;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "ButtonModel");

	//---GUI -- control -- TextBox
	ModelTextBox TextBox = ModelTextBox();
	Models.push_back(std::make_unique<ModelTextBox>(TextBox));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "TextBoxUI.vert";
	nextModelPrt->PathShaderFrag = "TextBoxUI.frag";
	nextModelPrt->PathModel3D = "./Models3D/TextBox.obj";
	nextModelPrt->PathTexture = "./Textures/Alphabet.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->IsSquareModel = true;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "TextBoxModel");

	//---GUI -- control -- Cursor
	ModelFrame curcorModel = ModelFrame();
	Models.push_back(std::make_unique<ModelFrame>(curcorModel));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "FrameUI.vert";
	//nextModelPrt->PathShaderFrag = "FrameUI.frag";
	nextModelPrt->PathShaderFrag = "CursorUI.frag";
	nextModelPrt->PathModel3D = "./Models3D/Cursor.obj";
	//nextModelPrt->PathTexture = "./Textures/Cursor.bmp";
	nextModelPrt->PathTexture = "./Textures/CursorB.bmp";
	//nextModelPrt->PathTexture = "./Textures/CursorSDF.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->IsSquareModel = true;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "CursorModel");

	//---GUI -- control -- EditBox
	ModelEditBox edittBox = ModelEditBox();
	Models.push_back(std::make_unique<ModelEditBox>(edittBox));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "TextBoxUI.vert";
	nextModelPrt->PathShaderFrag = "TextBoxUI.frag";
	nextModelPrt->PathModel3D = "./Models3D/TextBox.obj";
	nextModelPrt->PathTexture = "./Textures/Alphabet.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->IsSquareModel = true;
	nextModelPrt->Init(ShaderPrograms);
	AddModel(nextModelPrt, "EditBoxModel");

}

void CreatorModelData::LoadObjectsGUI() {

	// ---- Object frame
	string caption;
	string childModel = "conextGUI_2";
	//childModel = "conextGUI_T";
	string objName;
	vec3 color = vec3(0.3);
	shared_ptr<ObjectButton> objCreateButton;// 
	shared_ptr<ObjectData> objCreate;

	// ---- Object Context frame GUI	(SYSTEM CONTROL)
	shared_ptr<ModelData> modelGUI = GetModelPrt("ConextFrameModel");
	shared_ptr<ObjectData> objBackGUI_Data = 
		AddObject("BackContectGUI", modelGUI, Button, vec3(0, -50, 0), vec3(1), -1 , LayerBackground);
	//shared_ptr<ObjectGUI> objBackGUI = std::dynamic_pointer_cast<ObjectGUI>(objBackGUI_Data);
	shared_ptr<ObjectButton> objBackGUI = std::dynamic_pointer_cast<ObjectButton>(objBackGUI_Data);
	objBackGUI->IsToogleButon = false;
	SetCommand(objBackGUI, TypeCommand::None);
	objBackGUI->IsSelected = false;
	objBackGUI->IsTransformable = false;

	// ---- Object frame	
	objName = "TEST3";
	caption = objBackGUI->Name + "." + objName;
	childModel = "FrameModel";
	//objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.4, .01, 0.02), vec2(1.1, 0.2), GUI, vec3(1));
	//AddChildObject(objBackGUI, caption, childModel, objName, vec3(.4, .01, 0.002), vec2(1.1, 0.2), GUI, vec3(1));
	AddChildObject(objBackGUI, caption, childModel, objName, vec3(.4, .01, StartPosGUI_Z), vec2(1.1, 0.2), GUI, vec3(1));
	

	// ---- Object Button edit obj GUI (BASE CONTROL)
	objName = "ButtonEditOn";
	//caption = objBackGUI->Name + "." + objName;
	caption = "редакт";
	childModel = "ButtonModel";
	//objCreate = objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.05, .05, 0.03), vec2(0.1, 0.1), Button, vec3(1));
	objCreate = 
		AddChildObject(objBackGUI, caption, childModel, objName, vec3(.05, .05, StartPosGUI_Z), vec2(0.1, 0.1), Button, vec3(1), LayerBase);
	objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);
	objCreateButton->IsToogleButon = true;
	objCreateButton->IsTransformable = false;
	SetCommand(objCreateButton, EditGUI_OnOff);
	ControlConstruct(objCreateButton, caption, Button);


	// ---- Object Button create obj GUI
	objName = "ButtonCreateObjGUI";
	caption = "создать";//  objBackGUI->Name + "." + objName;
	//caption = "абвгдежз";
	childModel = "ButtonModel";
	//objCreate = objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.15, .05, 0.02), vec2(0.3, 0.2), Button, vec3(1));
	objCreate = AddChildObject(objBackGUI, caption, childModel, objName, vec3(.15, .05, StartPosGUI_Z), vec2(0.3, 0.2), Button, vec3(1));
	objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);
	objCreateButton->IsToogleButon = false;
	SetCommand(objCreateButton, SelectPosForObject);
	ControlConstruct(objCreateButton, caption, Button);
	
	// ---- Object text box GUI
	objName = "TextBoxObject";
	caption = "привет мир и доброе утро";
	//caption = "абвгдежзиклмн";
	childModel = "TextBoxModel";
	color = vec3(0.117, 0.351, 0.950);
	//AddChildObject(objBackGUI, caption, childModel, objName, vec3(.5, .5, 0.031), vec2(1.5, 1.), TextBox, color);
	AddChildObject(objBackGUI, caption, childModel, objName, vec3(.5, .5, StartPosGUI_Z), vec2(1.5, 1.), TextBox, color);

	// ---- Object Edit Box	(SYSTEM CONTROL)
	//objName = "Base_EditBox_NameObject";
	objName = SceneData->NameSystemEditBox;
	caption = "ох";
	childModel = "ButtonEditBoxModel";	///!!!!!
	color = vec3(0.117, 0.351, 0.950);
	objCreate = AddChildObject(objBackGUI, caption, childModel, objName, vec3(.1, .3, StartPosSystemGUI_Z), vec2(.7, .1), Button, color, LayerBase);
	//SceneData->IndexBaseEditBox = objCreate->Index; //(SYSTEM CONTROL)
	objCreateButton = std::dynamic_pointer_cast<ObjectButton>(objCreate);
	ControlConstruct(objCreateButton, caption, EditBox, "SystemEditBox", LayerBase);
	objCreateButton->IsToogleButon = true;
	objCreateButton->IsFocusable = false;
	objCreateButton->IsTransformable = false;
	objCreateButton->IsVisible = false;

	// ---- Object Cursor GUI (Last is alpha background fix)
	objName = "CursorGUI";
	caption = objBackGUI->Name + "." + objName;
	childModel = "CursorModel";
	AddChildObject(objBackGUI, caption, childModel, objName, vec3(.15, .15, .01), vec2(.05, .05), CursorGUI, vec3(0.2, 0.5, 0.1), LayerSystem);
}


void CreatorModelData::LoadShells(vector<shared_ptr<ShellFileds>> filedsShells)  {

	if (filedsShells.size() == 0)
		return;

	ObjectsShells.clear();

	for (auto shellFiled : filedsShells)
	{
		AddShell(shellFiled->Name, stoi(shellFiled->RootObjIndex), stoi(shellFiled->CaptionObjIndex), true);
	}

}

void CreatorModelData::LoadObjects() {

	SceneSerialize* serializer = new SceneSerialize();
	serializer->Load();
	LoadObjects(serializer->FiledsObjects, serializer->FiledsObjectsSpecific);

	if (SceneObjectsSize() > 0)
		return;

	float offsetCentrePlane = 500;
	float radiusPlane = 70;

	std::shared_ptr<ModelData> modelPolygon = GetModelPrt("plane");
	AddObject("Plane", modelPolygon, Polygon, vec3(-20.f, -55, radiusPlane));
	//CurrentPolygonObject = GetObjectPrt("Plane");

	std::shared_ptr<ModelData> modelMon = GetModelPrt("mon");

    for (int i = 0; i < 2; i++)
	{
		AddObject("Mon", modelMon, NPC);
	}

	vec3 color_red = vec3(1, 0, 0);
	vec3 color_blue = vec3(0, 0, 1);
	vec3 color_green = vec3(0, 1, 0);
	vec3 color_violet = vec3(1, 0, 1);
	vec3 color_yelow = vec3(0, 1, 1);

	// ------- Wall

		std::shared_ptr<ModelData> modelBox = GetModelPrt("box");
		AddObject("Box", modelBox, Solid, vec3(0, -35, 0));
		AddObject("Box2", modelBox, Solid, vec3(0, -25, 0));
		AddObject("Box3", modelBox, Solid, vec3(-50, -55, 70));

		//TEST gen Box
		/*for (int i = 0; i < 200; i++)
		{
			int step = 10;
			int z = floor(i / step) * step;
			int x = (i * step) - (z * step);

			if(glm::mod((float)i, (float)step)<0.00001)
				AddObject("BoxN", modelBox, Solid, vec3(30 - x, -45, 50 + z));
			AddObject("BoxN", modelBox, Block, vec3(0 - x, -5, 50 + z));
		}*/

		AddObject("BlockBox", modelBox, Block, vec3(-10, -50, -10));
		AddObject("BlockBox1", modelBox, Block, vec3(-20, -50, -10));
		AddObject("BlockBox2", modelBox, Block, vec3(-10, -50, -20));
		AddObject("BlockBox3", modelBox, Block, vec3(-20, -50, -20));

	std::shared_ptr<ModelData> modelM_P = GetModelPrt("marker_Point");
	AddObject("M_P_1", modelM_P, Solid, vec3(-50, 0, 0));
	AddObject("M_P_2", modelM_P, Solid, vec3(-50, 0, 0));

	AddObject("Bullet", modelM_P, Bullet, vec3(0, 0, 0));
	AddObject("BulletHero", modelM_P, BulletHero, vec3(0, 0, 0));
	//AddObject(this, "Bullet", modelBox, Bullet, vec3(0, 0, 0));


	std::shared_ptr<ModelData> modelM_C = GetModelPrt("marker_Cross");
	AddObject("M_C_1", modelM_C, Solid, vec3(0, 0, -50));
	AddObject("M_C_2", modelM_C, Solid, vec3(0, 0, -50));

	std::shared_ptr<ModelData> modelM_V = GetModelPrt("marker_Vector");
	AddObject("M_V_1", modelM_V, Solid, vec3(0, 0, 50), color_green);
	AddObject("M_V_2", modelM_V, Solid, vec3(0, 0, 50), color_blue);
	AddObject("M_V_3", modelM_V, Solid, vec3(0, 0, 50), color_yelow);

	std::shared_ptr<ModelData> modeHero = GetModelPrt("homo");
	shared_ptr<ObjectData> objHeroData = AddObject("Hero", modeHero, Hero, vec3(0, 0, 0));
	shared_ptr<ObjectHero> objHero = std::dynamic_pointer_cast<ObjectHero>(objHeroData);
	//objHero->LoadCursorRay();
	std::shared_ptr<ModelData> model = GetModelPrt("cursorRay");
	auto obj = AddObject("CursorRay", model, CursorRay, vec3(-50, -55, 70));
	obj->SetOwnerObject(objHero);


	LoadObjectsGUI();

	/* for (int x = 0; x < 5; x++)
	{
		for (int z = 0; z < 5; z++)
		{
			int o_x = -offsetCentrePlane + (radiusPlane * x);
			int o_z = -offsetCentrePlane + (radiusPlane * z);
			std::shared_ptr<ModelData> model1 = GetModelPrt("plane");
			AddObject("Plane", model1, Polygon, vec3(o_x, -55, o_z));
		}
	}*/

	UpdateObjectsOrders();
}

void CreatorModelData::UpdateObjectsOrders() {

	LayerScene->Clear();

	//Sort ordering
	for (auto obj : SceneObjects) {

		if (!LayerScene->IsNeedSort(obj->Index))
			continue;

		LayerScene->SaveOrderIndex(obj);
	}
}

void CreatorModelData::Load() {

	LoadModels();
	LoadShells();
	LoadObjects();
	LoadShellsLinks();
	LoadClusters();
}

void CreatorModelData::LoadShells() {

	SceneSerialize* serializer = new SceneSerialize();
	serializer->Load();
	LoadShells(serializer->FiledsShells);
}

void  CreatorModelData::LoadShellsLinks() {

	for (auto shape : ObjectsShells) {
		shape->RootObj = GetObjectPrt(shape->RootObjIndex);
		if (shape->CaptionObjIndex != -1)
			shape->CaptionObj = GetObjectPrt(shape->CaptionObjIndex);
		if (shape->HeadIndexList != -1)
			shape->HeadObj = GetObjectPrt(shape->HeadIndexList);
	}
	for (auto obj : SceneObjects) {

		if (obj->ShellIndex != -1)
			obj->Shell = GetObjectShellPrt(obj->ShellIndex);
	}
}


void CreatorModelData::LoadClusters() {

	Clusters->Sectors = new WorldSectors();
	Clusters->Storage = this;
	Clusters->PlaneClusterization();
}


shared_ptr<ObjectData> CreatorModelData::AddChildObject(shared_ptr<ObjectData> ownerObjData, string caption, string nameModel, string nameObject, vec3 startPosChild, vec2 size, TypeObject p_typeObj, vec3 color, TypeLayer p_layer)
{
	shared_ptr<ObjectGUI> ownerObj = std::dynamic_pointer_cast<ObjectGUI>(ownerObjData);

	if (color == vec3(0))
		color = vec3(1);

	std::shared_ptr<ModelData> model = GetModelPrt(nameModel);
	auto  modelTextBox = std::dynamic_pointer_cast<ModelTextBox>(model);
	if (modelTextBox != nullptr) {
		model = modelTextBox;
	}
	auto modelFrame = std::dynamic_pointer_cast<ModelFrame>(model);
	if (modelFrame != nullptr) {
		model = modelFrame;
	}


	//-------------- Z Order Controls ---------------
	vec3 startPos = startPosChild;

	startPos.z = StartPosGUI_Z;
	if (ownerObj->Index != SceneData->IndexBackgroundGUIObj)
		startPos.z = StartPosGUI_Z + StartPosSubGUI_ADD_Z;

	if (p_layer == LayerBase) {
		if (ownerObj->Index != SceneData->IndexBackgroundGUIObj)
			startPos.z = StartPosSystemGUI_Z + StartPosSubGUI_ADD_Z;
		else
			startPos.z = StartPosSystemGUI_Z;
	}
	if (p_layer == LayerSystem) {
		startPos.z = .01;
	}
	//-------------------------------------------------
	
	//shared_ptr<ObjectData> obj = AddObject(nameObject, model, p_typeObj, ownerObj->StartPos, color, -1, p_layer);
	shared_ptr<ObjectData> obj = AddObject(nameObject, model, p_typeObj, startPos, color, -1, p_layer);

	auto objGUI = std::dynamic_pointer_cast<ObjectGUI>(obj);
	objGUI->SetOwnerObject(ownerObj);
	//objGUI->StartPos = startPosChild;
	objGUI->StartPos = startPos;
	objGUI->SizePanel = size;
	objGUI->Color = color;
	objGUI->UpdateState();
	//objGUI->IsTextureRepeat = true;

	objGUI->GetShapeSquare()->SetSizeControl(vec3(size.x, size.y, 1));

	auto objTextBox = std::dynamic_pointer_cast<ObjectTextBox>(obj);
	if (objTextBox != nullptr) {
		objTextBox->Message = caption;
		objTextBox->CreateMessage();
	}

	return objGUI;
}

shared_ptr<ObjectData>  CreatorModelData::ControlConstruct(shared_ptr<ObjectData> obj, string caption, TypeObject p_typeObj, string name, TypeLayer p_layer)
{
	shared_ptr<ObjectData> objData;
	auto objButton = std::dynamic_pointer_cast<ObjectButton>(obj);
	if (p_typeObj == TypeObject::Button) {
		if (objButton != nullptr) {
			if (!objButton->IsToogleButon) {
				//--- Add textbox
				vec2 offset = vec2(0.01);
				vec3 startPos = vec3(offset.x, offset.y, 0.021);
				startPos = vec3(.01, .01, 0.002);
				
				//startPos.z = objButton->StartPos.z + 0.0005;
				/*startPos.z = StartPosGUI_Z + StartPosSubGUI_ADD_Z;*/
				/*if(p_layer == LayerSystem)
					startPos.z = StartPosSystemGUI_Z + StartPosSubGUI_ADD_Z;*/

				if (name.size() == 0)
					name = "Button_TextBox";
				objData = AddChildObject(objButton, caption, "TextBoxModel", name, startPos, vec2(1.5, 1.), TextBox, vec3(-1), p_layer);
				auto objTextButton = std::dynamic_pointer_cast<ObjectTextBox>(objData);

				AddShell("ButtonShell_" + objButton->Name,
					objButton->Index,
					objTextButton->Index);

				//LayerScene->SaveOrderIndex(objTextButton);
				return objTextButton;
			}
		}
	}

	if (p_typeObj == TypeObject::EditBox) {
		if (objButton != nullptr) {
		
			// ---- Object Edit box create	
			vec2 offset = vec2(0.01);
			vec3 startPos = vec3(offset.x, offset.y, 0.002);
			startPos = vec3(.03, .03, 0.021);

			//startPos.z = objButton->StartPos.z + 0.0005;
			/*startPos.z = StartPosGUI_Z + StartPosSubGUI_ADD_Z;
			if (p_layer == LayerSystem)
				startPos.z = StartPosSystemGUI_Z + StartPosSubGUI_ADD_Z;*/

			if (name.size() == 0)
				name = "Button_EditBox";
			auto objCreate = AddChildObject(objButton, caption, "EditBoxModel", name, startPos, vec2(1.5, 1.), EditBox, vec3(-1), p_layer);
			auto editBoxObj = std::dynamic_pointer_cast<ObjectEditBox>(objCreate);

			SetCommand(objButton, TypeCommand::EditObjectCommand, objButton->Index, editBoxObj->Index);

			AddShell("EditBoxShell_" + objButton->Name,
				objButton->Index, 
				editBoxObj->Index);

			//LayerScene->SaveOrderIndex(editBoxObj);

			return editBoxObj;
		}
	}
	return nullptr;
}
