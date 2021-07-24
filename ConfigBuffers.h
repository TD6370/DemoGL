#pragma once
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

GLuint InitImage();
GLuint InitVAO();
GLuint InitBuffer();

void SetBufferUV(std::vector< glm::vec2 >& uv_data, GLuint uvBuffer, bool isLoadedIntoMem = false);

void SetNormals(std::vector< glm::vec3 >& normals, GLuint normalbuffer, bool isLoadedIntoMem = false);

void SetImage(unsigned char* data, unsigned int width, unsigned int height, GLuint textureID, int numLink = 0, bool isLoadedIntoMem = false);

void GenBufferColors(std::vector< glm::vec3 >& colors, GLuint colorbufferID, bool isLoadedIntoMem = false);

void GenVertexArrayObject(bool isIndex, 
	std::vector< glm::vec3 > vertices,
	std::vector<unsigned int> indices,
	GLuint VAO,
	GLuint VBO, 
	bool isLoadedIntoMem = false);
