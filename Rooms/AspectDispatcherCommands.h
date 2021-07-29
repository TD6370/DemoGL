#include "SceneRoom.h"
#include "../CoreSettings.h"

#include <fstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

using std::string;
using std::vector;
using std::shared_ptr;
using std::string;
using std::map;
using glm::vec4;
//using glm::vec3;

class SceneConstructor;
class ObjectData;
enum TypeCommand;
//struct CommandPack;

class AspectDispatcherCommands : public SceneRoom
{
private:
	const int m_commandPassLimit = 10;// 1000;
	int m_commandPassCount = 0;
	TypeCommand m_commandLast = TypeCommand::None;

public:
	map <string, vector<CommandPack>> StaticListCommand = map <string, vector<CommandPack>>{};
	vector<CommandPack> ActiveCommands;

	AspectDispatcherCommands(string Name, SceneConstructor* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	~AspectDispatcherCommands();

	void Init();
	void Config();
	void Work();

	void LoadStaticCommandList();

	void AddCommand(CommandPack command);

	void AddCommand(TypeCommand commandType, int sourceIndex = -1, int targetIndex = -1, string keyOptions = std::string(), int valueOptions = -1);
	
	void AddCommand(TypeCommand commandType, int sourceIndex = -1, int targetIndex = -1, vector<string> keyOptions = vector<string>(), vector<int> valueOptions = vector<int>(), 
		int valueI = -1, float valueF = -1.0, vec4 valueV4 = vec4(), string valueS = std::string(), bool isLong = false);

	void AddCommand(TypeCommand commandType, int sourceIndex, int targetIndex, int valueI, float valueF = -1.0, vec4 valueV4 = vec4(), string valueS = std::string(), bool isLong = false);

	void DebugReadCommand(CommandPack* command, string name);
};

void SetCommand(shared_ptr<ObjectData> obj, TypeCommand commandType, int targetIndex = -1, int sourceIndex = -1, string keyOptions = std::string(), int valueOptions = -1,
	int valueI = -1, float valueF = 0.0, vec4 valueV4 = vec4(), string valueS = std::string(), string description = std::string());

void SetCommand(shared_ptr<ObjectData> obj, CommandPack command);

void AddCommandOptions(shared_ptr<ObjectData> obj, string keyOptions, int valueOptions);

void UpdateCommandOptions(shared_ptr<ObjectData> obj, string keyOptions, int valueOptions);

void UpdateCommandOptions(ObjectData* obj, string keyOptions, int valueOptions);

void AddCommandInList(vector<CommandPack>& listObjectFiels, string description, TypeCommand  commandType);

