
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <string>
//#include <iostream>
//#include <fstream>
//#include <string>
#include <vector>

using glm::vec3;
using glm::mat4;
using std::vector;

class ConfigUniform
{
private:
	int m_setColorID = 0;
	int m_setPosMouseID = 0;
	int m_paramCaseID = 0;
	int m_vertexTimeID = 0;
	int m_transformLocID = 0; //unsigned int
	int m_viewID = 0;
	int m_modelID = 0;
	int m_matrixID = 0;
	int m_lightPosID = 0;
	int m_filterVectorsID = 0;
	GLuint m_shaderProgram;
public:

	ConfigUniform()
	{
	}

	ConfigUniform(GLuint shaderProgram)
	{
		m_shaderProgram = shaderProgram;
		m_setColorID = glGetUniformLocation(shaderProgram, "setColor");
		m_setPosMouseID = glGetUniformLocation(shaderProgram, "setPosMouse");
		m_paramCaseID = glGetUniformLocation(shaderProgram, "paramCase");
		m_vertexTimeID = glGetUniformLocation(shaderProgram, "fTime");
		m_transformLocID = glGetUniformLocation(shaderProgram, "transform");
		m_viewID = glGetUniformLocation(shaderProgram, "VIEW");
		m_modelID = glGetUniformLocation(shaderProgram, "MODEL");
		m_matrixID = glGetUniformLocation(shaderProgram, "MVP");
		m_lightPosID = glGetUniformLocation(shaderProgram, "lightPos");
		m_filterVectorsID = glGetUniformLocation(shaderProgram, "filterVectors");
	}


	void SetVectorsParams(vec3 vectorsParams[10]) {

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

	void SetPositionLight(vec3 positionLight) {
		glUniform3f(m_lightPosID, positionLight.x, positionLight.y, positionLight.z);
	}

	void SetColor(vec3 color = vec3(0, 0, 0), bool isRnd = false) {

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

	void SetPositionMouse(vec3 m_positionCursorModel) {
		glUniform3f(m_setPosMouseID, m_positionCursorModel.x, m_positionCursorModel.y, m_positionCursorModel.z);
	}

	void SetParamCase(GLfloat paramCase) {

		//glUniform3f(paramCaseID, paramCase);
		glUniform1f(m_paramCaseID, paramCase);

		float timeValue = glfwGetTime();
		glUniform1f(m_vertexTimeID, timeValue);
	}

	void SetTransform(mat4* trans)
	{
		glUniformMatrix4fv(m_transformLocID, 1, GL_FALSE, glm::value_ptr(*trans));
	}

	void SetMVP(mat4 MVP)
	{
		//MVP param
		glUniformMatrix4fv(m_matrixID, 1, GL_FALSE, &MVP[0][0]);
	}

	void SetView(mat4 View)
	{
		//VIEW param
		glUniformMatrix4fv(m_viewID, 1, GL_FALSE, &View[0][0]);
	}

	void SetModel(mat4 Model)
	{
		//MODEL param
		glUniformMatrix4fv(m_modelID, 1, GL_FALSE, &Model[0][0]);
	}
};