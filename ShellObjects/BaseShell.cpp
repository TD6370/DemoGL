#include "BaseShell.h"

//BaseShell::BaseShell() {
//
//}


BaseShell::~BaseShell() {

}

void BaseShell::AddComponent(string name, int value) {
	Components.insert(std::pair<string, int>(name, value));
}

int BaseShell::ComponentIndex(string name)
{
	map <string, int> ::iterator it;
	it = Components.find(name);
	if (it == Components.end())
		return -1;
	return Components[name];
}

//void BaseShell::FillComponents() {
//	//NextItemShellIndex
//}

//void BaseShell::AddItem(ObjectData* nextItem)
//{
//	nextItem->NextItem = HeadItem;
//	HeadItem = nextItem;
//}


//ShellButtonGUI::~ShellButtonGUI() {
//
//}


//ShellButtonGUI::ShellButtonGUI()
//{
//
//}