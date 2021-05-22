#include "SceneSerialize.h"

#include "CreatorModelData.h"  
#include "ModelData.h"
#include "ObjectsTypes\ObjectData.h"

//#include <vector>
//#include <map>
//
//using std::map;

SceneSerialize::SceneSerialize() {

	m_dataObjects = std::string();
	m_dataModels = std::string();
	m_stringSeparator = "------------------------------------------------------------------";

	//Create map types objects
	AddNextType(TypeObject::Polygon, "Polygon");
	AddNextType(TypeObject::Solid, "Solid");
	AddNextType(TypeObject::Block, "Block");
	AddNextType(TypeObject::Tree, "Tree");
	AddNextType(TypeObject::Terra, "Terra");
	AddNextType(TypeObject::NPC, "NPC");
	AddNextType(TypeObject::Bullet, "Bullet");
	AddNextType(TypeObject::Hero, "Hero");
	AddNextType(TypeObject::BulletHero, "BulletHero");
	AddNextType(TypeObject::CursorRay, "CursorRay");
	AddNextType(TypeObject::GUI, "GUI");

	//enum ActionObject { Moving, Stay, Look, Search, Lock, Starting }; 
	//Create map types action object
	AddNextType(ActionObject::Moving, "Moving");
	AddNextType(ActionObject::Stay, "Stay");
	AddNextType(ActionObject::Look, "Look");
	AddNextType(ActionObject::Search, "Search");
	AddNextType(ActionObject::Lock, "Lock");
	AddNextType(ActionObject::Starting, "Starting");
	//AddNextType(ActionObject::, "");
}

SceneSerialize::~SceneSerialize() {


}


void  SceneSerialize::Save(shared_ptr<ObjectData> obj) {

	ObjectFileds fileds;
	std::stringstream streamObjects;

	streamObjects << fileds.Type << GetNameType(obj->TypeObj) << "\n";
	streamObjects << fileds.Name << obj->Name << "\n";
	streamObjects << fileds.Model << obj->ModelPtr->Name << "\n";

	streamObjects << fileds.Postranslate << Vec3Str(obj->Postranslate) << "\n";
	streamObjects << fileds.Target << Vec3Str(obj->Target) << "\n";
	streamObjects << fileds.ActionObjectCurrent << GetNameType(obj->ActionObjectCurrent) << "\n";
	streamObjects << m_stringSeparator << "\n";
	
	m_dataObjects.append(streamObjects.str());
}

//void  SceneSerialize::Save(ObjectData obj) {
//}

void  SceneSerialize::Save(shared_ptr<ModelData> model) {

	std::stringstream streamModels;
	ModelFileds fileds;

	streamModels << fileds.Name << model->Name << "\n";

	m_dataModels.append(streamModels.str());
}

//void  SceneSerialize::Save(ModelData model) {
//}

std::string SceneSerialize::Vec3Str(glm::vec3 vec) {
	std::ostringstream os;
	os << vec.x << " " << vec.y << " " << vec.z;
	return os.str();
}

//-------------------------------------- ActionObject

void SceneSerialize::AddNextType(ActionObject typeObj, string Name) {
	m_mapNamesTypesActions.insert(std::pair<ActionObject, string>(typeObj, Name));
	m_mapTypesActions.insert(std::pair<string, ActionObject>(Name, typeObj));
}

string SceneSerialize::GetNameType(ActionObject typeObj) {

	string name = "Error";
	map <ActionObject, string> ::iterator it;
	
	auto map = m_mapNamesTypesActions;

	it = map.find(typeObj);
	if (it != map.end()) {
		name = map[typeObj];
	}
	return name;
}

ActionObject SceneSerialize::GetTypeAction(string name) {

	ActionObject result;
	map <string, ActionObject> ::iterator it;
	auto map = m_mapTypesActions;

	it = map.find(name);
	if (it != map.end()) {
		result = map[name];
	}
	return result;
}

//--------------------------------------- TypeObject
void SceneSerialize::AddNextType(TypeObject typeObj, string Name) {
	m_mapNamesTypesObjects.insert(std::pair<TypeObject, string>(typeObj, Name));
	m_mapTypesObjects.insert(std::pair<string, TypeObject>(Name, typeObj));
}

string SceneSerialize::GetNameType(TypeObject typeObj) {

	string name = "Error";
	map <TypeObject, string> ::iterator it;
	auto map = m_mapNamesTypesObjects;

	it = map.find(typeObj);
	if (it != map.end()) {
		name = map[typeObj];
	}
	return name;
}

TypeObject SceneSerialize::GetTypeObject(string name) {

	TypeObject result;
	map <string, TypeObject> ::iterator it;
	auto map = m_mapTypesObjects;

	it = map.find(name);
	if (it != map.end()) {
		result = map[name];
	}
	return result;
}
//----------------------------------------

void SceneSerialize::Save() {

	World WorldSetting;
	string filePath;

	std::ofstream out;

	if (!m_dataObjects.empty()) //не пустая  (!m_streamObjects.rdbuf()->in_avail()) /  (!m_streamObjects.str().empty())
	{
		filePath = WorldSetting.PathObjects + "/Objects.txt";

		out.open(filePath);
		if (out.is_open())
		{
			out << m_dataObjects << std::endl;
		}
		out.close();
		m_dataObjects = std::string();
	}

	if (!m_dataModels.empty())
	{
		filePath = WorldSetting.PathModels + "/Models.txt";
		std::getline(std::cin, filePath);
		out.open(filePath);
		if (out.is_open())
		{
			out << m_dataModels << std::endl;
		}
		out.close();
		m_dataModels = std::string();
	}
}

//void  SceneSerialize::Load(string path, ObjectData& obj, TypeObject type) {
//	
//
//}

void  SceneSerialize::Load() {


}