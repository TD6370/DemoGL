#define _CRT_SECURE_NO_WARNINGS

#include "LoaderModelObj.h"

#include <iostream>
#include <string>

bool LoadModelObj(const char* filePath,
	std::vector< glm::vec3 >& out_vertices,
	std::vector< glm::vec2 >& out_uvs,
	std::vector< glm::vec3 >& out_normals,
	std::vector<unsigned int>& out_indices,
	bool isGetIndices) {

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	unsigned char* data;
	char infErr[] = "ModelObj";

	FILE* file;
	fopen_s(&file, filePath, "rt");
	if (!file)
	{
		printf("%s could not be opened\n", infErr);
		return false;
	}

	int vertCount = 0;

	while (1) {
		char lineHeader[128];

		// читаем первый символ в файле
		int res = fscanf(file, "%s", lineHeader);
		//int res = fscanf_s(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = Конец файла. Заканчиваем цикл чтения
		else if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			//Если не «v», а «vt», то после них должно быть 2 float числа, поэтому мы создаем glm::vec2 и читаем числа в него.
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			//нормали :
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		
		else if (strcmp(lineHeader, "f") == 0) {
			vertCount++;
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.clear();
			uvIndices.clear();
			normalIndices.clear();

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

			//glm::vec3 tmpIndices(vertexIndex[0], vertexIndex[1], vertexIndex[2]);
			//out_indices.push_back(tmpIndices);

			//--- set indexes
			if (isGetIndices) {
				out_indices.push_back(vertexIndex[0]); //vertexIndex[0] - 1 ????
				out_indices.push_back(vertexIndex[1]); //
				out_indices.push_back(vertexIndex[2]); //
			}
						
			// По каждой вершине каждого треугольника
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {
				//Индекс к позиции вершины : vertexIndices[i]
				unsigned int vertexInd = vertexIndices[i];
				//Поэтому позиция, это temp_vertices[vertexIndex - 1](тут у нас - 1, так как в С++ индексация идет с 0, а в OBJ с 1) :
				glm::vec3 vertex = temp_vertices[vertexInd - 1];

				//--------------------
				//if (std::find(out_vertices.begin(), out_vertices.end(), vertex) != out_vertices.end()) {
				//	/* v contains x */
				//}
				//else {
				//	out_vertices.push_back(vertex);
				//}
				//--------------------
				//И в итоге мы получаем позицию нашей новой вершины
				out_vertices.push_back(vertex);
			}

			for (unsigned int i = 0; i < uvIndices.size(); i++) {
				unsigned int uvInd = uvIndices[i];
				glm::vec2 uv = temp_uvs[uvInd - 1];
				out_uvs.push_back(uv);
			}

			for (unsigned int i = 0; i < normalIndices.size(); i++) {
				unsigned int normalInd = normalIndices[i];
				glm::vec3 normal = temp_normals[normalInd - 1];
				out_normals.push_back(normal);
			}
		}
	}

	fclose(file);

	if (isGetIndices)
		out_vertices = temp_vertices;

	//printf("%s vertCount", vertCount);
	return true;
}



