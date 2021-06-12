#pragma once

#include <string>
#include <iostream>
#include <map>

using std::string;
using std::map;

string GetFile(const char* path);

string Trim(string path);

bool Str_bool(std::string const& s);

bool IsMapContains_StrInt(map <string, int> inMap, string key);
