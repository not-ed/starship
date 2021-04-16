#include "Texture2D.h"
#include <iostream>
#include <fstream>

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &_ID);
}

// TODO: Does this need to be removed? 
// This function is for loading specifically formatted files. For loading TGA or BMP files, check the LoadTextureTGA and LoadTextureBMP
bool Texture2D::Load(char* path, int width, int height) {
	char* tempTextureData; int fileSize; std::ifstream inFile;
	_width = width; _height = height;

	inFile.open(path, std::ios::binary); //Reading file as bits
	if (!inFile.good()) {
		std::cerr << "[!] Cannot open texture '" << path << "'." << std::endl;
		return false;
	}

	inFile.seekg(0, std::ios::end); // Seek to eof
	fileSize = (int)inFile.tellg(); // Get the file's overall size from counting the current position in the file.
	tempTextureData = new char[fileSize];
	inFile.seekg(0, std::ios::beg); // Seek back to start of file.
	inFile.read(tempTextureData, fileSize); // Read all file data up to EOF
	inFile.close();

	glGenTextures(1, &_ID); // Get next texture ID.
	glBindTexture(GL_TEXTURE_2D, _ID); // Bind texture to the ID.
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData); // Specify texture details

	// Image data can be discarded from memory once bound to a texture ID.
	delete[] tempTextureData;
	return true;
}

bool Texture2D::LoadTextureTGA(const char* textureFileName)
{
	int ID;
	char* tempHeaderData = new char[18]; //18 Bytes is TGA Header Size
	char* tempTextureData;
	int fileSize;
	char type, pixelDepth, mode;

	std::ifstream inFile;

	inFile.open(textureFileName, std::ios::binary);
	if (!inFile.good())
	{
		std::cerr << "Can't open texture file " << textureFileName << std::endl;
		return false;
	}

	//18 Bytes is the size of a TGA Header
	inFile.seekg(0, std::ios::beg); //Seek back to beginning of file
	inFile.read(tempHeaderData, 18); //Read in all the data in one go

	inFile.seekg(0, std::ios::end); //Seek to end of file
	fileSize = (int)inFile.tellg() - 18; //Get current position in file - The End, this gives us total file size
	tempTextureData = new char[fileSize]; //Create an new aray to store data
	inFile.seekg(18, std::ios::beg); //Seek back to beginning of file + 18
	inFile.read(tempTextureData, fileSize); //Read in all the data in one go
	inFile.close(); //Close the file

	type = tempHeaderData[2]; //Get TGA Type out of Header - Must be RGB for this to work
	_width = ((unsigned char)tempHeaderData[13] << 8u) + (unsigned char)tempHeaderData[12]; // Find the width (Combines two bytes into a short)
	_height = ((unsigned char)tempHeaderData[15] << 8u) + (unsigned char)tempHeaderData[14]; //Find the height
	std::cout << _width << "," << _height << std::endl;
	pixelDepth = tempHeaderData[16]; // Find the pixel depth (24/32bpp)

	bool flipped = false;
	if ((int)((tempHeaderData[11] << 8) + tempHeaderData[10]) == 0)
		flipped = true;

	//We only support RGB type
	if (type == 2)
	{
		std::cout << textureFileName << " loaded." << std::endl;

		glGenTextures(1, &_ID); //Get next Texture ID
		glBindTexture(GL_TEXTURE_2D, _ID); //Bind the texture to the ID

		mode = pixelDepth / 8;

		//Note that TGA files are stored as BGR(A) - So we need to specify the format as GL_BGR(A)_EXT
		if (mode == 4) {
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, tempTextureData);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, _width, _height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, tempTextureData); // Mip-maps version
		}
		else {
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, tempTextureData);std::cout << "eee";
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, _width, _height, GL_BGR_EXT, GL_UNSIGNED_BYTE, tempTextureData); // Mip-maps version
		}
	}
	
	delete[] tempHeaderData; //We don't need the header memory anymore
	delete[] tempTextureData; //Clear up the data - We don't need this any more

	return true;
}

bool Texture2D::LoadTextureBMP(const char* textureFileName) {

	int ID;

	std::ifstream inFile;
	inFile.open(textureFileName, std::ios::binary);
	if (!inFile.good())
	{
		std::cerr << "Can't open texture file " << textureFileName << std::endl;
		return false;
	}

	// Get the size of the file in bytes from the BMP header
	int fileSize = 0;
	inFile.seekg(2, std::ios::beg);
	inFile.read((char*)&fileSize, 4); 

	// Get the byte that image pixel data starts from in the file.
	inFile.seekg(10, std::ios::beg);
	int imageStart = 0;
	inFile.read((char*)&imageStart, 4);

	// Calculate the image actual size in bytes
	int imageByteSize = fileSize - imageStart;

	// Pull bit-depth from file.
	int bitsPerPixel = 0;
	inFile.seekg(28, std::ios::beg);
	inFile.read((char*)&bitsPerPixel,2);
	
	// Get texture width and height
	inFile.seekg(18, std::ios::beg);
	int width = 0;
	inFile.read((char*)&width, 4);
	int height = 0;
	inFile.read((char*)&height, 4);

	char* imageData = nullptr;
	// Only 24 and 32 bit bitmaps will be parsed, as they do not use LUTs.
	if (bitsPerPixel == 24) // 24-bit image (BGR, but no alpha bytes - 1 byte for each channel, 3 total)
	{
		imageData = new char[(width * height) * 3];
		inFile.seekg(imageStart, std::ios::beg);
		inFile.read(imageData, imageByteSize);
	}
	else if (bitsPerPixel == 32){ // 32-bit image (BGRA - 1 byte for each channel, 4 total)
		imageData = new char[(width * height) * 4];
		inFile.seekg(imageStart, std::ios::beg);
		inFile.read(imageData, imageByteSize);
	}

	inFile.close();

	glGenTextures(1, &_ID); //Get next Texture ID
	glBindTexture(GL_TEXTURE_2D, _ID); //Bind the texture to the ID

	if (bitsPerPixel == 24) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageData);
	}
	else if (bitsPerPixel == 32) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, imageData);
	}

	// Image data is no longer needed once the texture is bound.
	delete[] imageData;

	return true;
}