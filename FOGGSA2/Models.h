#pragma once
#ifndef MODELS_H
#define MODELS_H

#include "Structures.h"
#include "MeshLoader.h"

namespace Model {
	enum ID {
		CORRIDOR,
		SHIP,
		RUBBLE,
		CRATE,
		BARREL,
		GLOBE,
		Count
	};
}

// Singleton class for managing meshes used in game that can be accessed from anywhere in code, reducing memory usage by ensuring only 1 of each mesh exists.
class ModelAtlas
{
public:
	ModelAtlas();
	~ModelAtlas();

	void Shutdown();

	static ModelAtlas* Instance();

	Mesh* GetMesh(Model::ID id);

private:
	static ModelAtlas* _instance;
	Mesh* _models[Model::ID::Count];
};

#endif MODELS_H