#include "Corridor.h"

Corridor::Corridor(Mesh* mesh, Texture2D* texture, Vector3 position) : SceneObject(mesh, texture, position, _rotation, _scale) {
	SetPosition(position);
	SetRotation(Vector3{ 0.0,0.0,0.0 });
	SetScale(Vector3{ 1.1,1.0,1.0 });
	_mesh = mesh;
	_texture = texture;
}

void Corridor::Update(const bool* key) {
	// Lock Y-rotation and scale
	_rotation.y = 90.0f;
	SetScale(Vector3{ 1.0f, 1.25f, 1.0f });

	if (_travelSpeed != nullptr)
	{
		_position.z += *_travelSpeed;
	}

	// Return to the start outside of player's view once far enough behind it.
	if (_position.z >= 0.0f)
	{
		_position.z = (-20 * 30)+(_position.z);
	}
}

void Corridor::Draw() {
	DrawMesh();
}