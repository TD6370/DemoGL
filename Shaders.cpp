#include "FileReader.h"
#include "Shaders.h"

#define GLEW_STATIC
#include <GL/glew.h>


void TestShaders(GLuint shader, char const* info)
{
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		fprintf(stderr, *info + " shader compilation failed!\n");
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char* log = new char[logLen];
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log);
			fprintf(stderr, "Shader log:\n%s", log);
			delete[] log;
		}
	}
}

GLuint GenShader(const GLchar* pathShader, bool isVertex) {

	const GLchar* typeShader;
	if (isVertex)
		typeShader = "Vertex";
	else
		typeShader = "Fragment";

	GLuint shader;
	if(isVertex)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else
		shader = glCreateShader(GL_FRAGMENT_SHADER);

	if (0 == shader)
	{
		fprintf(stderr, "Error creating {0} shader.\n", typeShader);
		exit(EXIT_FAILURE);
	}

	//const char* shaderSrc = GetReadFile(pathShader);
	std::string shaderSrc1 = GetReadFile2(pathShader);
	const char* shaderSrc = shaderSrc1.c_str();

	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);
	
	TestShaders(shader, typeShader);

	return shader;
}

GLuint ProgramConfig(GLuint vertShader, GLuint fragShader)
{
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		//-----------------------------------
		//glGetProgramInfoLog
		char temp[1024];
		glGetProgramInfoLog(shaderProgram, 1024, 0, temp);
		fprintf(stderr, "Failed to link program:\n%s\n", temp);
		glDeleteProgram(shaderProgram);
		exit(EXIT_FAILURE);
	}
	//remove shaders
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	return shaderProgram;
}
