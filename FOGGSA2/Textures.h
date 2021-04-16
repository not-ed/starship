#pragma once
#ifndef TEXTURES_H
#define TEXTURES_H

class Texture2D;

namespace Texture {
	enum ID {
		ROCK,
		SHIP,
		SHIP_DESTROYED,
		CRATE,
		BARREL,
		GLOBE,
		Count
	};
}

class TextureAtlas
{
public:
	TextureAtlas();
	~TextureAtlas();

	void Shutdown();

	static TextureAtlas* Instance();

	Texture2D* GetTexture(Texture::ID id);

private:
	static TextureAtlas* _instance;
	Texture2D* _textures[Texture::ID::Count];
};

#endif