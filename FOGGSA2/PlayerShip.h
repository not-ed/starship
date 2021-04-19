#pragma once
#ifndef PLAYER_SHIP
#define PLAYER_SHIP

#include "SceneObject.h"

class PlayerShip : public SceneObject
{
public:
	PlayerShip(Mesh* mesh, Texture2D* texture, Vector3 position);

	void Draw();
	void Update(const bool* key);

	bool IsAlive() { return _alive; }

	void Kill();

	void Reset();
private:
	bool _alive = true;

	Vector3 _startingPosition;
	Vector3 _startingRotation;

	float _moveSpeed = 0.15f;

};

#endif