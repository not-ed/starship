#pragma once
#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "Structures.h"
#include <vector>

namespace MeshLoader
{
	//Mesh* Load(char* path);
	Mesh* LoadOBJ(char* obj_path);
	Material* LoadMTL(char* mtl_path);
};

#endif