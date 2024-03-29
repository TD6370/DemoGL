#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>

using std::string;
using std::map;
using std::shared_ptr;

class ObjectData;

class BaseShell
{
private:
	//shared_ptr<ObjectData> m_headItem
public:
	string Name;
	int Index =-1;
		
	int RootObjIndex = -1;
	int CaptionObjIndex = -1;
	int HeadIndexList = -1;
	
	shared_ptr<ObjectData> RootObj;
	shared_ptr<ObjectData> CaptionObj;
	shared_ptr<ObjectData> HeadObj;
	shared_ptr<ObjectData> SelectItemObj;

	//Child shapes
	map <string, int> Components = map <string, int>{};
	void AddComponent(string name, int value);
	int ComponentIndex(string name);

	BaseShell(string name, int rootObjIndex) : RootObjIndex(rootObjIndex), Name(name) {};

	~BaseShell();
};


//class ShellButtonGUI : BaseShell
//{
//	ShellButtonGUI();
//
//	//~ShellButtonGUI();
//};

