#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <Windows.h> //Required for OpenGL on Windows
#include <gl/GL.h> //OpenGL
#include <gl/GLU.h> //OpenGL Utilities
#include "GL\freeglut.h" //FreeGLUT Library
#include <string>

class SceneObject;

struct Vector2 {
	float x, y;
};

struct Vector3
{
	float x, y, z;
};

struct Vector4 {
	float x, y, z, w;
};

struct Lighting {
	Vector4 ambient, diffuse, specular, position;
};

struct Material{
	Vector4 ambient, diffuse, specular;
	float shininess;
};

struct Camera {
	Vector3 eye, center, up;
};

struct Color {
	GLfloat r, g, b;
};

struct Vertex {
	GLfloat x, y, z;
};

struct TexCoord {
	GLfloat u, v;
};

struct Mesh {
	Vertex* Vertices;
	Vector3* Normals;
	GLushort* Indices;
	int VertexCount, NormalCount, IndexCount,TexCoordCount;
	TexCoord* TexCoords;
	std::string MaterialPath;
};

struct ListNode {
	SceneObject* object = nullptr;
	ListNode* next = nullptr;
};

#endif