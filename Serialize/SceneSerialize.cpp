#include "SceneSerialize.h"

#include "../CreatorModelData.h"  
#include "../ModelData.h"
#include "../ObjectsTypes/ObjectData.h"
#include "../ShellObjects/BaseShell.h"

#include "../OperationString.h"


SceneSerialize::SceneSerialize() {

	m_dataObjects = std::string();
	m_dataModels = std::string();
	m_mapObjectFieldsNameAndValue = map<string, string>();
	m_mapObjectFieldsNames = vector<string>();
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
	AddNextType(TypeObject::TextBox, "TextBox");
	AddNextType(TypeObject::CursorGUI, "CursorGUI");
	AddNextType(TypeObject::Button, "Button");
	AddNextType(TypeObject::EditBox, "EditBox");
	AddNextType(TypeObject::ListBox, "ListBox");
	AddNextType(TypeObject::ListEditBox, "ListEditBox");
	AddNextType(TypeObject::ListTextBox, "ListTextBox");
	AddNextType(TypeObject::ObjectFieldsEdit, "ObjectFieldsEdit");

	//Create map types action object
	AddNextType(ActionObject::Moving, "Moving");
	AddNextType(ActionObject::Stay, "Stay");
	AddNextType(ActionObject::Look, "Look");
	AddNextType(ActionObject::Search, "Search");
	AddNextType(ActionObject::Lock, "Lock");
	AddNextType(ActionObject::Starting, "Starting");
	AddNextType(ActionObject::Woking, "Woking");
	AddNextType(ActionObject::Transforming, "Transforming");
	AddNextType(ActionObject::Clearing, "Clearing");
	
	//AddNextType(ActionObject::ReadingEvents, "ReadingEvents");
	
	//AddNextType(ActionObject::, "");

	AddNextType(TypeCommand::None, "None");
	AddNextType(TypeCommand::CreateObject, "CreateObject");
	AddNextType(TypeCommand::DeleteObject, "DeleteObject");
	AddNextType(TypeCommand::SelectPosForObject, "SelectPosForObject");
	AddNextType(TypeCommand::EditGUI_OnOff, "EditGUI_OnOff");
	AddNextType(TypeCommand::EditObjectCommand, "EditObjectCommand");
	AddNextType(TypeCommand::CheckStateObjectCommand, "CheckStateObjectCommand");
	AddNextType(TypeCommand::StopWorking, "StopWorking");
	AddNextType(TypeCommand::KeyInputCommand, "KeyInputCommand");
	AddNextType(TypeCommand::RenameObject, "RenameObject");
	AddNextType(TypeCommand::ObjectReading, "ObjectReading");
	AddNextType(TypeCommand::SelectItemValue, "SelectItemValue");
	
	AddNextType(TypeLayer::LayerBackground, "LayerBackground");
	AddNextType(TypeLayer::LayerBack, "LayerBack");
	AddNextType(TypeLayer::LayerNone, "LayerNone"); 
	AddNextType(TypeLayer::LayerBase, "LayerBase");
	AddNextType(TypeLayer::LayerFront, "LayerFront");
	AddNextType(TypeLayer::LayerBase, "LayerBase");
	AddNextType(TypeLayer::LayerSystem, "LayerSystem");
}

SceneSerialize::~SceneSerialize() {
}
 
// -------------------- Serialize	->	ObjectData specific fields
//#SaveFieldSpecific
void  SceneSerialize::SaveSpecific(vector<ObjectFiledsSpecific>& specificFields) {

	if (specificFields.size() == 0)
		return;

	ObjectFileds fileds;
	std::stringstream streamObjects;

	streamObjects << m_specificFields << "\n";
	
	for (auto field : specificFields)
	{
		streamObjects << field.FieldName << " " << field.Value << "\n";
	}
	
	streamObjects << m_stringSeparator << "\n";

	m_dataObjects.append(streamObjects.str());
}

