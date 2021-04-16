#include "MeshLoader.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace MeshLoader
{
	//TODO: Should this be kept? Ask
	//void LoadVertices(ifstream& inFile, Mesh& mesh);
	//void LoadColours(ifstream& inFile, Mesh& mesh);
	//void LoadTexCoords(ifstream& inFile, Mesh& mesh);
	//void LoadIndices(ifstream& inFile, Mesh& mesh);

	//void LoadVertices(ifstream& inFile, Mesh& mesh)
	//{
	//	inFile >> mesh.VertexCount;

	//	if (mesh.VertexCount > 0)
	//	{
	//		mesh.Vertices = new Vertex[mesh.VertexCount];

	//		for (int i = 0; i < mesh.VertexCount; i++)
	//		{
	//			inFile >> mesh.Vertices[i].x;
	//			inFile >> mesh.Vertices[i].y;
	//			inFile >> mesh.Vertices[i].z;
	//		}
	//	}
	//}

	//void LoadColours(ifstream& inFile, Mesh& mesh)
	//{
	//	inFile >> mesh.NormalCount;

	//	if (mesh.NormalCount > 0)
	//	{
	//		mesh.Normals = new Vector3[mesh.NormalCount];

	//		for (int i = 0; i < mesh.NormalCount; i++)
	//		{
	//			inFile >> mesh.Normals[i].x;
	//			inFile >> mesh.Normals[i].y;
	//			inFile >> mesh.Normals[i].z;
	//		}
	//	}
	//}

	//void LoadTexCoords(ifstream& inFile, Mesh& mesh) {
	//	inFile >> mesh.TexCoordCount;

	//	mesh.TexCoords = new TexCoord[mesh.TexCoordCount];

	//	if (mesh.TexCoordCount > 0)
	//	{
	//		for (size_t i = 0; i < mesh.TexCoordCount; i++)
	//		{
	//			inFile >> mesh.TexCoords[i].u;
	//			inFile >> mesh.TexCoords[i].v;
	//		}
	//	}
	//}

	//void LoadIndices(ifstream& inFile, Mesh& mesh)
	//{
	//	inFile >> mesh.IndexCount;

	//	if (mesh.IndexCount > 0)
	//	{
	//		mesh.Indices = new GLushort[mesh.IndexCount];

	//		for (int i = 0; i < mesh.IndexCount; i++)
	//		{
	//			inFile >> mesh.Indices[i];
	//		}
	//	}
	//}

	//Mesh* MeshLoader::Load(char* path)
	//{
	//	Mesh* mesh = new Mesh();

	//	ifstream inFile;

	//	inFile.open(path);

	//	if (!inFile.good())  
	//	{
	//		cerr  << "Can't open mesh file " << path << endl;
	//		delete mesh; //not tested
	//		return nullptr;
	//	}

	//	LoadVertices(inFile, *mesh);
	//	LoadTexCoords(inFile, *mesh);
	//	LoadColours(inFile, *mesh);
	//	LoadIndices(inFile, *mesh);

	//	inFile.close(); //Not tested

	//	return mesh;
	//}

	Mesh* MeshLoader::LoadOBJ(char* obj_path) {
		Mesh* mesh = new Mesh();
		
		ifstream inFile;
		inFile.open(obj_path);
		if (!inFile.good())
		{
			cerr << "Can't open OBJ file " << obj_path << endl;
			delete mesh;
			return nullptr;
		}

		// Temporary storage of mesh data read from file, as the total count of each thing will not be known until the file has been read through.
		vector<GLfloat> tempVertices; tempVertices.clear();
		vector<GLfloat> tempTexCoords; tempTexCoords.clear();
		vector<GLushort> tempIndices; tempIndices.clear();
		vector<float> tempNormals; tempNormals.clear();

		while (!inFile.eof())
		{
			std::string current_line;
			inFile >> current_line;

			if (current_line == "vt") // Texture Co-ordinates found
			{
				while (inFile.peek() != '\n')
				{
					float in_val;
					inFile >> in_val;
					tempTexCoords.push_back(in_val);
				}
			}
			else if (current_line == "vn") { // Vertex Normal found
				while (inFile.peek() != '\n')
				{
					float in_normal;
					inFile >> in_normal;
					tempNormals.push_back(in_normal);
				}
			}
			else if (current_line == "v" ) { // Vertices found
				while (inFile.peek() != '\n')
				{
					float in_vertice;
					inFile >> in_vertice;
					tempVertices.push_back(in_vertice);
				}
			}
			else if (current_line == "f") { // Indices found
				while (inFile.peek() != '\n')
				{
					int in_indice;
					inFile >> in_indice;
					// Skip over the slash in the file if the obj file has been formatted to seperate indices this way
					if (inFile.peek() == '/') {
						inFile.ignore(1);
					}
					tempIndices.push_back(in_indice);
				}
			}
			else if (current_line == "mtllib") { // Associated .mtl file has been found.
				// Get the file name as it is listed in the .obj file
				std::string matpath;
				inFile >> matpath;

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
		mesh->IndexCount = tempIndices.size();
		mesh->Indices = new GLushort[mesh->IndexCount];

		mesh->VertexCount = tempIndices.size() / 3;
		mesh->Vertices = new Vertex[mesh->VertexCount];

		mesh->TexCoordCount = tempIndices.size() / 3;
		mesh->TexCoords = new TexCoord[mesh->TexCoordCount];
		
		mesh->NormalCount = tempIndices.size() / 3;
		mesh->Normals = new Vector3[mesh->NormalCount];

		// This current implementation has to act on the assumption that all meshes read in are triangulated (no polygons, only traingles - AKA 3 points per face)
		// If a mesh in this current state uses a polygon (4 points), then it will not be displayed correctly.
		for (int i = 0; i < tempIndices.size() / 3; i++)
		{
			// Obj files starts counting indices from 1 and not 0. Decrements are required otherwise subscripting out of range could cause a crash.
			int v_index = tempIndices[(i * 3)];v_index -= 1;
			int vt_index = tempIndices[(i * 3) + 1]; vt_index -= 1;
			int vn_index = tempIndices[(i * 3) + 2]; vn_index -= 1;

			// Each set of indices in an obj points to the following per set:
			// vertice index / tex-coord index / normal index
			mesh->Vertices[i].x = tempVertices[(v_index*3)];
			mesh->Vertices[i].y = tempVertices[(v_index*3)+1];
			mesh->Vertices[i].z = tempVertices[(v_index*3)+2];

			mesh->TexCoords[i].u = tempTexCoords[(vt_index*2)];
			mesh->TexCoords[i].v = tempTexCoords[(vt_index*2)+1];

			mesh->Normals[i].x = tempNormals[(vn_index*3)];
			mesh->Normals[i].y = tempNormals[(vn_index*3)+1];
			mesh->Normals[i].z = tempNormals[(vn_index*3)+2];

			mesh->Indices[i*3] = tempIndices[i*3]-1;
			mesh->Indices[(i*3)+1] = tempIndices[(i*3)+1]-1;
			mesh->Indices[(i*3)+2] = tempIndices[(i*3)+2]-1;
		}

		inFile.close();
		return mesh;
	}

	Material* MeshLoader::LoadMTL(char* mtl_path) {
		Material* mat = new Material();

		ifstream inFile;
		inFile.open(mtl_path);
		if (!inFile.good())
		{
			cerr << "Can't open MTL file " << mtl_path << endl;

			// Pass back a fallback material, otherwise this is going to be called every frame for each SceneObject that doesn't have a valid material.
			mat->ambient.x = 0.8; mat->ambient.y = 0.8; mat->ambient.z = 0.05; mat->ambient.w = 1.0;
			mat->diffuse.x = 0.8; mat->diffuse.y = 0.8; mat->diffuse.z = 0.8; mat->diffuse.w = 1.0;
			mat->specular.x = 0.1; mat->specular.y = 0.1; mat->specular.z = 0.1; mat->specular.w = 0.1;
			mat->shininess = 100.0f;
			return mat;
		}

		while (!inFile.eof())
		{
			std::string current_line;
			inFile >> current_line;

			if (current_line == "Ka") //Ambient Color values found (3 values, 0-1)
			{
				inFile >> mat->ambient.x;
				inFile >> mat->ambient.y;
				inFile >> mat->ambient.z;

				// If there isn't a 4th ambient value, then a placeholder is passed in, as it is assumed to be opaque if serialized that way.
				if (inFile.peek() == '\n') 
				{
					mat->ambient.w = 1.0;
				}
			}
			else if (current_line == "Kd") //Diffuse values found (3 values, 0-1)
			{
				inFile >> mat->diffuse.x;
				inFile >> mat->diffuse.y;
				inFile >> mat->diffuse.z;

				// If there isn't a 4th value, then a placeholder is passed in, as it is assumed to be opaque if serialized that way.
				if (inFile.peek() == '\n')
				{
					mat->diffuse.w = 1.0;
				}
			}
			else if (current_line == "Ks") //Specular values found (3 values, 0-1)
			{
				inFile >> mat->specular.x;
				inFile >> mat->specular.y;
				inFile >> mat->specular.z;
				// If there isn't a 4th value, then a placeholder is passed in, as it is assumed to be opaque if serialized that way.
				if (inFile.peek() == '\n')
				{
					mat->specular.w = 1.0;
				}
			}
			else if (current_line == "Ns") //Specular Highlights / Shininess values found (1 value, 0-1000)
			{
				inFile >> mat->shininess;
			}
		}

		inFile.close();

		return mat;
	}
}