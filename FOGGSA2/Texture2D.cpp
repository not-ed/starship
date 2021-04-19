#include "Texture2D.h"
#include <iostream>
#include <fstream>

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &_ID);
}

bool Texture2D::LoadTextureBMP(const char* texture_file_name) {

	std::ifstream in_file;
	in_file.open(texture_file_name, std::ios::binary);
	if (!in_file.good())
	{
		std::cerr << "Can't open texture file " << texture_file_name << std::endl;
		return false;
	}

	// Get the size of the file in bytes from the BMP header
	int file_size = 0;
	in_file.seekg(2, std::ios::beg);
	in_file.read((char*)&file_size, 4);

	// Get the byte that image pixel data starts from in the file.
	in_file.seekg(10, std::ios::beg);
	int image_start = 0;
	in_file.read((char*)&image_start, 4);

	// Calculate the image actual size in bytes
	int image_byte_size = file_size - image_start;

	// Pull bit-depth from file.
	int bits_per_pixel = 0;
	in_file.seekg(28, std::ios::beg);
	in_file.read((char*)&bits_per_pixel,2);
	
	// Get texture width and height
	in_file.seekg(18, std::ios::beg);
	int width = 0;
	in_file.read((char*)&width, 4);
	int height = 0;
	in_file.read((char*)&height, 4);

	char* image_data = nullptr;
	// Only 24 and 32 bit bitmaps will be parsed, as they do not use LUTs.
	if (bits_per_pixel == 24) // 24-bit image (BGR, but no alpha bytes - 1 byte for each channel, 3 total)
	{
		image_data = new char[(width * height) * 3];
		in_file.seekg(image_start, std::ios::beg);
		in_file.read(image_data, image_byte_size);
	}
	else if (bits_per_pixel == 32){ // 32-bit image (BGRA - 1 byte for each channel, 4 total)
		image_data = new char[(width * height) * 4];
		in_file.seekg(image_start, std::ios::beg);
		in_file.read(image_data, image_byte_size);
	}

	in_file.close();

	glGenTextures(1, &_ID); //Get next Texture ID
	glBindTexture(GL_TEXTURE_2D, _ID); //Bind the texture to the ID

	if (bits_per_pixel == 24) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, image_data);
	}
	else if (bits_per_pixel == 32) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image_data);
	}

	// Image data is no longer needed once the texture is bound.
	delete[] image_data;

	return true;
}