// -------------------- Serialize	->	ObjectData
void SceneSerialize::Save(shared_ptr<ObjectData> obj, bool isSpecificExist, bool isGet) {

	if (isGet) {
		/*if (m_indLastObjectSerialize == obj->Index)
			return;
		m_indLastObjectSerialize = obj->Index;*/
		m_dataObjects.clear();
	}

	ObjectFileds fileds;
	std::stringstream streamObjects;

	streamObjects << fileds.Type << " " << GetNameType(obj->TypeObj) << "\n";
	streamObjects << fileds.Name << " " << obj->Name << "\n";
	streamObjects << fileds.Model << " " << obj->ModelPtr->Name << "\n";
	streamObjects << fileds.Index << " " << obj->Index << "\n";
	streamObjects << fileds.Layer << " " << GetNameType(obj->Layer) << "\n";

	streamObjects << fileds.Postranslate << " " << Vec3Str(obj->Postranslate) << "\n";
	streamObjects << fileds.Target << " " << Vec3Str(obj->Target) << "\n";
	streamObjects << fileds.ActionObjectCurrent << " " << GetNameType(obj->ActionObjectCurrent) << "\n";

	streamObjects << fileds.IndexObjectOwner << " " << obj->IndexObjectOwner << "\n";
	streamObjects << fileds.ShellIndex << " " << obj->ShellIndex << "\n";
	streamObjects << fileds.NextItemShellIndex << " " << obj->NextItemShellIndex << "\n";

	streamObjects << fileds.Color << " " << Vec3Str(obj->MaterialData.Color) << "\n";

	streamObjects << fileds.Command << " " << GetNameType(obj->SceneCommand->CommandType) << "\n";
	streamObjects << fileds.CommandSourceIndex << " " << obj->SceneCommand->SourceIndex << "\n";
	streamObjects << fileds.CommandTargetIndex << " " << obj->SceneCommand->TargetIndex << "\n";
	streamObjects << fileds.CommandValueI << " " << obj->SceneCommand->ValueI << "\n";
	streamObjects << fileds.CommandValueF << " " << obj->SceneCommand->ValueF << "\n";
	streamObjects << fileds.CommandValueS << " " << GetStrValue(obj->SceneCommand->ValueS) << "\n";
	streamObjects << fileds.CommandDescription << " " << GetStrValue(obj->SceneCommand->Description) << "\n";
	//Options
	streamObjects << fileds.Options.IsVisible << " " << obj->IsVisible << "\n";
	streamObjects << fileds.Options.IsGravity << " " << obj->IsGravity << "\n";
	streamObjects << fileds.Options.IsGUI << " " << obj->IsGUI << "\n";
	streamObjects << fileds.Options.IsTextureRepeat << " " << obj->IsTextureRepeat << "\n";
	streamObjects << fileds.Options.IsNPC << " " << obj->IsNPC << "\n";
	streamObjects << fileds.Options.IsHexagonModel << " " << obj->IsHexagonModel << "\n";
	streamObjects << fileds.Options.IsSquareModel << " " << obj->IsSquareModel << "\n";
	streamObjects << fileds.Options.IsAbsolutePosition << " " << obj->IsAbsolutePosition << "\n";
	streamObjects << fileds.Options.IsFocusable << " " << obj->IsFocusable << "\n";
	streamObjects << fileds.Options.IsTransformable << " " << obj->IsTransformable << "\n";
	streamObjects << fileds.Options.IsUsable << " " << obj->IsUsable << "\n";

	streamObjects << fileds.Options.IsChecked << " " << obj->IsChecked << "\n";

	if (isGet) {
		AddFieldNameAndValue(streamObjects);
		return;
	}

	if(!isSpecificExist)
		streamObjects << m_stringSeparator << "\n";
	
	m_dataObjects.append(streamObjects.str());
}

// -------------------- Serialize	->	ModelData

void  SceneSerialize::Save(shared_ptr<ModelData> model) {

	std::stringstream streamModels;
	ModelFileds fileds;

	streamModels << fileds.Name << " " << model->Name << "\n";
	streamModels << fileds.TypeName << " " << model->TypeName << "\n";
	streamModels << fileds.PathShaderVertex << " " << model->MaterialData.PathShaderVertex << "\n";
	streamModels << fileds.PathShaderFrag << " " << model->MaterialData.PathShaderFrag << "\n";
	streamModels << fileds.PathTexture << " " << model->MaterialData.PathTexture << "\n";
	streamModels << fileds.PathTextureAtlas << " " << GetStrValue(model->MaterialData.PathTextureAtlas) << "\n";
	streamModels << fileds.PathModel3D << " " << model->MeshData.PathModel3D << "\n";

	streamModels << fileds.RadiusCollider << " " << model->MeshData.RadiusCollider << "\n";
	streamModels << fileds.IsSquareModel << " " << model->MeshData.IsSquareModel << "\n";	///?????
	
	streamModels << m_stringSeparator << " " << "\n";

	m_dataModels.append(streamModels.str());
}

