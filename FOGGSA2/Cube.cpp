#include "Cube.h"
#include <fstream>
#include <iostream>

//Vertex* Cube::indexedVertices = nullptr;
//Color* Cube::indexedColors = nullptr;
//GLushort* Cube::indices = nullptr;
//int Cube::numVertices = 0;
//int Cube::numColors = 0;
//int Cube::numIndices = 0;

Cube::Cube(Mesh* mesh, Texture2D* texture, float x, float y, float z) : SceneObject(mesh, texture){
	_rotation = 0.0f;

	_position.x = x;
	_position.y = y;
	_position.z = z;

	_rotation = (rand() % 360) * 1.0f;
}

Cube::~Cube() {
	std::cout << "Cube Dest" << std::endl;
}

void Cube::Draw() {
	if (_mesh->Vertices != nullptr && _mesh->Normals != nullptr && _mesh->Indices != nullptr) {
		glBindTexture(GL_TEXTURE_2D, _texture->GetID());
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
		glNormalPointer(GL_FLOAT, 0, _mesh->Normals);

		glTexCoordPointer(2, GL_FLOAT, 0, _mesh->TexCoords);

		// Material
		UpdateMaterial();
		glMaterialfv(GL_FRONT, GL_AMBIENT, &(_material->ambient.x));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &(_material->diffuse.x));
		glMaterialfv(GL_FRONT, GL_SPECULAR, &(_material->specular.x));
		glMaterialf(GL_FRONT, GL_SHININESS, _material->shininess);

		glPushMatrix();

		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(_rotation, 1.0f, 5.0f, 2.0f);

		//ORIGINAL glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, _mesh->Indices);
		glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);

		glPopMatrix();

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void Cube::Update() {
	_rotation += 0.9f;
	//_position.x = 0.0f;
	//_position.y = 0.0f;
	//_position.z = -2.0f;
	//_position.z += 1.0f;
	//if (_position.z > 100.0f) {
	//	_position.z = -1000.0f;
	//}
}

void Cube::UpdateMaterial() {
	if (_material == nullptr) {
		_material = new Material();
		_material->ambient.x = 0.8; _material->ambient.y = 0.8; _material->ambient.z = 0.5; _material->ambient.w = 1.0;
		_material->diffuse.x = 0.8; _material->diffuse.y = 0.8; _material->diffuse.z = 0.8; _material->diffuse.w = 1.0;
		_material->specular.x = 0.1; _material->specular.y = 0.1; _material->specular.z = 0.1; _material->specular.w = 0.1;
		_material->shininess = 100.0f;
	}
}

//
//bool Cube::Load(char* path) {
//	std::ifstream inFile;
//	inFile.open(path);
//	if (!inFile.good())
//	{
//		std::cerr << "Cannot load '" << path << "'" << std::endl;
//		return false;
//	}
//
//	inFile >> numVertices;
//	indexedVertices = new Vertex[numVertices];
//	for (int i = 0; i < numVertices; i++)
//	{
//		inFile >> indexedVertices[i].x;
//		inFile >> indexedVertices[i].y;
//		inFile >> indexedVertices[i].z;
//	}
//
//	inFile >> numColors;
//	indexedColors = new Color[numColors];
//	for (int i = 0; i < numColors; i++)
//	{
//		inFile >> indexedColors[i].r;
//		inFile >> indexedColors[i].g;
//		inFile >> indexedColors[i].b;
//	}
//
//	inFile >> numIndices;
//	indices = new GLushort[numIndices];
//	for (int i = 0; i < numIndices; i++)
//	{
//		inFile >> indices[i];
//	}
//
//	inFile.close();
//
//	return true;
//}