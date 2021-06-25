#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

using glm::vec3;
using glm::mat4;
using std::vector;
using std::string;

class ConfigUniform
{
protected:
	int m_setColorID = 0;
	int m_setPosMouseID = 0;
	int m_transformLocID = 0; //unsigned int

	int m_paramCaseID = 0;
	int m_vertexTimeID = 0;
	int m_widthID = 0;
	int m_heightID = 0;
	int m_posMoveID = 0;
	int m_posMoveSizeID = 0;

	int m_lightPosID = 0;
	int m_filterVectorsID = 0;

	int m_viewID = 0;
	int m_modelID = 0;
	int m_matrixID = 0;
	
	int m_messageID = 0;

	string m_debug1 = std::string();
	string m_debug2 = std::string();

	GLuint m_shaderProgram;
public:
	
	ConfigUniform();
	ConfigUniform(GLuint shaderProgram);
	~ConfigUniform();

	void Init();

	void InitBox();

	void SetMVP(mat4 MVP);

	void SetView(mat4 View);

	void SetModel(mat4 Model);

	void SetVectorsParams(vec3 vectorsParams[10]);

	void SetPositionLight(vec3 positionLight);

	void SetColor(vec3 color = vec3(0, 0, 0), bool isRnd = false);

	void SetPositionMouse(vec3 m_positionCursorModel);

	void SetParamCase(GLfloat paramCase);

	void SetTransform(mat4* trans);

	void SetMessage(string message);

	void SetWidth(GLfloat width);

	void SetHeight(GLfloat height);
	
	void SetPosMove(vec3 height);

	void SetPosMoveSize(vec3 height);
	

};
