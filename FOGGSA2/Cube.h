#pragma once
#ifndef CUBE_H
#define CUBE_H

#include <Windows.h> //Required for OpenGL on Windows
#include <gl/GL.h> //OpenGL
#include <gl/GLU.h> //OpenGL Utilities
#include "GL\freeglut.h" //FreeGLUT Library
#include "Structures.h"
#include "SceneObject.h"

class Cube : public SceneObject
{
public:
	Cube(Mesh* mesh, Texture2D* texture, float x, float y, float z);

	void Draw();
	void Update();

private:
	GLfloat _rotation;
	Vector3 _position;

	void UpdateMaterial();

	Material* _material;
};

#endif