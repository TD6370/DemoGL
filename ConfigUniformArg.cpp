
#include "ConfigUniformArg.h"

#include <iostream>
#include <sstream>

ConfigUniform::ConfigUniform() {};

ConfigUniform::~ConfigUniform() {};

ConfigUniform::ConfigUniform(GLuint shaderProgram)
{
	m_shaderProgram = shaderProgram;
	m_viewID = glGetUniformLocation(shaderProgram, "VIEW");
	m_modelID = glGetUniformLocation(shaderProgram, "MODEL");
	m_matrixID = glGetUniformLocation(shaderProgram, "MVP");
	m_transformLocID = glGetUniformLocation(shaderProgram, "transform");
}


void ConfigUniform::SetMVP(mat4 MVP)
{
	//MVP param
	glUniformMatrix4fv(m_matrixID, 1, GL_FALSE, &MVP[0][0]);
}

void ConfigUniform::SetView(mat4 View)
{
	//VIEW param
	glUniformMatrix4fv(m_viewID, 1, GL_FALSE, &View[0][0]);
}

void ConfigUniform::SetModel(mat4 Model)
{
	//MODEL param
	glUniformMatrix4fv(m_modelID, 1, GL_FALSE, &Model[0][0]);
}

void ConfigUniform::SetVectorsParams(vec3 vectorsParams[10]) {

	if (m_filterVectorsID == 0)
		return;

	//std::vector vector1;

	//std::vector vector1;// = new std::vector[10]; // _kernel_size==16
	//std::vector vector1;

	//glUniform3fv(m_filterVectorsID, 10, &positionLight[]);
	//glUniform3fv(glGetUniformLocation(m_shaderProgram, "filterVectors[0]"), 1, vectorsParams[0]);

	//glUniform3fv(m_filterVectorsID, 10, vectorsParams)
	int p_size = 10;
	int size = p_size * 3;
	//int size = 10 * 3;
	//float* setVectorsParams = new float[size];
	GLfloat setVectorsParams[30];
	int next = 0;
	while (next < size) {
		int indV = (int)(next / 3);
		setVectorsParams[next] = vectorsParams[indV].x;
		next++;
		setVectorsParams[next] = vectorsParams[indV].y;
		next++;
		setVectorsParams[next] = vectorsParams[indV].z;
		next++;
	};
	glProgramUniform3fv(m_shaderProgram, m_filterVectorsID, size, setVectorsParams);
}

void ConfigUniform::SetPositionLight(vec3 positionLight) {
	if (m_lightPosID == 0)
		return;

	glUniform3f(m_lightPosID, positionLight.x, positionLight.y, positionLight.z);
}

void ConfigUniform::SetColor(vec3 color, bool isRnd) {

	if (m_setColorID == 0)
		return;

	// update the uniform color
	//float colorValue = sin(timeValue) / 2.0f + 0.5f;
	//float colorValue = sin(timeValue)*cos(timeValue) / 2.0f + 0.5f;
	//glUniform3f(vertexColorLocation, 0.0f, greenValue, 0.0f);
	//glUniform3f(vertexColorLocation, colorValue - 0.3f, colorValue, colorValue + 0.2f);
	//glUniform3f(vertexColorLocation, sin(timeValue), cos(timeValue + 1.0f), sin(timeValue + 0.5f));
	//glUniform3f(vertexColorLocation, sin(timeValue), cos(timeValue*0.5f), sin(timeValue * 1.5f));
	if (isRnd) {
		float timeValue = glfwGetTime();
		glUniform3f(m_setColorID, sin(timeValue), 0.0f, cos(timeValue));
	}
	else {
		glUniform3f(m_setColorID, color.x, color.y, color.z);
	}
}

void ConfigUniform::SetPositionMouse(vec3 m_positionCursorModel) {
	if (m_setPosMouseID == 0)
		return;

	glUniform3f(m_setPosMouseID, m_positionCursorModel.x, m_positionCursorModel.y, m_positionCursorModel.z);
}

void ConfigUniform::SetParamCase(GLfloat paramCase) {
	if (m_paramCaseID != 0)
		glUniform1f(m_paramCaseID, paramCase);

	if (m_vertexTimeID != 0) {
		float timeValue = glfwGetTime();
		glUniform1f(m_vertexTimeID, timeValue);
	}
}

void ConfigUniform::SetParamValue(GLint paramValue) {
	if (m_paramValueID != 0){
		glUniform1i(m_paramValueID, paramValue);
	}
}

void ConfigUniform::SetStartTime(GLfloat timeValue) {

	if (m_startTimeID != 0) {
		glUniform1f(m_startTimeID, timeValue);
	}
}


void ConfigUniform::SetTransform(mat4* trans)
{
	if (m_transformLocID == 0)
		return;

	glUniformMatrix4fv(m_transformLocID, 1, GL_FALSE, glm::value_ptr(*trans));
}

void ConfigUniform::SetMessage(string message)
{
	if (m_messageID == 0)
		return;

	GLint messageRsult[50];
	int i = 0;

	for (char& c : message) {
		messageRsult[i] = int(c);
	}

	glUniform1iv(m_messageID, 50, messageRsult);
}

void ConfigUniform::SetWidth(GLfloat width) {
	if (m_widthID == 0)
		return;
	
	////TEST
	//string w = std::to_string(width);
	//if (m_debug1 != w) {
	//	m_debug1 = w;
	//	std::cout << " W: " << width << "\n";
	//}

	glUniform1f(m_widthID, width);
}

void ConfigUniform::SetHeight(GLfloat height) {
	if (m_heightID == 0)
		return;
	
	////TEST
	//string h = std::to_string(height);
	//if (m_debug2 != h) {
	//	m_debug2 = h;
	//	std::cout << " H: " << height << "\n";
	//}

	glUniform1f(m_heightID, height);
}

void ConfigUniform::SetPosMove(vec3 posMove) {
	if (m_posMoveID == 0)
		return;

	glUniform3f(m_posMoveID, posMove.x, posMove.y, posMove.z);
}

void ConfigUniform::SetPosMoveSize(vec3 posMoveS) {
	if (m_posMoveSizeID == 0)
		return;

	glUniform3f(m_posMoveSizeID, posMoveS.x, posMoveS.y, posMoveS.z);
}
//------------

void ConfigUniform::Init()
{
	m_setColorID = glGetUniformLocation(m_shaderProgram, "setColor");
	m_setPosMouseID = glGetUniformLocation(m_shaderProgram, "setPosMouse");
	m_paramCaseID = glGetUniformLocation(m_shaderProgram, "paramCase");
	m_vertexTimeID = glGetUniformLocation(m_shaderProgram, "fTime");
	m_lightPosID = glGetUniformLocation(m_shaderProgram, "lightPos");
	m_filterVectorsID = glGetUniformLocation(m_shaderProgram, "filterVectors");

}

void ConfigUniform::InitBox()
{
	m_widthID = glGetUniformLocation(m_shaderProgram, "width");
	m_heightID = glGetUniformLocation(m_shaderProgram, "height");
	m_posMoveID = glGetUniformLocation(m_shaderProgram, "posMove");
	m_posMoveSizeID = glGetUniformLocation(m_shaderProgram, "posMoveSize");
	m_paramValueID = glGetUniformLocation(m_shaderProgram, "paramValue");
	m_startTimeID = glGetUniformLocation(m_shaderProgram, "startTimer");
}
