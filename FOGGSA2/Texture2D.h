#pragma once
#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <Windows.h> //Required for OpenGL on Windows
#include <gl/GL.h> //OpenGL
#include <gl/GLU.h> //OpenGL Utilities
#include "GL\freeglut.h" //FreeGLUT Library

class Texture2D
{
public:
	~Texture2D();

	bool LoadTextureBMP(const char* texture_file_name);

	GLuint GetID() const { return _ID; }
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
private:
	GLuint _ID;
	int _width, _height;
};

#endif