#include "MeshLoader.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace MeshLoader
{
	Mesh* MeshLoader::LoadOBJ(char* obj_path) {
		Mesh* mesh = new Mesh();
		
		ifstream in_file;
		in_file.open(obj_path);
		if (!in_file.good())
		{
			cerr << "Can't open OBJ file " << obj_path << endl;
			delete mesh;
			return nullptr;
		}

		// Temporary storage of mesh data read from file, as the total count of each thing will not be known until the file has been read through.
		vector<GLfloat> temp_vertices; temp_vertices.clear();
		vector<GLfloat> temp_tex_coords; temp_tex_coords.clear();
		vector<GLushort> temp_indices; temp_indices.clear();
		vector<float> temp_normals; temp_normals.clear();

		while (!in_file.eof())
		{
			std::string current_line;
			in_file >> current_line;

			if (current_line == "vt") // Texture Co-ordinates found
			{
				while (in_file.peek() != '\n')
				{
					float in_val;
					in_file >> in_val;
					temp_tex_coords.push_back(in_val);
				}
			}
			else if (current_line == "vn") { // Vertex Normal found
				while (in_file.peek() != '\n')
				{
					float in_normal;
					in_file >> in_normal;
					temp_normals.push_back(in_normal);
				}
			}
			else if (current_line == "v" ) { // Vertices found
				while (in_file.peek() != '\n')
				{
					float in_vertice;
					in_file >> in_vertice;
					temp_vertices.push_back(in_vertice);
				}
			}
			else if (current_line == "f") { // Indices found
				while (in_file.peek() != '\n')
				{
					int in_indice;
					in_file >> in_indice;
					// Skip over the slash in the file if the obj file has been formatted to seperate indices this way
					if (in_file.peek() == '/') {
						in_file.ignore(1);
					}
					temp_indices.push_back(in_indice);
				}
			}
			else if (current_line == "mtllib") { // Associated .mtl file has been found.
				// Get the file name as it is listed in the .obj file
				std::string matpath;
				in_file >> matpath;

				// Determine if the original model is in any extra folders that need to be added to the path in code.
				std::string relative_path = obj_path;
				if (relative_path.rfind('/') != relative_path.npos) // if a slash was found, indicating a relative folder to be reading the material from.
				{
					relative_path.resize(relative_path.rfind('/')+1); // Shrink to the last slash in the OBJ path, so that the material path read can be appended.
					relative_path.append(matpath);
				}
				else {
					relative_path = matpath; // There are no relative folders, and the .mtl path can be passed through as-is.
				}

				// Pass the final path to the mesh instance, which can be used to determine material properties later if the path noted in the file exists.
				mesh->MaterialPath = relative_path;
			}
		}

		// Prepare the final amount of each value to be passed into the mesh now that we've read the whole file.
		mesh->IndexCount = temp_indices.size();
		mesh->Indices = new GLushort[mesh->IndexCount];

		mesh->VertexCount = temp_indices.size() / 3;
		mesh->Vertices = new Vertex[mesh->VertexCount];

		mesh->TexCoordCount = temp_indices.size() / 3;
		mesh->TexCoords = new TexCoord[mesh->TexCoordCount];
		
		mesh->NormalCount = temp_indices.size() / 3;
		mesh->Normals = new Vector3[mesh->NormalCount];

		// This current implementation has to act on the assumption that all meshes read in are triangulated (no polygons, only traingles - AKA 3 points per face)
		// If a mesh in this current state uses a polygon (4 points), then it will not be displayed correctly.
		for (int i = 0; i < temp_indices.size() / 3; i++)
		{
			// Obj files starts counting indices from 1 and not 0. Decrements are required otherwise subscripting out of range could cause a crash.
			int v_index = temp_indices[(i * 3)];v_index -= 1;
			int vt_index = temp_indices[(i * 3) + 1]; vt_index -= 1;
			int vn_index = temp_indices[(i * 3) + 2]; vn_index -= 1;

			// Each set of indices in an obj points to the following per set:
			// vertice index / tex-coord index / normal index
			mesh->Vertices[i].x = temp_vertices[(v_index*3)];
			mesh->Vertices[i].y = temp_vertices[(v_index*3)+1];
			mesh->Vertices[i].z = temp_vertices[(v_index*3)+2];

			mesh->TexCoords[i].u = temp_tex_coords[(vt_index*2)];
			mesh->TexCoords[i].v = temp_tex_coords[(vt_index*2)+1];

			mesh->Normals[i].x = temp_normals[(vn_index*3)];
			mesh->Normals[i].y = temp_normals[(vn_index*3)+1];
			mesh->Normals[i].z = temp_normals[(vn_index*3)+2];

			mesh->Indices[i*3] = temp_indices[i*3]-1;
			mesh->Indices[(i*3)+1] = temp_indices[(i*3)+1]-1;
			mesh->Indices[(i*3)+2] = temp_indices[(i*3)+2]-1;
		}

		in_file.close();
		return mesh;
	}

	Material* MeshLoader::LoadMTL(char* mtl_path) {
		Material* mat = new Material();

		ifstream in_file;
		in_file.open(mtl_path);
		if (!in_file.good())
		{
			cerr << "Can't open MTL file " << mtl_path << endl;

			// Pass back a fallback material, otherwise this is going to be called every frame for each SceneObject that doesn't have a valid material.
			mat->ambient.x = 0.8; mat->ambient.y = 0.8; mat->ambient.z = 0.05; mat->ambient.w = 1.0;
			mat->diffuse.x = 0.8; mat->diffuse.y = 0.8; mat->diffuse.z = 0.8; mat->diffuse.w = 1.0;
			mat->specular.x = 0.1; mat->specular.y = 0.1; mat->specular.z = 0.1; mat->specular.w = 0.1;
			mat->shininess = 100.0f;
			return mat;
		}

		while (!in_file.eof())
		{
			std::string current_line;
			in_file >> current_line;

			if (current_line == "Ka") //Ambient Color values found (3 values, 0-1)
			{
				in_file >> mat->ambient.x;
				in_file >> mat->ambient.y;
				in_file >> mat->ambient.z;

				// If there isn't a 4th ambient value, then a placeholder is passed in, as it is assumed to be opaque if serialized that way.
				if (in_file.peek() == '\n')
				{
					mat->ambient.w = 1.0;
				}
			}
			else if (current_line == "Kd") //Diffuse values found (3 values, 0-1)
			{
				in_file >> mat->diffuse.x;
				in_file >> mat->diffuse.y;
				in_file >> mat->diffuse.z;

				// If there isn't a 4th value, then a placeholder is passed in, as it is assumed to be opaque if serialized that way.
				if (in_file.peek() == '\n')
				{
					mat->diffuse.w = 1.0;
				}
			}
			else if (current_line == "Ks") //Specular values found (3 values, 0-1)
			{
				in_file >> mat->specular.x;
				in_file >> mat->specular.y;
				in_file >> mat->specular.z;
				// If there isn't a 4th value, then a placeholder is passed in, as it is assumed to be opaque if serialized that way.
				if (in_file.peek() == '\n')
				{
					mat->specular.w = 1.0;
				}
			}
			else if (current_line == "Ns") //Specular Highlights / Shininess values found (1 value, 0-1000)
			{
				in_file >> mat->shininess;
			}
		}

		in_file.close();

		return mat;
	}
}