#include "SceneSerialize.h"

#include "../CreatorModelData.h"  
#include "../ModelData.h"
#include "../ObjectsTypes/ObjectData.h"

#include "../OperationString.h"

//#include <vector>
//#include <map>
//
//using std::map;

SceneSerialize::SceneSerialize() {

	m_dataObjects = std::string();
	m_dataModels = std::string();
	m_stringSeparator = "------------------------------------------------------------------";
	m_specificFields = "Specific:";

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
 
// -------------------- Serialize	->	ObjectData specific fields
//#SaveFieldSpecific
void  SceneSerialize::SaveSpecific(vector<ObjectFiledsSpecific>& specificFields) {

	//#SaveFieldSpecific
	return;

	ObjectFileds fileds;
	std::stringstream streamObjects;

	if (specificFields.size() == 0)
		return;

	streamObjects << m_specificFields << "\n";
	
	for (auto field : specificFields)
	{
		streamObjects << field.FieldName << " " << field.Value << "\n";
	}
	
	streamObjects << m_stringSeparator << "\n";

	m_dataObjects.append(streamObjects.str());
}

// -------------------- Serialize	->	ObjectData
void  SceneSerialize::Save(shared_ptr<ObjectData> obj) {

	ObjectFileds fileds;
	std::stringstream streamObjects;

	streamObjects << fileds.Type << " " << GetNameType(obj->TypeObj) << "\n";
	streamObjects << fileds.Name << " " << obj->Name << "\n";
	streamObjects << fileds.Model << " " << obj->ModelPtr->Name << "\n";

	streamObjects << fileds.Postranslate << " " << Vec3Str(obj->Postranslate) << "\n";
	streamObjects << fileds.Target << " " << Vec3Str(obj->Target) << "\n";
	streamObjects << fileds.ActionObjectCurrent << " " << GetNameType(obj->ActionObjectCurrent) << "\n";
	streamObjects << m_stringSeparator << "\n";
	
	m_dataObjects.append(streamObjects.str());
}

// -------------------- Serialize	->	ModelData

void  SceneSerialize::Save(shared_ptr<ModelData> model) {

	std::stringstream streamModels;
	ModelFileds fileds;

	streamModels << fileds.Name << " " << model->Name << "\n";
	streamModels << fileds.PathShaderVertex << " " << model->PathShaderVertex << "\n";
	streamModels << fileds.PathShaderFrag << " " << model->PathShaderFrag << "\n";
	streamModels << fileds.PathTexture << " " << model->PathTexture << "\n";
	streamModels << fileds.PathModel3D << " " << model->PathModel3D << "\n";
	streamModels << m_stringSeparator << " " << "\n";

	m_dataModels.append(streamModels.str());
}

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
		filePath = WorldSetting.PathObjects;

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
		filePath = WorldSetting.PathModels;
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

void SceneSerialize::Load(bool isOnlyObjects) {

	World WorldSetting;
	string filePath;
	std::ifstream in;
	//ObjectFileds* filedsObj = new ObjectFileds;
	string stringSeparator;
	string specificFields;
	string lineStr;
	int lineInt;
	float lineF;

	filePath = WorldSetting.PathObjects;

	in.open(filePath);

	/*if (!in.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return NULL;
	}*/

	//---- load objects

	if (in.is_open())
	{
		FiledsObjects.clear();
		FiledsObjectsSpecific.clear();
		vector<ObjectFiledsSpecific> specificFiels;

		//vector<shared_ptr<ObjectFiledsSpecific>>* specificFiels;
		//vector<shared_ptr<ObjectFiledsSpecific>> specificFiels;

		bool isNext = false;

		ObjectFileds filedsObj;
		ObjectFileds filedsObjResult;

		while (!in.eof()) {
			//std::getline(in, line);

			if (in >> lineStr && lineStr == filedsObj.Type) {
				if (isNext)
				{
					FiledsObjectsSpecific.push_back(specificFiels);
					FiledsObjects.push_back(std::make_unique<ObjectFileds>(filedsObjResult));
					isNext = false;
				}

				in >> filedsObj.Type;
			}
			else {
				if (lineStr.size() == 0) {
					break;
				}
			}
			if (in >> lineStr && lineStr == filedsObj.Name)
				in >> filedsObj.Name;
			if (in >> lineStr && lineStr == filedsObj.Model)
				in >> filedsObj.Model;

			if (in >> lineStr && lineStr == filedsObj.Postranslate) {
				in >> filedsObj.PostranslateValue.x;
				in >> filedsObj.PostranslateValue.y;
				in >> filedsObj.PostranslateValue.z;
			}

			if (in >> lineStr && lineStr == filedsObj.Target) {
				in >> filedsObj.TargetValue.x;
				in >> filedsObj.TargetValue.y;
				in >> filedsObj.TargetValue.z;
			}

			if (in >> lineStr && lineStr == filedsObj.ActionObjectCurrent)
				in >> filedsObj.ActionObjectCurrent;

			if (in >> lineStr && lineStr == m_stringSeparator) {

				filedsObjResult = filedsObj;
				filedsObj = ObjectFileds();
				isNext = true;
			}
			else if (lineStr == m_specificFields)
			{
				//#SaveFieldSpecific
				ObjectFiledsSpecific fieldsSpecific = ObjectFiledsSpecific();
				in >> fieldsSpecific.FieldName;
				in >> fieldsSpecific.Value;
				specificFiels.push_back(fieldsSpecific);
			}
		}
	}

	
	in.close();

	if (isOnlyObjects)
		return;
	
	//---- load models

	filePath = WorldSetting.PathModels;

	in.open(filePath);

	if (in.is_open())
	{
		FiledsModels.clear();
		while (!in.eof()) {

			ModelFileds* filedsModel = new ModelFileds;

			if (in >> lineStr && lineStr == filedsModel->Name)
				in >> filedsModel->Name;
			else
				break;

			if (in >> lineStr && lineStr == filedsModel->PathShaderVertex)
				in >> filedsModel->PathShaderVertex;

			if (in >> lineStr && lineStr == filedsModel->PathShaderFrag)
				in >> filedsModel->PathShaderFrag;

			if (in >> lineStr && lineStr == filedsModel->PathTexture)
				in >> filedsModel->PathTexture;

			if (in >> lineStr && lineStr == filedsModel->PathModel3D)
				in >> filedsModel->PathModel3D;
			
			in >> stringSeparator;

			FiledsModels.push_back(std::make_unique<ModelFileds>(*filedsModel));
		}
	}
	in.close();
}