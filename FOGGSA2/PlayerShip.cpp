#include "PlayerShip.h"
#include <iostream>

PlayerShip::PlayerShip(Mesh* mesh, Texture2D* texture, Vector3 position) : SceneObject(mesh, texture, position, _rotation, _scale) {
	SetPosition(position);
	SetRotation(Vector3{ 0.0,180.0,0.0 });
	SetScale(Vector3{ .5,.5,.5 });
	_mesh = mesh;
	_texture = texture;
	_collisionRadius = 1.0f;
	_startingPosition = _position;
	_startingRotation = _rotation;
}

void PlayerShip::Draw() {
	DrawMesh();
}

void PlayerShip::Update(const bool* key) {
	// Fly towards ground if dead.
	if (!alive)
	{
		if (_position.y > 1.45f)
		{
			_position.y -= .33f;
			_rotation.x -= 1.4f;
			if (_position.y < 1.45f) {
				_position.y = 1.45f;
			}
		}
	}
	else {
		if (key['A'] || key['a'])
		{
			_position.x -= _moveSpeed;
		}

		if (key['D'] || key['d'])
		{
			_position.x += _moveSpeed;
		}

		if (key['W'] || key['w'])
		{
			_position.y += _moveSpeed / 2;
		}

		if (key['S'] || key['s'])
		{
			_position.y -= _moveSpeed / 2;
		}

		if (key['t']) { Kill(); }
	}
}

void PlayerShip::Kill() {
	if (alive == true)
	{
		alive = false;
		SetTexture(TextureAtlas::Instance()->GetTexture(Texture::ID::SHIP_DESTROYED));
	}
}

void PlayerShip::Reset() {
	alive = true;
	_position = _startingPosition;
	_rotation = _startingRotation;
	SetTexture(TextureAtlas::Instance()->GetTexture(Texture::ID::SHIP));
}