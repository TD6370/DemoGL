#include "LoadBMP.h"
#include <iostream>

unsigned char* LoadBmp(std::string p_filePath, unsigned int* width, unsigned int* height) {

	const char* filePath = p_filePath.c_str();
	unsigned char header[54]; // каждый BMP файл начинается с 54байтного заголовка
	unsigned int dataPos; // Позиция в файле где сами данные начинаются
	//unsigned int width, height;
	unsigned int imageSize;   // = ширина*высота*3
	// Сами RGB данные
	unsigned char* data;

	// Открываем файл
	FILE* file;
	fopen_s(&file, filePath, "rb");
	if (!file)
	{ 
		printf("Image could not be opened\n"); 
		return 0; 
	}

	if (fread(header, 1, 54, file) != 54) { // У нас проблемы, если не смогли прочитать 54 байта
		printf("Not a correct BMP file\n");
		return 0;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Читаем int из массива байтов
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	*width = *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);

	// в некоторых BMP файлах нет полной информации, поэтому мы её добавим сами
	if (imageSize == 0)    imageSize = *width * *height * 3; // 3 : Один байт на каждую Red, Green, Blue компоненты
	if (dataPos == 0)      dataPos = 54; // Тут заканчивается заголовок, и по идее, должны начаться данные

	// Создаем буфер
	data = new unsigned char[imageSize];

	// Читаем данные из файла в буфер
	fread(data, 1, imageSize, file);
	//Теперь все данные в памяти, и можно закрыть файл
	fclose(file);

	return data;
}
