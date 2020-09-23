#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


GLuint GenShader(const GLchar* pathShader, bool isVertex);
GLuint ProgramConfig(GLuint vertShader, GLuint fragShader);


