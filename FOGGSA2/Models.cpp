#include "Models.h"

ModelAtlas* ModelAtlas::_instance = nullptr;

ModelAtlas::ModelAtlas() {
	_models[Model::ID::CORRIDOR] = MeshLoader::LoadOBJ((char*)"assets/corridor.obj");
	_models[Model::ID::SHIP] = MeshLoader::LoadOBJ((char*)"assets/spaceship.obj");
	_models[Model::ID::RUBBLE] = MeshLoader::LoadOBJ((char*)"assets/rubble.obj");
	_models[Model::ID::CRATE] = MeshLoader::LoadOBJ((char*)"assets/crate.obj");
	_models[Model::ID::BARREL] = MeshLoader::LoadOBJ((char*)"assets/barrel.obj");
	_models[Model::ID::GLOBE] = MeshLoader::LoadOBJ((char*)"assets/globe.obj");
}

ModelAtlas::~ModelAtlas() {
	delete _models[Model::ID::CORRIDOR];
	delete _models[Model::ID::SHIP];
	delete _models[Model::ID::RUBBLE];
	delete _models[Model::ID::CRATE];
	delete _models[Model::ID::BARREL];
	delete _models[Model::ID::GLOBE];
}

void ModelAtlas::Shutdown() {
	if (_instance != nullptr)
	{
		delete _instance;
	}
}

ModelAtlas* ModelAtlas::Instance() {
	if (_instance == nullptr)
	{
		_instance = new ModelAtlas();
	}

	return _instance;
}

Mesh* ModelAtlas::GetMesh(Model::ID id) {
	if (id != Model::ID::Count)
	{
		return _models[id];
	}
	else {
		return nullptr;
	}
}