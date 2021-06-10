#include "ConfigBuffers.h"
#include "LoaderModelObj.h"
#include "CreatorModelData.h"

#include <string>
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

GLint VertexAttribNumer_Position = 0;
GLint VertexAttribNumer_Color = 1;
GLint VertexAttribNumer_UV = 2;
GLint VertexAttribNumer_Normals = 3;

GLint TYPE_DRAW = GL_STATIC_DRAW;
//GLint TYPE_DRAW = GL_DYNAMIC_DRAW;
//GLint TYPE_DRAW = GL_STREAM_DRAW;

/*
GLfloat* GenVertices(int* size)
{
	GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	*size = sizeof(vertices) / sizeof(GLfloat);
	return vertices;
}
*/
GLuint InitBuffer()
{
	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	return uvBuffer;
}

GLuint InitImage()
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	return textureID;
}


void SetImage(unsigned char* data, unsigned int width, unsigned int height, GLuint textureID)
{
	// Биндим текстуру, и теперь все функции по работе с текстурами будут работать с этой
	glBindTexture(GL_TEXTURE_2D, textureID);

	int glParam_MAG_FILTER = GL_LINEAR; //GL_NEAREST
	int glParam_MIN_FILTER = GL_LINEAR_MIPMAP_LINEAR; // GL_NEAREST

	// Отправляем картинку в OpenGL текстуру
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Когда картинка будет увеличиваться(нет большей Мипмапы), используем LINEAR фильтрацию
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Когда минимизируем — берем две ближних мипмапы и лиейно смешиваем цвета
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glParam_MAG_FILTER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glParam_MIN_FILTER);

	//repeat texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// И создаем сами мипмапы.
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind texture
	//glBindTexture(GL_TEXTURE_2D, NULL);

	//glBindTexture(GL_TEXTURE_2D, 0); // CLEAR
}

GLuint InitUV()
{
	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	return uvBuffer;
}


void SetBufferUV(std::vector< glm::vec2 >& uv_data, GLuint uvBuffer) {
	//Создайте буфер и заполните его точно так же, как и в предыдущих случаях :
	//GLuint uvBuffer;
	//glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);

	//GL_STATIC_DRAW: данные либо никогда не будут изменяться, либо будут изменяться очень редко;
	//GL_DYNAMIC_DRAW: данные будут меняться довольно часто;
	//GL_STREAM_DRAW: данные будут меняться при каждой отрисовке.
	glBufferData(GL_ARRAY_BUFFER, uv_data.size() * sizeof(glm::vec2), &uv_data[0], TYPE_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, (*uv_data).size() * sizeof(glm::vec2), uv_data[0], GL_STATIC_DRAW);

	//Конфигурирование почти такое же :
	//glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);

	/*
	1 описывает какой аргумент шейдера мы хотим настроить. (Мы хотим специфицировать значение аргумента position, позиция которого была указана следующим образом : layout(location = 0)).
	2 описывает размер аргумента в шейдере.Поскольку мы использовали vec3 то мы указываем 3.
	3 описывает используемый тип данных.Мы указываем GL_FLOAT, поскольку vec в шейдере использует числа с плавающей точкой.
	4 указывает необходимость нормализовать входные данные.Если мы укажем GL_TRUE, то все данные будут расположены между 0 (-1 для знаковых значений) и 1. Нам нормализация не требуется, поэтому мы оставляем GL_FALSE;
	5 называется шагом и описывает расстояние между наборами данных.Мы также можем указать шаг равный 0 и тогда OpenGL высчитает шаг(работает только с плотно упакованными наборами данных).
	6 параметр имеет тип GLvoid * и поэтому требует такое странное приведение типов.Это смещение начала данных в буфере.Если буфер не имеет смещения мы указываем 0
	*/
	glVertexAttribPointer(
		VertexAttribNumer_UV,                                // атрибут. Нет особых причин писать 1, главное, чтобы совпадало со значением в шейдере.
		2,                                // размер
		GL_FLOAT,                         // тип
		GL_FALSE,                         // нормализировано ли?
		0,                                // шаг
		(void*)0                          //смещение в буфере
	);
	glEnableVertexAttribArray(VertexAttribNumer_UV);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // CLEAR
}

GLuint InitNormals() {
	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	return normalbuffer;
}

void SetNormals(std::vector< glm::vec3 >& normals, GLuint normalbuffer)
{
	//GLuint normalbuffer;
	//glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], TYPE_DRAW);

	// Третий атрибутный буфер : нормали
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		VertexAttribNumer_Normals,                                // атрибут
		3,                                // размер
		GL_FLOAT,                        // тип
		GL_FALSE,                        // нормализованный ли?
		0,                                // шаг
		(void*)0                         // смещение в буфере
	);

	glEnableVertexAttribArray(VertexAttribNumer_Normals);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // CLEAR
}

