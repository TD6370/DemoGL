#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <map>

using std::string;
using std::map;

string GetFile(const char* path);

string Trim(string path);

bool StrToBool(string const& s);

string BoolToStr(bool value);

//bool StrToBool(string str);

bool IsMapContains_StrInt(map <string, int> inMap, string key);

string FormatTypeName(string text);

bool IsCompareF(float a, float b);

