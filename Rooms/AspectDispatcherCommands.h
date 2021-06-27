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
//struct CommandParams;

class AspectDispatcherCommands : public SceneRoom
{
public:
	vector<CommandParams*> SceneCommands;
	//vector<CommandParams> SceneCommands;

	AspectDispatcherCommands(string Name, SceneConstruction* sceneConstructor)
		: SceneRoom(Name, sceneConstructor) {};

	void Init();
	void Config();
	void Work();
};


