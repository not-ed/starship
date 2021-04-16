#include "Textures.h"
#include "Texture2D.h"

TextureAtlas* TextureAtlas::_instance = nullptr;

TextureAtlas::TextureAtlas() {
	_textures[Texture::ID::ROCK] = new Texture2D();
	_textures[Texture::ID::SHIP] = new Texture2D();
	_textures[Texture::ID::SHIP_DESTROYED] = new Texture2D();
	_textures[Texture::ID::CRATE] = new Texture2D();
	_textures[Texture::ID::BARREL] = new Texture2D();
	_textures[Texture::ID::GLOBE] = new Texture2D();

	_textures[Texture::ID::ROCK]->LoadTextureBMP("assets/corridor.bmp");
	_textures[Texture::ID::SHIP]->LoadTextureBMP("assets/spaceship.bmp");
	_textures[Texture::ID::SHIP_DESTROYED]->LoadTextureBMP("assets/spaceship_destroyed.bmp");
	_textures[Texture::ID::CRATE]->LoadTextureBMP("assets/crate.bmp");
	_textures[Texture::ID::BARREL]->LoadTextureBMP("assets/barrel.bmp");
	_textures[Texture::ID::GLOBE]->LoadTextureBMP("assets/globe.bmp");
}

TextureAtlas::~TextureAtlas() {
	delete _textures[Texture::ID::ROCK];
	delete _textures[Texture::ID::SHIP];
	delete _textures[Texture::ID::SHIP_DESTROYED];
	delete _textures[Texture::ID::CRATE];
	delete _textures[Texture::ID::BARREL];
	delete _textures[Texture::ID::GLOBE];
}

void TextureAtlas::Shutdown() {
	if (_instance != nullptr)
	{
		delete _instance;
	}
}

TextureAtlas* TextureAtlas::Instance() {
	if (_instance == nullptr)
	{
		_instance = new TextureAtlas();
	}

	return _instance;
}

Texture2D* TextureAtlas::GetTexture(Texture::ID id) {
	if (id != Texture::ID::Count) {
		return _textures[id];
	}
	else {
		return nullptr;
	}
}