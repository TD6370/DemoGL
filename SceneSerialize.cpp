#include "SceneSerialize.h"

#include "CreatorModelData.h"  
#include "ModelData.h"
#include "ObjectsTypes\ObjectData.h"
//#include "CoreSettings.h"


SceneSerialize::SceneSerialize() {


}

SceneSerialize::~SceneSerialize() {


}


void  SceneSerialize::Save(ObjectData obj) {

	ObjectFileds fileds;

	m_streamObjects << fileds.Type  << obj.TypeObj;
	m_streamObjects << fileds.Name << obj.Name;
	m_streamObjects << fileds.Model << obj.ModelPtr->Name;

	m_streamObjects << fileds.Postranslate << Vec3Str(obj.Postranslate);
	m_streamObjects << fileds.Target << Vec3Str(obj.Target);
	m_streamObjects << fileds.ActionObjectCurrent << obj.ActionObjectCurrent;
}

std::string SceneSerialize::Vec3Str(glm::vec3 vec) {
	std::ostringstream os;
	os << vec.x << " " << vec.y << " " << vec.z;
	return os.str();
}

void  SceneSerialize::Save(ModelData model) {

	ModelFileds fileds;

	m_streamModels << fileds.Name << model.Name;
}


void  SceneSerialize::Save() {

	World WorldSetting;
	string filePath = WorldSetting.PathObjects + "\\Objects.txt";

	std::ofstream out;
	out.open(filePath);
	if (out.is_open())
	{
		out << m_streamObjects.str() << std::endl;
	}
	out.close();

	filePath = WorldSetting.PathModels + "\\Models.txt";
	out.open(filePath);
	if (out.is_open())
	{
		out << m_streamModels.str() << std::endl;
	}
	out.close();
}

//void  SceneSerialize::Load(string path, ObjectData& obj, TypeObject type) {
//	
//
//}

void  SceneSerialize::Load() {


}