void GenBufferColors(std::vector< glm::vec3 >& colors, GLuint colorbuffer)
{
	//Создайте буфер и заполните его точно так же, как и в предыдущих случаях :
	//glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);

	//GL_STATIC_DRAW: данные либо никогда не будут изменяться, либо будут изменяться очень редко;
	//GL_DYNAMIC_DRAW: данные будут меняться довольно часто;
	//GL_STREAM_DRAW: данные будут меняться при каждой отрисовке.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], TYPE_DRAW);

	//Конфигурирование почти такое же :
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		VertexAttribNumer_Color,                                // атрибут. Нет особых причин писать 1, главное, чтобы совпадало со значением в шейдере.
		3,                                // размер
		GL_FLOAT,                         // тип
		GL_FALSE,                         // нормализировано ли?
		0,                                // шаг
		(void*)0                          //смещение в буфере
	);
	glEnableVertexAttribArray(VertexAttribNumer_Color);

	//4. Отвязываем VAO
	//glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // CLEAR
	//layout(location = 1) in vec3 vertexColor;
}

void GenBufferColors()
{
	// Один цвет на каждую вершину. Числа от фонаря.
	static const GLfloat g_color_buffer_data[] = {
	   0.583f,  0.771f,  0.014f,
	   0.609f,  0.115f,  0.436f,
	   0.327f,  0.483f,  0.844f,
	   0.822f,  0.569f,  0.201f,
	   0.435f,  0.602f,  0.223f,
	   0.310f,  0.747f,  0.185f,
	   0.597f,  0.770f,  0.761f,
	   0.559f,  0.436f,  0.730f,
	   0.359f,  0.583f,  0.152f,
	   0.483f,  0.596f,  0.789f,
	   0.559f,  0.861f,  0.639f,
	   0.195f,  0.548f,  0.859f,
	   0.014f,  0.184f,  0.576f,
	   0.771f,  0.328f,  0.970f,
	   0.406f,  0.615f,  0.116f,
	   0.676f,  0.977f,  0.133f,
	   0.971f,  0.572f,  0.833f,
	   0.140f,  0.616f,  0.489f,
	   0.997f,  0.513f,  0.064f,
	   0.945f,  0.719f,  0.592f,
	   0.543f,  0.021f,  0.978f,
	   0.279f,  0.317f,  0.505f,
	   0.167f,  0.620f,  0.077f,
	   0.347f,  0.857f,  0.137f,
	   0.055f,  0.953f,  0.042f,
	   0.714f,  0.505f,  0.345f,
	   0.783f,  0.290f,  0.734f,
	   0.722f,  0.645f,  0.174f,
	   0.302f,  0.455f,  0.848f,
	   0.225f,  0.587f,  0.040f,
	   0.517f,  0.713f,  0.338f,
	   0.053f,  0.959f,  0.120f,
	   0.393f,  0.621f,  0.362f,
	   0.673f,  0.211f,  0.457f,
	   0.820f,  0.883f,  0.371f,
	   0.982f,  0.099f,  0.879f
	};

	//Создайте буфер и заполните его точно так же, как и в предыдущих случаях :
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);

	//GL_STATIC_DRAW: данные либо никогда не будут изменяться, либо будут изменяться очень редко;
	//GL_DYNAMIC_DRAW: данные будут меняться довольно часто;
	//GL_STREAM_DRAW: данные будут меняться при каждой отрисовке.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	//Конфигурирование почти такое же :
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		VertexAttribNumer_Color,                                // атрибут. Нет особых причин писать 1, главное, чтобы совпадало со значением в шейдере.
		3,                                // размер
		GL_FLOAT,                         // тип
		GL_FALSE,                         // нормализировано ли?
		0,                                // шаг
		(void*)0                          //смещение в буфере
	);
	glEnableVertexAttribArray(VertexAttribNumer_Color);

	//layout(location = 1) in vec3 vertexColor;
}

GLuint InitVAO() {
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	return VAO;
}

GLuint InitVBO() {
	GLuint VBO;
	glGenBuffers(1, &VBO);
	return VBO;
}

GLuint GenVertexArrayObject(bool isIndex, 
	std::vector< glm::vec3 > vertices,
	std::vector<unsigned int> indices,
	GLuint VAO,
	GLuint VBO)
{
	// 1. Привязываем VAO
	glBindVertexArray(VAO);
	// 2. Копируем наш массив вершин в буфер для OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	//glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], TYPE_DRAW);

	glVertexAttribPointer(VertexAttribNumer_Position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(VertexAttribNumer_Position);

	if (isIndex)
	{
		GLuint EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], TYPE_DRAW);
	}

	vertices.clear();
	indices.clear();

	//4. Отвязываем VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // CLEAR

	return VBO;
}