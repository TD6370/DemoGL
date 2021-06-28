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
//struct CommandPack;

class AspectDispatcherCommands : public SceneRoom
{
private:
	const int m_commandPassLimit = 2;
	int m_commandPassCount = 0;

public:
	vector<CommandPack> ActiveCommands;

	AspectDispatcherCommands(string Name, SceneConstruction* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();
};