void  SceneSerialize::Save(shared_ptr<BaseShell> shell) {

	std::stringstream streamModels;
	ShellFileds fileds;

	streamModels << fileds.Name << " " << shell->Name << "\n";
	streamModels << fileds.Index << " " << shell->Index << "\n";
	streamModels << fileds.RootObjIndex << " " << shell->RootObjIndex << "\n";
	streamModels << fileds.CaptionObjIndex << " " << shell->CaptionObjIndex << "\n";
	streamModels << fileds.HeadIndexList << " " << shell->HeadIndexList << "\n";
	
	streamModels << m_stringSeparator << " " << "\n";

	m_dataShells.append(streamModels.str());
}
//BaseShell

//--- ***

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

	if (!m_dataShells.empty())
	{
		filePath = WorldSetting.PathShells;
		out.open(filePath);
		if (out.is_open())
		{
			out << m_dataShells << std::endl;
		}
		out.close();
		m_dataShells = std::string();
	}
}



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

		bool isNext = false;
		bool isSpecificFields = false;

		ObjectFileds filedsObj;
		ObjectFileds filedsObjResult;
		ObjectFiledsSpecific fieldsSpecific = ObjectFiledsSpecific();

		while (!in.eof()) {

			if (!isSpecificFields)
			{
				if (isNext)
				{
					FiledsObjectsSpecific.push_back(specificFiels);
					FiledsObjects.push_back(std::make_unique<ObjectFileds>(filedsObjResult));
					specificFiels.clear();
					isNext = false;
				}
				if (in >> lineStr && lineStr == filedsObj.Type) {
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
				if (in >> lineStr && lineStr == filedsObj.Index)
					in >> filedsObj.Index;
				if (in >> lineStr && lineStr == filedsObj.Layer)
					in >> filedsObj.Layer;

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

				if (in >> lineStr && lineStr == filedsObj.IndexObjectOwner)
					in >> filedsObj.IndexObjectOwner;
				if (in >> lineStr && lineStr == filedsObj.ShellIndex)
					in >> filedsObj.ShellIndex;
				if (in >> lineStr && lineStr == filedsObj.NextItemShellIndex)
					in >> filedsObj.NextItemShellIndex;

				if (in >> lineStr && lineStr == filedsObj.Color) {
					in >> filedsObj.ColorValue.x;
					in >> filedsObj.ColorValue.y;
					in >> filedsObj.ColorValue.z;
				}

				if (in >> lineStr && lineStr == filedsObj.Command)
					in >> filedsObj.Command;

				if (in >> lineStr && lineStr == filedsObj.CommandSourceIndex)
					in >> filedsObj.CommandSourceIndex;
				if (in >> lineStr && lineStr == filedsObj.CommandTargetIndex)
					in >> filedsObj.CommandTargetIndex;
				if (in >> lineStr && lineStr == filedsObj.CommandValueI)
					in >> filedsObj.CommandValueI;
				if (in >> lineStr && lineStr == filedsObj.CommandValueF)
					in >> filedsObj.CommandValueF;
				if (in >> lineStr && lineStr == filedsObj.CommandValueS)
					SetStrValue(in, filedsObj.CommandValueS);
					//in >> filedsObj.CommandValueS;

				if (in >> lineStr && lineStr == filedsObj.CommandDescription)
					SetStrValue(in, filedsObj.CommandDescription);
				if (in >> lineStr && lineStr == filedsObj.Options.IsVisible)
					in >> filedsObj.Options.IsVisible;
				if (in >> lineStr && lineStr == filedsObj.Options.IsGravity)
					in >> filedsObj.Options.IsGravity;
				if (in >> lineStr && lineStr == filedsObj.Options.IsGUI)
					in >> filedsObj.Options.IsGUI;
				if (in >> lineStr && lineStr == filedsObj.Options.IsTextureRepeat)
					in >> filedsObj.Options.IsTextureRepeat;
				if (in >> lineStr && lineStr == filedsObj.Options.IsNPC)
					in >> filedsObj.Options.IsNPC;
				if (in >> lineStr && lineStr == filedsObj.Options.IsHexagonModel)
					in >> filedsObj.Options.IsHexagonModel;
				if (in >> lineStr && lineStr == filedsObj.Options.IsSquareModel)
					in >> filedsObj.Options.IsSquareModel;
				if (in >> lineStr && lineStr == filedsObj.Options.IsAbsolutePosition)
					in >> filedsObj.Options.IsAbsolutePosition;
				if (in >> lineStr && lineStr == filedsObj.Options.IsFocusable)
					in >> filedsObj.Options.IsFocusable;
				if (in >> lineStr && lineStr == filedsObj.Options.IsTransformable)
					in >> filedsObj.Options.IsTransformable;
				if (in >> lineStr && lineStr == filedsObj.Options.IsUsable)
					in >> filedsObj.Options.IsUsable;
				if (in >> lineStr && lineStr == filedsObj.Options.IsChecked)
					in >> filedsObj.Options.IsChecked;

		/*		if (in >> lineStr && lineStr == filedsObj.Options.)
					in >> filedsObj.Options.;*/
							
			}

			if (in >> lineStr && lineStr == m_stringSeparator) {

				filedsObjResult = filedsObj;
				filedsObj = ObjectFileds();
				isSpecificFields = false;
				isNext = true;
			}
			else if (lineStr == m_specificFields)
			{
				isSpecificFields = true;
			}
			else if (isSpecificFields) 
			{
				fieldsSpecific.FieldName = lineStr;
				std::getline(in, fieldsSpecific.Value);
				if (fieldsSpecific.Value[0] == char(' ')) //remove split
					fieldsSpecific.Value.erase(0, 1);
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

			if (in >> lineStr && lineStr == filedsModel->TypeName)
				in >> filedsModel->TypeName;

			if (in >> lineStr && lineStr == filedsModel->PathShaderVertex)
				in >> filedsModel->PathShaderVertex;

			if (in >> lineStr && lineStr == filedsModel->PathShaderFrag)
				in >> filedsModel->PathShaderFrag;

			if (in >> lineStr && lineStr == filedsModel->PathTexture)
				in >> filedsModel->PathTexture;

			if (in >> lineStr && lineStr == filedsModel->PathTextureAtlas)
				SetStrValue(in, filedsModel->PathTextureAtlas);
			
			if (in >> lineStr && lineStr == filedsModel->PathModel3D)
				in >> filedsModel->PathModel3D;

			if (in >> lineStr && lineStr == filedsModel->RadiusCollider)
				in >> filedsModel->RadiusCollider;

			if (in >> lineStr && lineStr == filedsModel->IsSquareModel)
				in >> filedsModel->IsSquareModel;

		
			
			in >> stringSeparator;

			FiledsModels.push_back(std::make_unique<ModelFileds>(*filedsModel));
		}
	}
	in.close();

	//----- Shells

	filePath = WorldSetting.PathShells;

	in.open(filePath);

	if (in.is_open())
	{
		FiledsShells.clear();
		while (!in.eof()) {

			ShellFileds* filedsShell = new ShellFileds;

			if (in >> lineStr && lineStr == filedsShell->Name)
				in >> filedsShell->Name;
			else
				break;

			if (in >> lineStr && lineStr == filedsShell->Index)
				in >> filedsShell->Index;
			if (in >> lineStr && lineStr == filedsShell->RootObjIndex)
				in >> filedsShell->RootObjIndex;
			if (in >> lineStr && lineStr == filedsShell->CaptionObjIndex)
				in >> filedsShell->CaptionObjIndex;
			if (in >> lineStr && lineStr == filedsShell->HeadIndexList)
				in >> filedsShell->HeadIndexList;

			in >> stringSeparator;

			FiledsShells.push_back(std::make_unique<ShellFileds>(*filedsShell));
		}
	}
	in.close();
}

