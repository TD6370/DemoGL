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
#include "ObjectsTypes/ObjectTextBlock.h"
#include "ObjectsTypes/ObjectCursorGUI.h"
#include "GeomertyShapes//ShapeBase.h" //###

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
}

CreatorModelData::~CreatorModelData() {
}

void CreatorModelData::ClearObjects() {

	if(SceneObjects.size() != 0)
		SceneObjects.clear();
	if (MapSceneObjects.size() != 0)
		MapSceneObjects.clear();
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
		if (typeModel == "ModelTextBlock") {
			auto modelTextBlock = ModelTextBlock();
			Models.push_back(std::make_unique<ModelTextBlock>(modelTextBlock));
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
		nextModelPrt->IsCubeModel = StrToBool(fieldsModel->IsCubeModel);
		nextModelPrt->Init();

		AddModel(nextModelPrt, fieldsModel->Name);

		i++;
	}

	//-------------------------------

	//for (auto fieldsModel : filedsModels) {

	//	ModelData nextModel;

	//	string typeModel = fieldsModel->TypeName;
	//	if (typeModel == "ModelFrame") {
	//		nextModel = ModelFrame();
	//	}
	//	if (typeModel == "ModelTextBlock") {
	//		nextModel = ModelTextBlock();
	//	}

	//	//ModelData nextModel = ModelData();
	//	nextModel.PathShaderVertex = fieldsModel->PathShaderVertex.c_str();
	//	nextModel.PathShaderFrag = fieldsModel->PathShaderFrag.c_str();
	//	nextModel.PathTexture = fieldsModel->PathTexture.c_str();
	//	nextModel.PathModel3D = fieldsModel->PathModel3D.c_str();
	//	nextModel.RadiusCollider = std::stof(fieldsModel->RadiusCollider);
	//	nextModel.IsCubeModel = StrToBool(fieldsModel->IsCubeModel);
	//	nextModel.Init();
	//	AddModel(&nextModel, fieldsModel->Name);

	//	i++;
	//}
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

//std::shared_ptr<ObjectData> CreatorModelData::AddObject(CreatorModelData* storage, string name, std::shared_ptr<ModelData> modelPtr, TypeObject p_typeObj, vec3 p_pos, vec3 p_color) {
std::shared_ptr<ObjectData> CreatorModelData::AddObject(string name, std::shared_ptr<ModelData> modelPtr, TypeObject p_typeObj, vec3 p_pos, vec3 p_color) {

	if (IsExistObject(p_typeObj))
		return NULL;

	SceneObjectsLastIndex = SceneObjects.size();

	bool isGenName = name.length() == 0;
	if (!isGenName) {
		map <string, int> ::iterator it;
		it = MapSceneObjects.find(name);
		if (it != MapSceneObjects.end()) {
			//isGenName = true;
			name += "_" + std::to_string(SceneObjectsLastIndex);
		}
	}

	if (isGenName) {
		string fileModel = GetFile(modelPtr->PathModel3D);
		string fileTexture = GetFile(modelPtr->PathTexture);
		name += fileModel + "_" + std::to_string(SceneObjectsLastIndex);
	}

	std::shared_ptr<ObjectData> object;
	switch (p_typeObj)
	{
		case GUI: {
			ObjectGUI obj = ObjectGUI(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectGUI>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		case TextBlock: {
			ObjectTextBlock obj = ObjectTextBlock(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectTextBlock>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		case CursorGUI: {
			ObjectCursorGUI obj = ObjectCursorGUI(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectCursorGUI>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		case Polygon: {
			ObjectPolygon obj = ObjectPolygon(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectPolygon>(obj));
			object = GetObjectPrt(obj.Index);
			if(!CurrentPolygonObject)
				CurrentPolygonObject = object;
			break;
		}
		case Solid:
		case Terra: {
			ObjectPhysic obj = ObjectPhysic(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectPhysic>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		case Block: {
			ObjectBlock obj = ObjectBlock(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectBlock>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		case NPC:
		{
			ObjectNPC obj = ObjectNPC(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectNPC>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		case Bullet:
		{
			ObjectDynamic obj = ObjectDynamic(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectDynamic>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		case CursorRay:
		{
			ObjectCursorRay obj = ObjectCursorRay(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectCursorRay>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		case BulletHero:
		{
			ObjectBullet obj = ObjectBullet(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectBullet>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		case Hero: {
			ObjectHero obj = ObjectHero(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
			SceneObjects.push_back(std::make_unique<ObjectHero>(obj));
			object = GetObjectPrt(obj.Index);
			break;
		}
		default:
			break;
	}

	if (object != NULL) {
		//objectModel->Storage = storage;
		object->Storage = this;
		object->Color = p_color;
		object->Name = name;
		object->InitData();
	}

	MapSceneObjects.insert(std::pair<string, int>(name, SceneObjectsLastIndex));
	return object;
}

std::shared_ptr<ObjectData> CreatorModelData::GetObjectPrt(int index)
{
	std::shared_ptr<ObjectData> prt_objData = SceneObjects[index];
	return prt_objData;
}

std::shared_ptr<ObjectData> CreatorModelData::GetObjectPrt(string key)
{
	int index = MapSceneObjects[key];
	std::shared_ptr<ObjectData> prt_objData = SceneObjects[index];
	return prt_objData;
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
	return false;
}


void CreatorModelData::GenerateObjects()
{
	for (int i = 0; i < LimitSceneObjects; i++)
	{
		ObjectData objectModel = ObjectData(i, GetModelPrt(0));
		SceneObjectsLastIndex++;
		SceneObjects.push_back(std::make_unique<ObjectData>(objectModel));
	}
}

void CreatorModelData::ObjectAction(int index)
{

	std::shared_ptr<ObjectData> prt_objData = SceneObjects[index];
	prt_objData->Action();
}

void CreatorModelData::SaveObject(ObjectData* objData)
{
	SceneObjects[objData->Index] = std::make_unique<ObjectData>(*objData);
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
	
		auto newObj = AddObject(objFields->Name, model, typeObj, objFields->PostranslateValue);
	
		if (newObj == NULL)
			continue;

		newObj->Target = objFields->TargetValue;

		ActionObject typeAction = serializer->GetTypeAction(objFields->ActionObjectCurrent);
		newObj->ActionObjectCurrent = typeAction;

		newObj->IndexObjectOwner = std::stoi(objFields->IndexObjectOwner);

		newObj->Color = objFields->ColorValue;

		OptionsObject opt = objFields->Options;
		newObj->IsVisible = StrToBool(opt.IsVisible);
		newObj->IsGravity = StrToBool(opt.IsGravity);
		newObj->IsGUI = StrToBool(opt.IsGUI);
		newObj->IsTextureRepeat = StrToBool(opt.IsTextureRepeat);
		newObj->IsNPC = StrToBool(opt.IsNPC);
		newObj->IsBlock = StrToBool(opt.IsBlock);
		newObj->IsCubeModel = StrToBool(opt.IsCubeModel);
		newObj->IsAbsolutePosition = StrToBool(opt.IsAbsolutePosition);
		newObj->IsFocusable = StrToBool(opt.IsFocusable);
		newObj->IsTransformable = StrToBool(opt.IsTransformable);
		newObj->IsUsable = StrToBool(opt.IsUsable);
		//newObj-> = StrToBool(opt.);
		
		//#SaveFieldSpecific
		specificFields = objectsDataSpecific[i];
		newObj->SetSpecificFiels(specificFields);

		//--- After init
		newObj->UpdateState();
		i++;
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
	nextModelPrt->Init();
	AddModel(nextModelPrt, "mon");

	ModelData nextModel2 = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModel2));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathTexture = "./Textures/planet.bmp";
	nextModelPrt->PathModel3D = "./Models3D/polygonPlane.obj";
	nextModelPrt->RadiusCollider = 0.2;
	nextModelPrt->Init();
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
	nextModelPrt->IsCubeModel = true;
	nextModelPrt->Init();
	AddModel(nextModelPrt, "box");

	ModelData nextModel4 = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModel4));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/Marker_Vector.obj";
	nextModelPrt->PathTexture = "./Textures/testTexture.bmp";
	nextModelPrt->RadiusCollider = 0.1;
	nextModelPrt->Init();
	AddModel(nextModelPrt, "marker_Vector");

	ModelData nextModel5 = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModel5));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/Marker_Cross.obj";
	nextModelPrt->PathTexture = "./Textures/testTexture.bmp";
	nextModelPrt->RadiusCollider = 0.1;
	nextModelPrt->Init();
	AddModel(nextModelPrt, "marker_Cross");

	ModelData nextModel6 = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModel6));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/Marker_Point.obj";
	nextModelPrt->PathTexture = "./Textures/testTexture.bmp";
	nextModelPrt->RadiusCollider = 0.1;
	nextModelPrt->Init();
	AddModel(nextModelPrt, "marker_Point");

	ModelData nextModelHomo = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModelHomo));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/Marker_Point.obj";
	nextModelPrt->PathTexture = "./Textures/future.bmp";
	nextModelPrt->RadiusCollider = 1;
	nextModelPrt->Init();
	AddModel(nextModelPrt, "homo");

	ModelData nextModelCursorRay = ModelData();
	Models.push_back(std::make_unique<ModelData>(nextModelCursorRay));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "basic.vert";
	nextModelPrt->PathShaderFrag = "basic.frag";
	nextModelPrt->PathModel3D = "./Models3D/Marker_Cross.obj";
	nextModelPrt->PathTexture = "./Textures/future.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->Init();
	AddModel(nextModelPrt, "cursorRay");

	//---GUI -- control -- Background frame
	ModelFrame contextFrame = ModelFrame();
	Models.push_back(std::make_unique<ModelFrame>(contextFrame));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "FrameUI.vert";
	nextModelPrt->PathShaderFrag = "FrameUI.frag";
	nextModelPrt->PathModel3D = "./Models3D/Frame.obj";
	//nextModelPrt->PathTexture = "./Textures/future.bmp";
	nextModelPrt->PathTexture = "./Textures/Frame.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->IsCubeModel = true;
	nextModelPrt->Init();
	AddModel(nextModelPrt, "ConextFrameModel");

	//---GUI -- control -- Frame
	ModelFrame modelFrame = ModelFrame();
	Models.push_back(std::make_unique<ModelFrame>(modelFrame));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "FrameUI.vert";
	nextModelPrt->PathShaderFrag = "FrameUI.frag";
	nextModelPrt->PathModel3D = "./Models3D/Frame.obj";
	nextModelPrt->PathTexture = "./Textures/syzanna.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->IsCubeModel = true;
	nextModelPrt->Init();
	AddModel(nextModelPrt, "FrameModel");

	//---GUI -- control -- TextBlock
	ModelTextBlock textBlock = ModelTextBlock();
	Models.push_back(std::make_unique<ModelTextBlock>(textBlock));
	nextModelPrt = GetModelPrt(Models.size() - 1);
	nextModelPrt->PathShaderVertex = "TextBlockUI.vert";
	nextModelPrt->PathShaderFrag = "TextBlockUI.frag";
	nextModelPrt->PathModel3D = "./Models3D/TextBlock.obj";
	nextModelPrt->PathTexture = "./Textures/Alphabet.bmp";
	nextModelPrt->RadiusCollider = .1;
	nextModelPrt->IsCubeModel = true;
	nextModelPrt->Init();
	AddModel(nextModelPrt, "TextBlockModel");

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
	nextModelPrt->IsCubeModel = true;
	nextModelPrt->Init();
	AddModel(nextModelPrt, "CursorModel");
}

void CreatorModelData::LoadObjectsGUI() {

	// ---- Object Context frame GUI
	shared_ptr<ModelData> modelGUI = GetModelPrt("ConextFrameModel");
	shared_ptr<ObjectData> objBackGUI_Data = AddObject("BackContectGUI", modelGUI, GUI, vec3(0, -50, 0), vec3(1));
	shared_ptr<ObjectGUI> objBackGUI = std::dynamic_pointer_cast<ObjectGUI>(objBackGUI_Data);

	// ---- Object frame
	string caption;
	string childModel = "conextGUI_2";
	//childModel = "conextGUI_T";
	string objName;

	// ---- Object Cursor GUI
	//objName = "CursorGUI";
	//caption = objBackGUI->Name + "." + objName;
	//childModel = "CursorModel";
	////objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.15, .15, .01), vec2(.05, .05), CursorGUI, vec3(0.2, 0.5, 0.1));
	//objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.15, .15, .03), vec2(.05, .05), CursorGUI, vec3(0.2, 0.5, 0.1));

	/*objName = "TEST1";
	caption = objBackGUI->Name + "." + objName;
	objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.05, .05, 0.01), vec2(0.1, 0.1));


	objName = "TEST2";
	caption = objBackGUI->Name + "." + objName;
	objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.3, .05, 0.01), vec2(0.1, 0.1), vec3(0,1,1));*/

	// ---- Object frame
	objName = "TEST3";
	caption = objBackGUI->Name + "." + objName;
	childModel = "FrameModel";
	//objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.4, .01, 0.01), vec2(1.1, 0.2), GUI, vec3(1));
	objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.4, .01, 0.01), vec2(1.1, 0.2), GUI, vec3(1));

	// ---- Object text block GUI
	objName = "TextBlockObject";
	caption = "привет мир, и доброе утро";
	childModel = "TextBlockModel";
	objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.01, .01, 0.011), vec2(1.5, 1.), TextBlock, vec3(0.2, 0.5, 0.1));

	// ---- Object Cursor GUI (Last is alpha background fix)
	objName = "CursorGUI";
	caption = objBackGUI->Name + "." + objName;
	childModel = "CursorModel";
	//objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.15, .15, .01), vec2(.05, .05), CursorGUI, vec3(0.2, 0.5, 0.1));
	objBackGUI->ConfigInterface(caption, childModel, objName, vec3(.15, .15, .03), vec2(.05, .05), CursorGUI, vec3(0.2, 0.5, 0.1));
}


void CreatorModelData::LoadObjects() {

	SceneSerialize* serializer = new SceneSerialize();
	serializer->Load();
	LoadObjects(serializer->FiledsObjects, serializer->FiledsObjectsSpecific);

	if (SceneObjects.size() > 0)
		return;

	float offsetCentrePlane = 500;
	float radiusPlane = 70;

	std::shared_ptr<ModelData> modelPolygon = GetModelPrt("plane");
	AddObject("Plane", modelPolygon, Polygon, vec3(-20.f, -55, radiusPlane));
	//CurrentPolygonObject = GetObjectPrt("Plane");

	std::shared_ptr<ModelData> modelMon = GetModelPrt("mon");

    for (int i = 0; i < 1; i++)
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
	AddObject("Hero", modeHero, Hero, vec3(0, 0, 0));


	LoadObjectsGUI();

	//AddObject(this, "Hero", modelM_V, Hero, vec3(0, 0, 0));

	
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
}

void CreatorModelData::Load() {

	LoadModels();
	LoadObjects();
	LoadClusters();
}

void CreatorModelData::LoadClusters() {

	Clusters = new WorldCluster();
	Clusters->Storage = this;
	Clusters->PlaneClusterization();
}



