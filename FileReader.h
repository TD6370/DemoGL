//#pragma once
#include <string>

//std::string GetReadFile(const char* filePath);
const char* GetReadFile(const char* filePath);
std::string GetReadFile2(const char* filePath);
void WriteFile(std::string filePath, std::string data, std::string nameFile);
