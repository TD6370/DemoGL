//@@@
#include "CreatorModelData.h"
#include "WorldCollision.h"
#include "ModelData.h"
#include "ObjectData.h"

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

void LoadImageBmp(const char* pathImage)
{
	unsigned int width;
	unsigned int height;

	//pathTextureTest
	unsigned char* data = LoadBmp(pathImage, &width, &height);
	if (data == NULL)
	{
		std::cerr << "Could not read image file " << pathImage << ". File does not exist." << std::endl;
		return;
	}

	// Создаем одну OpenGL текстуру
	GLuint textureID;
	glGenTextures(1, &textureID);
	// Биндим текстуру, и теперь все функции по работе с текстурами будут работать с этой
	glBindTexture(GL_TEXTURE_2D, textureID);

	//GLint width  = 200;
	//GLint height = 200;

	// Отправляем картинку в OpenGL текстуру
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Когда картинка будет увеличиваться(нет большей Мипмапы), используем LINEAR фильтрацию
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Когда минимизируем — берем две ближних мипмапы и лиейно смешиваем цвета
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// И создаем сами мипмапы.
	glGenerateMipmap(GL_TEXTURE_2D);
}

CreatorModelData::CreatorModelData() {
	
}

void CreatorModelData::AddModel(ModelData* newModel, std::string name) {

	int index = Models.size();
	Models.push_back(std::make_unique<ModelData>(*newModel));
	
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

ObjectData CreatorModelData::AddObject(CreatorModelData* storage, string name, std::shared_ptr<ModelData> modelPtr, TypeObject p_typeObj, vec3 p_pos, vec3 p_color) {

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

	ObjectData objectModel = ObjectData(SceneObjectsLastIndex, modelPtr, p_typeObj, p_pos);
	objectModel.Storage = storage;
	objectModel.Color = p_color;
	objectModel.Name = name;

	SceneObjects.push_back(std::make_unique<ObjectData>(objectModel));

	MapSceneObjects.insert(std::pair<string, int>(name, SceneObjectsLastIndex));

	return objectModel;
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

void CreatorModelData::LoadModels() {

	ModelData nextModel = ModelData();
	nextModel.PathShaderVertex = "basic.vert";
	nextModel.PathShaderFrag = "basic.frag";
	nextModel.PathTexture = "./Textures/testTexture.bmp";
	//nextModel.PathTexture = "./Textures/syzanna.bmp";
	//nextModel.PathModel3D = "./Models3D/polygonPlane.obj";
	nextModel.PathModel3D = "./Models3D/monkey.obj";
	//nextModel.PathModel3D = "./Models3D/kub.obj";
	nextModel.RadiusCollider = 1;
	nextModel.Init();
	AddModel(&nextModel, "mon");

	ModelData nextModel2 = ModelData();
	nextModel2.PathShaderVertex = "basic.vert";
	nextModel2.PathShaderFrag = "basic.frag";
	nextModel2.PathTexture = "./Textures/planet.bmp";
	//nextModel2.PathModel3D = "./Models3D/kub.obj";
	//nextModel2.PathModel3D = "./Models3D/monkey.obj";
	nextModel2.PathModel3D = "./Models3D/polygonPlane.obj";
	nextModel2.RadiusCollider = 0.2;
	nextModel2.Init();
	AddModel(&nextModel2, "plane");

	ModelData nextModel3 = ModelData();
	nextModel3.PathShaderVertex = "basic.vert";
	nextModel3.PathShaderFrag = "basic.frag";
	nextModel3.PathModel3D = "./Models3D/kub.obj";
	nextModel3.PathTexture = "./Textures/testTexture.bmp";
	nextModel3.RadiusCollider = 1;
	nextModel3.Init();
	AddModel(&nextModel3, "box");


	ModelData nextModel4 = ModelData();
	nextModel4.PathShaderVertex = "basic.vert";
	nextModel4.PathShaderFrag = "basic.frag";
	nextModel4.PathModel3D = "./Models3D/Marker_Vector.obj";
	nextModel4.PathTexture = "./Textures/testTexture.bmp";
	nextModel4.RadiusCollider = 0.1;
	nextModel4.Init();
	AddModel(&nextModel4, "marker_Vector");

	ModelData nextModel5 = ModelData();
	nextModel5.PathShaderVertex = "basic.vert";
	nextModel5.PathShaderFrag = "basic.frag";
	nextModel5.PathModel3D = "./Models3D/Marker_Cross.obj";
	nextModel5.PathTexture = "./Textures/testTexture.bmp";
	nextModel5.RadiusCollider = 0.1;
	nextModel5.Init();
	AddModel(&nextModel5, "marker_Cross");

	ModelData nextModel6 = ModelData();
	nextModel6.PathShaderVertex = "basic.vert";
	nextModel6.PathShaderFrag = "basic.frag";
	nextModel6.PathModel3D = "./Models3D/Marker_Point.obj";
	nextModel6.PathTexture = "./Textures/testTexture.bmp";
	nextModel6.RadiusCollider = 0.1;
	nextModel6.Init();
	AddModel(&nextModel6, "marker_Point");

	ModelData nextModelHomo = ModelData();
	nextModelHomo.PathShaderVertex = "basic.vert";
	nextModelHomo.PathShaderFrag = "basic.frag";
	//nextModelHomo.PathModel3D = "./Models3D/homo.obj";
	nextModelHomo.PathModel3D = "./Models3D/Marker_Point.obj";
	nextModelHomo.PathTexture = "./Textures/future.bmp";
	//nextModelHomo.PathTexture = "./Textures/t2.bmp";
	nextModelHomo.RadiusCollider = 1;
	nextModelHomo.Init();
	AddModel(&nextModelHomo, "homo");


}

void CreatorModelData::LoadObjects() {

	std::shared_ptr<ModelData> modelMon = GetModelPrt("mon");

    for (int i = 0; i < 100; i++)
	{
		//std::shared_ptr<ModelData> model3 = GetModelPrt(0);
		AddObject(this, "Mon", modelMon, NPC);
	}

	vec3 color_red = vec3(1, 0, 0);
	vec3 color_blue = vec3(0, 0, 1);
	vec3 color_green = vec3(0, 1, 0);
	vec3 color_violet = vec3(1, 0, 1);
	vec3 color_yelow = vec3(0, 1, 1);

	/*std::shared_ptr<ModelData> modelK = GetModelPrt(2);
	AddObject(modelK, Polygon, vec3(0, -55, 0));
	AddObject(modelK, Polygon, vec3(0, -55, 0));*/


	std::shared_ptr<ModelData> modelBox = GetModelPrt("box");
	AddObject(this, "Box", modelBox, Block, vec3(-50, -55, 70));
	AddObject(this, "Box2", modelBox, Block, vec3(-50, -55, 70));
	//AddObject(this, "Marker", modelBox, Block, vec3(0, -55, 0));

	std::shared_ptr<ModelData> modelM_P = GetModelPrt("marker_Point");
	AddObject(this, "M_P_1", modelM_P, Block, vec3(-50, 0, 0));
	AddObject(this, "M_P_2", modelM_P, Block, vec3(-50, 0, 0));
	//AddObject(this, "Bullet", modelM_P, Bullet, vec3(0, 0, 0));
	AddObject(this, "Bullet", modelBox, Bullet, vec3(0, 0, 0));


	std::shared_ptr<ModelData> modelM_C = GetModelPrt("marker_Cross");
	AddObject(this, "M_C_1", modelM_C, Block, vec3(0, 0, -50));
	AddObject(this, "M_C_2", modelM_C, Block, vec3(0, 0, -50));

	std::shared_ptr<ModelData> modelM_V = GetModelPrt("marker_Vector");
	AddObject(this, "M_V_1", modelM_V, Block, vec3(0, 0, 50), color_green);
	AddObject(this, "M_V_2", modelM_V, Block, vec3(0, 0, 50), color_blue);
	AddObject(this, "M_V_3", modelM_V, Block, vec3(0, 0, 50), color_yelow);

	std::shared_ptr<ModelData> modeHero = GetModelPrt("homo");
	AddObject(this, "Hero", modeHero, Hero, vec3(0, 0, 0));
	//AddObject(this, "Hero", modelM_V, Hero, vec3(0, 0, 0));


	float offsetCentrePlane = 500;
	float radiusPlane = 70;

	/* for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 20; z++)
		{
			int o_x = -offsetCentrePlane + (radiusPlane * x);
			int o_z = -offsetCentrePlane + (radiusPlane * z);
			std::shared_ptr<ModelData> model1 = GetModelPrt("plane");
			AddObject("Plane", model1, Polygon, vec3(o_x, -55, o_z));
		}
	}*/


	std::shared_ptr<ModelData> modelPolygon = GetModelPrt("plane");
	//AddObject("Plane", model1, Polygon, vec3(radiusPlane/2, -55, radiusPlane / 2));
	AddObject(this, "Plane", modelPolygon, Polygon, vec3(-20.f, -55, radiusPlane));
	CurrentPolygonObject = GetObjectPrt("Plane");

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



