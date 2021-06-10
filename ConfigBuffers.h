#pragma once
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

GLuint InitImage();
GLuint InitUV();
GLuint InitNormals();
GLuint InitVAO();
GLuint InitVBO();
GLuint InitBuffer();

void SetBufferUV(std::vector< glm::vec2 >& uv_data, GLuint uvBuffer);
void SetNormals(std::vector< glm::vec3 >& normals, GLuint normalbuffer);
void SetImage(unsigned char* data, unsigned int width, unsigned int height, GLuint textureID);
void GenBufferColors(std::vector< glm::vec3 >& colors, GLuint colorbufferID);

GLuint GenVertexArrayObject(bool isIndex, 
	std::vector< glm::vec3 > vertices,
	std::vector<unsigned int> indices,
	GLuint VAO,
	GLuint VBO);
