#pragma once
#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "Structures.h"
#include "Texture2D.h"
#include "MeshLoader.h"
#include "Textures.h"
#include "Models.h"

class SceneObject
{
public:
	SceneObject(Mesh* mesh, Texture2D* texture);
	SceneObject(Mesh* mesh, Texture2D* texture, Vector3 position, Vector3 rotation, Vector3 scale);
	~SceneObject();

	virtual void Update(const bool* key) {};
	virtual void Draw() { DrawMesh(); };

	void SetTexture(Texture2D* texture) { _texture = texture; }
	void SetMesh(Mesh* mesh);

	void SetPosition(Vector3 pos) { _position = pos; }
	void SetRotation(Vector3 rot) { _rotation = rot; }
	void SetScale(Vector3 scale) { _scale = scale; }

	Vector3 GetPosition() { return _position; }
	Vector3 GetRotation() { return _rotation; }
	Vector3 GetScale() { return _scale; }

	float GetCollisionRadius() { return _collisionRadius; }

protected:
	Mesh* _mesh;
	Texture2D* _texture;
	Material* _material;

	Vector3 _scale = { 1.0f,1.0f,1.0f };
	Vector3 _rotation = { 1.0f, 1.0f, 1.0f };
	Vector3 _position = { 0.0f, 0.0f, 0.0f };

	void DrawMesh();
	void UpdateMeshMaterial();

	float _collisionRadius = 0.0f;
};

#endif