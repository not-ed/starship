#include "Debris.h"

Debris::Debris(Mesh* mesh, Texture2D* texture, Vector3 position) : SceneObject(mesh, texture, position, _rotation, _scale) {
	SetPosition(position);
	SetRotation(Vector3{ 0.0,180.0,0.0 });
	SetScale(Vector3{ .5,.5,.5 });
	_mesh = mesh;
	_texture = texture;
	_startingPointZ = position.z;
	_collisionRadius = .55;

	// Determine a constant rotation amount at random.
	_constantRotationSpeed.x = fmod(rand(), .82f);
	_constantRotationSpeed.y = fmod(rand(), .82f);
	_constantRotationSpeed.z = fmod(rand(), .82f);
}

void Debris::Draw() {
	DrawMesh();
}

void Debris::Update(const bool* key) {
	if (_travelSpeed != nullptr)
	{
		_position.z += *_travelSpeed;
	}

	_rotation.x += _constantRotationSpeed.x;
	_rotation.y += _constantRotationSpeed.y;
	_rotation.z += _constantRotationSpeed.z;

	// If behind the player's view.
	if (_position.z >= 0.0f)
	{
		ReturnToStart();
	}
}

void Debris::ReturnToStart() {
	Vector2 new_xy;
	// Get a new random x and y position based on the horizontal/vertical bounds.
	new_xy.x = _horizontalBounds.x + (fmod(rand(),abs(_horizontalBounds.x - _horizontalBounds.y)));
	new_xy.y = _verticalBounds.x + (fmod(rand(), abs(_verticalBounds.x - _verticalBounds.y)));
	SetPosition(Vector3{new_xy.x,new_xy.y,_startingPointZ});

	ChangeToRandomMesh();
}

void Debris::ChangeToRandomMesh() {
	int next_mesh = rand() % 3;
	// Rock/boulder
	if (next_mesh == 0) { SetTexture(TextureAtlas::Instance()->GetTexture(Texture::ID::ROCK)); SetMesh(ModelAtlas::Instance()->GetMesh(Model::ID::RUBBLE)); }
	// Barrel
	if (next_mesh == 1) { SetTexture(TextureAtlas::Instance()->GetTexture(Texture::ID::BARREL)); SetMesh(ModelAtlas::Instance()->GetMesh(Model::ID::BARREL)); }
	// Metal Crate
	if (next_mesh == 2) { SetTexture(TextureAtlas::Instance()->GetTexture(Texture::ID::CRATE)); SetMesh(ModelAtlas::Instance()->GetMesh(Model::ID::CRATE)); }
}