//---------------- Get value

void SceneSerialize::AddFieldNameAndValue(std::stringstream& stringStream) {

	m_mapObjectFieldsNameAndValue.clear();
	m_mapObjectFieldsNames.clear();
	while (!stringStream.eof()) {
		string strFieldName;
		string strFieldValue;
		stringStream >> strFieldName;
		SetStrValue(stringStream, strFieldValue);
		if (strFieldName.size() == 0)
			continue;
		strFieldName = FormatFieldName(strFieldName);
		strFieldValue = FormatEmptyValue(strFieldValue);
		m_mapObjectFieldsNameAndValue.insert({ strFieldName , strFieldValue });
		//m_mapObjectFieldsNameAndValue.insert(m_mapObjectFieldsNameAndValue.begin(), { strFieldName , strFieldValue });
		m_mapObjectFieldsNames.push_back(strFieldName);
	}
}


string SceneSerialize::GetFieldValueByName(string fieldName) 
{
	return m_mapObjectFieldsNameAndValue[fieldName];
}

map<string, string> SceneSerialize::GetObjectListFieldValue() 
{
	return m_mapObjectFieldsNameAndValue;
}

vector<string> SceneSerialize::GetObjectListFields() 
{
	return m_mapObjectFieldsNames;
}

//----- ****

