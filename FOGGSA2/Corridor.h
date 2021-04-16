#pragma once
#ifndef CORRIDOR_H
#define CORRIDOR_H

#include "SceneObject.h"

class Corridor : public SceneObject
{
public:
	Corridor(Mesh* mesh, Texture2D* texture, Vector3 position);

	void Draw();
	void Update(const bool* key);

	// Set float pointer to use as basis for travel speed.
	void SetTravelSpeed(float* speed) { _travelSpeed = speed; }
private:
	// Pointer to a float which dictates how fast the corridor travels. Ideally from the level it is in.
	float* _travelSpeed = nullptr;
};

#endif 