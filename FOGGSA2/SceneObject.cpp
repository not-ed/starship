#include "SceneObject.h"

SceneObject::SceneObject(Mesh* mesh, Texture2D* texture) {
	_mesh = mesh;
	_texture = texture;
}

SceneObject::SceneObject(Mesh* mesh, Texture2D* texture, Vector3 position, Vector3 rotation, Vector3 scale) {
	_mesh = mesh;
	_texture = texture;
	_position = position;
	_rotation = rotation;
	_scale = scale;
}

SceneObject::~SceneObject() {
	// _mesh and _texture do not need to be deleted, as the data is shared in other areas and not just the particular SceneObject instance.
	delete _material;
}

void SceneObject::DrawMesh() {
	if (_mesh != nullptr && _mesh->Vertices != nullptr && _mesh->Normals != nullptr && _mesh->Indices != nullptr) {
		// Bind the currently assigned texture to
		glBindTexture(GL_TEXTURE_2D, _texture->GetID());

		// Enable vertex, normal and texture arrays for rendering/drawing.
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		// Specify vertice, normal and texture co-ordinate data.
		glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
		glNormalPointer(GL_FLOAT, 0, _mesh->Normals);
		glTexCoordPointer(2, GL_FLOAT, 0, _mesh->TexCoords);

		UpdateMeshMaterial();
		if (_material != nullptr)
		{
			// Specify mesh material properties
			glMaterialfv(GL_FRONT, GL_AMBIENT, &(_material->ambient.x));
			glMaterialfv(GL_FRONT, GL_DIFFUSE, &(_material->diffuse.x));
			glMaterialfv(GL_FRONT, GL_SPECULAR, &(_material->specular.x));
			glMaterialf(GL_FRONT, GL_SHININESS, _material->shininess);
		}

		// Place new matrix onto the stack for this object to apply transformations on.
		glPushMatrix();

		glTranslatef(_position.x, _position.y, _position.z);
		
		glRotatef(_rotation.x, 1.0, 0.0, 0.0);
		glRotatef(_rotation.y, 0.0, 1.0, 0.0);
		glRotatef(_rotation.z, 0.0, 0.0, 1.0);
		
		glScalef(_scale.x, _scale.y, _scale.z);

		// Render all faces stored in the mesh.
		// Mesh loading and rendering assumes all faces are traingles (3 points). If a mesh's file uses polygons, it will not display correctly.
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < _mesh->IndexCount / 3; i++)
		{
			glNormal3f(_mesh->Normals[i].x, _mesh->Normals[i].y, _mesh->Normals[i].z);
			glTexCoord2f(_mesh->TexCoords[i].u, _mesh->TexCoords[i].v);
			glVertex3f(_mesh->Vertices[i].x, _mesh->Vertices[i].y, _mesh->Vertices[i].z);
		}

		glEnd();

		// Remove (or "pop") this matrix off the top of the stack now the mesh has been drawn.
		glPopMatrix();

		// Disable vertex, normal and texture arrays now that drawing is done.
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void SceneObject::UpdateMeshMaterial() {
	if (_material == nullptr)
	{
		_material = MeshLoader::LoadMTL((char*)_mesh->MaterialPath.c_str());
	}
}

void SceneObject::SetMesh(Mesh* mesh) {
	_mesh = mesh;
	if (_material != nullptr) { delete _material; _material = nullptr; }
	UpdateMeshMaterial();
}