string SceneSerialize::GetStrValue(string value) {
	if (value.size() == 0)
		value = STR_EMPTY;
	else if (value == STR_EMPTY)
		return std::string();
	return value;
}

const char* SceneSerialize::GetStrValueToChar(string value) {
	string result = GetStrValue(value);
	return result.c_str();
}

//--- fix space in text
void SceneSerialize::SetStrValue(std::ifstream& in, string& value) {

	std::getline(in, value);
	if (value[0] == char(' ')) //remove split
		value.erase(0, 1);
}

void SceneSerialize::SetStrValue(std::stringstream& in, string& value) {

	std::getline(in, value);
	if (value[0] == char(' ')) //remove split
		value.erase(0, 1);
}

map<TypeObject, string> SceneSerialize::GetNamesTypesObjects() {
	return m_mapNamesTypesObjects;
}

std::string SceneSerialize::Vec3Str(glm::vec3 vec) {
	std::ostringstream os;
	os << vec.x << " " << vec.y << " " << vec.z;
	return os.str();
}

std::string SceneSerialize::Vec2Str(glm::vec2 vec) {
	std::ostringstream os;
	os << vec.x << " " << vec.y;
	return os.str();
}

vec3 SceneSerialize::StrToVec3(string& value) {
	std::stringstream streamFieldValue;
	vec3 result;

	streamFieldValue.str(value);
	streamFieldValue >> result.x;
	streamFieldValue >> result.y;
	streamFieldValue >> result.z;

	return result;
}

vec2 SceneSerialize::StrToVec2(string& value) {
	std::stringstream streamFieldValue;
	vec2 result;

	streamFieldValue.str(value);
	streamFieldValue >> result.x;
	streamFieldValue >> result.y;

	return result;
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
//--------	SceneCommands	-------------------------------- 

void SceneSerialize::AddNextType(TypeCommand typeObj, string Name) {
	m_mapNamesTypesCommands.insert(std::pair<TypeCommand, string>(typeObj, Name));
	m_mapTypesCommands.insert(std::pair<string, TypeCommand>(Name, typeObj));
}

string SceneSerialize::GetNameType(TypeCommand typeObj) {

	string name = "Error";
	map <TypeCommand, string> ::iterator it;
	auto map = m_mapNamesTypesCommands;

	it = map.find(typeObj);
	if (it != map.end()) {
		name = map[typeObj];
	}
	return name;
}

TypeCommand SceneSerialize::GetTypeCommands(string name) {

	TypeCommand result;
	map <string, TypeCommand> ::iterator it;
	auto map = m_mapTypesCommands;

	it = map.find(name);
	if (it != map.end()) {
		result = map[name];
	}
	return result;
}
//--------------------------------------- Type Layer
void SceneSerialize::AddNextType(TypeLayer type, string Name) {
	m_mapNamesTypesLayers.insert(std::pair<TypeLayer, string>(type, Name));
	m_mapTypesLayers.insert(std::pair<string, TypeLayer>(Name, type));
}

string SceneSerialize::GetNameType(TypeLayer typeObj) {

	string name = "Error";
	map <TypeLayer, string> ::iterator it;
	auto map = m_mapNamesTypesLayers;

	it = map.find(typeObj);
	if (it != map.end()) {
		name = map[typeObj];
	}
	return name;
}

TypeLayer SceneSerialize::GetTypeLayer(string name) {

	TypeLayer result;
	map <string, TypeLayer> ::iterator it;
	auto map = m_mapTypesLayers;

	it = map.find(name);
	if (it != map.end()) {
		result = map[name];
	}
	return result;
}

string FormatEmptyValue(string value) {
	if (value == STR_EMPTY)
		return std::string();
	return value;
}

string FormatFieldName(string value) {
	
	if (value.size() != 0 && value[value.size() - 1] == ':')
		value.erase(value.end() - 1);
	return value;
}

//----------------------------------------