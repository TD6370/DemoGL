#include "SceneRoom.h"
#include "../CoreSettings.h"

#include <fstream>
#include <vector>

using std::string;
using std::vector;
using std::shared_ptr;
using std::string;
//using glm::vec3;

class SceneConstruction;
class ObjectData;
enum TypeCommand;
//struct CommandPack;

class AspectDispatcherCommands : public SceneRoom
{
private:
	const int m_commandPassLimit = 1000;
	int m_commandPassCount = 0;
	TypeCommand m_commandLast = TypeCommand::None;

public:
	vector<CommandPack> ActiveCommands;

	AspectDispatcherCommands(string Name, SceneConstruction* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	~AspectDispatcherCommands();

	void Init();
	void Config();
	void Work();

	void AddCommand(CommandPack command);

	void AddCommand(TypeCommand commandType, int targetIndex = -1, int sourceIndex = -1, string keyOptions = std::string(), int valueOptions = -1);
};

void SetCommand(shared_ptr<ObjectData> obj, TypeCommand commandType, int targetIndex = -1, int sourceIndex = -1, string keyOptions = std::string(), int valueOptions = -1);

void AddCommandOptions(shared_ptr<ObjectData> obj, string keyOptions, int valueOptions);

void UpdateCommandOptions(shared_ptr<ObjectData> obj, string keyOptions, int valueOptions);

void UpdateCommandOptions(ObjectData* obj, string keyOptions, int valueOptions);



