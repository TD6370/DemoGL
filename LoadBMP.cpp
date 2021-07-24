#include "LoadBMP.h"
#include <iostream>

unsigned char* LoadBmp(std::string p_filePath, unsigned int* width, unsigned int* height) {

	const char* filePath = p_filePath.c_str();
	unsigned char header[54]; // ������ BMP ���� ���������� � 54�������� ���������
	unsigned int dataPos; // ������� � ����� ��� ���� ������ ����������
	//unsigned int width, height;
	unsigned int imageSize;   // = ������*������*3
	// ���� RGB ������
	unsigned char* data;

	// ��������� ����
	FILE* file;
	fopen_s(&file, filePath, "rb");
	if (!file)
	{ 
		printf("Image could not be opened\n"); 
		return 0; 
	}

	if (fread(header, 1, 54, file) != 54) { // � ��� ��������, ���� �� ������ ��������� 54 �����
		printf("Not a correct BMP file\n");
		return 0;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	// ������ int �� ������� ������
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	*width = *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);

	// � ��������� BMP ������ ��� ������ ����������, ������� �� � ������� ����
	if (imageSize == 0)    imageSize = *width * *height * 3; // 3 : ���� ���� �� ������ Red, Green, Blue ����������
	if (dataPos == 0)      dataPos = 54; // ��� ������������� ���������, � �� ����, ������ �������� ������

	// ������� �����
	data = new unsigned char[imageSize];

	// ������ ������ �� ����� � �����
	fread(data, 1, imageSize, file);
	//������ ��� ������ � ������, � ����� ������� ����
	fclose(file);

	return data;
}
