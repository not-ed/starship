#include "Pyramid.h"

Pyramid::Pyramid(Mesh* mesh, float x, float y, float z) : SceneObject(mesh, nullptr){
	_position.x = x;
	_position.y = y;
	_position.z = z;

	_ro = 0.0f;
}

Pyramid::~Pyramid() {

}

void Pyramid::Draw() {
	//TODO: Dry principle violation (use cube as "master")
	if (_mesh->Vertices != nullptr && _mesh->Normals != nullptr && _mesh->Indices != nullptr) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
		glColorPointer(3, GL_FLOAT, 0, _mesh->Normals);

		glPushMatrix();

		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(_ro, 0.0f, 1.0f, 0.0f);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, _mesh->Indices);

		glPopMatrix();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}
void Pyramid::Update() {
	_ro += 1.32f;
}