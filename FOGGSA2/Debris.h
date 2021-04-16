#pragma once
#ifndef DEBRIS_H
#define DEBRIS_H

#include "SceneObject.h"

class Debris : public SceneObject
{
public:
	Debris(Mesh* mesh, Texture2D* texture, Vector3 position);

	void Draw();
	void Update(const bool* key);

	// Set float pointer to use as basis for travel speed.
	void SetTravelSpeed(float* speed) { _travelSpeed = speed; }

	// Set minimum and maximum horizontal/vertical bounds that object can exist in.
	void SetBounds(Vector2 horizontal, Vector2 vertical) { _horizontalBounds = horizontal; _verticalBounds = vertical; }
private:
	// The minimum and maximum bounds that the object can exist in on the x and y axis to keep it within the level's visible boundaries.
	Vector2 _horizontalBounds;
	Vector2 _verticalBounds;

	// Pointer to a float which dictates how fast the corridor travels. Ideally from the level it is in.
	float* _travelSpeed = nullptr;

	// The recorded z-position of the debris upon creation, which is used to allow it to cycle around to the start when travelling past the player's view.
	float _startingPointZ;
	// Randomize x and y position and return to initial starting point on the z-axis.
	void ReturnToStart();

	Vector3 _constantRotationSpeed = Vector3{ 0.0f,0.0f,0.0f };

	// Switch to a random texture-mesh pair defined in the function.
	void ChangeToRandomMesh();
};

#endif