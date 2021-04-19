#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include "LinkedLists.h"
#include "SceneObject.h"
#include "Structures.h"
#include "Corridor.h"
#include "PlayerShip.h"
#include "Debris.h"
#include "Collision.h"
#include <string>

class Level
{
public:
	Level();
	~Level();

	void Update(const bool* key);
	void Draw();

private:
	LinkedLists* _sceneObjects;
	ListNode* _sceneObjectsHead = nullptr;
	void LoadObjects();
	// Pointer to the player SceneObject in _sceneObjects
	PlayerShip* _player;
	// Pointer to a scene-specific prop
	SceneObject* _earthProp;

	Lighting* _sceneLight;
	void InitializeLights();

	Camera* _camera;
	void SetUpCamera();

	void SpawnDebris();
	float _debrisSpawnFrequency = 0.0f;
	int _startingPropAmount = 20;
	int _debrisCount = 0;

	// The minimum and max bounds that the player ship and debris can travel in.
	Vector2 _horizontalBounds = Vector2{ -8.75f,8.75f };
	Vector2 _verticalBounds = Vector2{ 2.5f,9.25f };

	// Level scroll / Prop travel speed
	float _startingScrollSpeed = 0.45f;
	float _maxScrollSpeed = 1.32f;
	float _scrollSpeed = _startingScrollSpeed;

	int _currentScrollTime = 0;
	// How often to increase scroll speed in seconds.
	float _scrollIncrementRate = 1.64f;
	// How much scroll speed is increased at a time
	float _scrollIncrementAmount = 0.015f;

	// How long the player has been alive, in seconds.
	float _currentScore = 0.0f;

	// Reset level
	void Reset();

	void DrawString(const char* text, Vector3* position, Color* color);
};

#endif