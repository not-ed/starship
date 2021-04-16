#pragma once
#ifndef HELLOGL_H
#define HELLOGL_H

#define REFRESH_RATE 16
#define ASCII_COUNT 128

#include <Windows.h> //Required for OpenGL on Windows
#include <gl/GL.h> //OpenGL
#include <gl/GLU.h> //OpenGL Utilities
#include "GL\freeglut.h" //FreeGLUT Library
#include "GLUTCallbacks.h"
#include <time.h>
#include <random>
#include "Level.h"

class HelloGL
{
public:

	HelloGL(int argc, char* argv[]);
	~HelloGL();
	void InitGL(int argc, char* argv[]);
	void InitObjects();

	void Update();
	void Display();

	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	bool _keyStates[ASCII_COUNT];

private:
	//void DrawString(const char* text, Vector3* position, Color* color);
};

#endif