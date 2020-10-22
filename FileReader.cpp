#include "FileReader.h"

#include <iostream>
#include <fstream>
#include <string>

unsigned long getFileLength(std::ifstream& file)
{
	if (!file.good()) return 0;

	unsigned long pos = file.tellg();
	file.seekg(0, std::ios::end);
	unsigned long len = file.tellg();
	file.seekg(std::ios::beg);

	return len;
}

const char* GetReadFile(const char* filePath) {
	//std::string readFile(const char* filePath, unsigned long* len) {

	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return NULL;
	}

	//*len = getFileLength(fileStream);

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content.c_str(); 
}


std::string GetReadFile2(const char* filePath) {
	//std::string readFile(const char* filePath, unsigned long* len) {

	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	//*len = getFileLength(fileStream);

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void WriteFile(std::string filePath, std::string data, std::string nameFile) {

	if(filePath.length() ==0)
		filePath = "F:\\Projects\\OpenGl\\DemoGL\\Debug\\debug_" + nameFile + ".txt";

	std::ofstream out; 
	out.open(filePath); //, std::ios_base::
	const char* dataC = data.c_str();
	if (out.is_open())
	{
		out << dataC << std::endl;
	}
	out.close();
}

