#pragma once
#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "Structures.h"
#include <vector>

namespace MeshLoader
{
	Mesh* LoadOBJ(char* obj_path);
	Material* LoadMTL(char* mtl_path);
};

#endif