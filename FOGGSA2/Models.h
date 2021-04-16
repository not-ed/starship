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