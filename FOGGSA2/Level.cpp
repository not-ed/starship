#include "Level.h"

Level::Level() {
	_sceneObjects = new LinkedLists();
	
	InitializeLights();

	LoadObjects();

	SetUpCamera();

	Reset();
}

Level::~Level() {
	if (_sceneObjects != nullptr)
	{
		_sceneObjects->DeleteList(&_sceneObjectsHead);
		delete _sceneObjects;
	}

	delete _camera;

	delete _sceneLight;

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

}

void Level::LoadObjects() {
	// Create earth prop.
	_earthProp = new SceneObject(ModelAtlas::Instance()->GetMesh(Model::ID::GLOBE), TextureAtlas::Instance()->GetTexture(Texture::ID::GLOBE), Vector3{ 0.0f,100.0f,-1000.0f }, Vector3{ 0.0f,1.0f * (rand() % 360),0.0f }, Vector3{ 500.0f,500.0f,500.0f });
	_sceneObjects->MakeNode(&_sceneObjectsHead, _earthProp);

	// Create player ship.
	_player = new PlayerShip(ModelAtlas::Instance()->GetMesh(Model::ID::SHIP), TextureAtlas::Instance()->GetTexture(Texture::ID::SHIP), Vector3{ 0.0f,4.0f,-20.0f });
	_sceneObjects->MakeNode(&_sceneObjectsHead, _player);

	// Create "corridor"/cavern segments, placed before the earth prop and player.
	for (int i = 0; i < 30; i++)
	{
		Corridor* new_corridor = new Corridor(ModelAtlas::Instance()->GetMesh(Model::ID::CORRIDOR), TextureAtlas::Instance()->GetTexture(Texture::ID::ROCK), Vector3{ 0.0f,0.0f,-20.0f * i });
		new_corridor->SetTravelSpeed(&_scrollSpeed);
		_sceneObjects->InsertFirst(&_sceneObjectsHead, new_corridor);
	}
}

void Level::InitializeLights() {
	// Enable Scene Lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Initialize Scene Light with the desired parameters
	_sceneLight = new Lighting();
	_sceneLight->position.x = -50.0;
	_sceneLight->position.y = -8.0;
	_sceneLight->position.z = -20.0;
	_sceneLight->position.w = 0.0;

	_sceneLight->ambient.x = 1.0 / 4;
	_sceneLight->ambient.y = 0.94 / 4;
	_sceneLight->ambient.z = .72 / 4;
	_sceneLight->ambient.w = 1;
	_sceneLight->diffuse.x = .048;
	_sceneLight->diffuse.y = .048;
	_sceneLight->diffuse.z = .048;
	_sceneLight->diffuse.w = 1;
	_sceneLight->specular.x = .2;
	_sceneLight->specular.y = .2;
	_sceneLight->specular.z = .2;
	_sceneLight->specular.w = 1;
}

void Level::SetUpCamera() {
	_camera = new Camera();
}

void Level::Update(const bool* key) {
	glLightfv(GL_LIGHT0, GL_POSITION, &(_sceneLight->position.x));
	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_sceneLight->ambient.x));

	// Update sceneobjects in the linked list
	ListNode* currentObj = _sceneObjectsHead;
	while (currentObj != nullptr)
	{
		if (currentObj->object != nullptr) {
			// Update each object through the linked list in sequence.
			currentObj->object->Update(key);

			if (currentObj->object != _player && _player->IsAlive())
			{
				// Because the player can only collide in this context with just debris, making contact with any other object guarantees that it is Debris.
				if (Collision::Sphere(_player,currentObj->object))
				{
					_player->Kill();
				}
			}
		}
		currentObj = currentObj->next;
	}

	// Continual rotation of earth prop
	_earthProp->SetRotation(Vector3{ 0.0f,_earthProp->GetRotation().y - 0.02f,32.4f });

	if (_player->IsAlive())
	{
		// Increment score (16ms per frame)
		_currentScore += 0.016f;

		if (_currentScore > 1 + (1.5 * _debrisCount))
		{
			SpawnDebris();
		}

		// Restrict the player ship postion the level bounds.
		Vector3 new_player_pos = _player->GetPosition();
		if (new_player_pos.x > _horizontalBounds.y)
		{
			new_player_pos.x = _horizontalBounds.y;
		}
		else if (new_player_pos.x < _horizontalBounds.x) {
			new_player_pos.x = _horizontalBounds.x;
		}
		if (new_player_pos.y > _verticalBounds.y)
		{
			new_player_pos.y = _verticalBounds.y;
		}
		else if (new_player_pos.y < _verticalBounds.x) {
			new_player_pos.y = _verticalBounds.x;
		}
		_player->SetPosition(new_player_pos);

		
		// Increment time spent scrolling so far
		_currentScrollTime += 16;

		// Increment scroll speed if enough time has passed
		while (_currentScrollTime >= (int)((16*60)*_scrollIncrementRate))
		{
			_scrollSpeed += _scrollIncrementAmount;
			if (_scrollSpeed > _maxScrollSpeed) {
				_scrollSpeed = _maxScrollSpeed;
			}
			_currentScrollTime -= (int)((16 * 60) * _scrollIncrementRate);
		}
	}
	else {
		// Slow down the scrolling until at a complete stop on player death.
		if (_scrollSpeed > 0.0f)
		{
			_scrollSpeed -= 0.01f;
			if (_scrollSpeed < 0.0f)
			{
				_scrollSpeed = 0.0f;
			}
		}

		if (key['R'] || key['r'])
		{
			Reset();
		}
	}

	// Update camera to continually follow position of the player ship.
	if (_camera != nullptr) {
		_camera->eye.x = _player->GetPosition().x / 2; _camera->eye.y = 5; _camera->eye.z = -10.0; // Position of camera (or eye point)
		_camera->center.x = _player->GetPosition().x; _camera->center.y = _player->GetPosition().y; _camera->center.z = _player->GetPosition().z; // Center/Subject of the camera.
		_camera->up.x = 0.0f; _camera->up.y = 1.0f; _camera->up.z = 0.0f;

		glLoadIdentity();
		gluLookAt(_camera->eye.x, _camera->eye.y, _camera->eye.z, _camera->center.x, _camera->center.y, _camera->center.z, _camera->up.x, _camera->up.y, _camera->up.z);
	}
}

void Level::Draw(){
	ListNode* currentObj = _sceneObjectsHead;
	while (currentObj != nullptr)
	{
		if (currentObj->object != nullptr) {
			// Draw each object through the linked list in sequence.
			currentObj->object->Draw();
		}
		currentObj = currentObj->next;
	}
	
	
	if (_player->IsAlive())
	{
		// Draw a diagetic text counter that looks like it is part of the ship itself.
		DrawString(std::to_string(_currentScore).c_str(), new Vector3{ _player->GetPosition().x - 0.625f,_player->GetPosition().y - 0.2f,_camera->eye.z - 6.0f }, new Color{ 0.0f,0.796f,1.0f });
	}
	else {
		// If the player is dead, show a game over message and prompt to restart.
		DrawString("YOUR SCORE:", new Vector3{ _camera->center.x - 0.99f,_camera->center.y + 1.2f,_camera->eye.z - 6.0f }, new Color{ 1.0f,0.1f,0.1f });
		DrawString(std::to_string(_currentScore).c_str(), new Vector3{ _camera->center.x - 0.625f,_camera->center.y + 0.88f,_camera->eye.z - 6.0f }, new Color{ 1.0f,0.1f,0.1f });
		DrawString("Press 'R' To Restart", new Vector3{ _camera->center.x - 1.25f,_camera->center.y + 0.56f,_camera->eye.z - 6.0f }, new Color{ 1.0f,0.1f,0.1f });
	}
	
}

void Level::SpawnDebris() {
	// Determine a random position within the level's horizontal and vertical bounds to place the new Debris object on the x and y axis.
	Vector2 debris_xy;
	debris_xy.x = _horizontalBounds.x + (fmod(rand(), abs(_horizontalBounds.x - _horizontalBounds.y)));
	debris_xy.y = _verticalBounds.x + (fmod(rand(), abs(_verticalBounds.x - _verticalBounds.y)));

	Debris* new_debris = new Debris(ModelAtlas::Instance()->GetMesh(Model::ID::RUBBLE), TextureAtlas::Instance()->GetTexture(Texture::ID::ROCK), Vector3{debris_xy.x,debris_xy.y,-200});
	_sceneObjects->MakeNode(&_sceneObjectsHead, new_debris);
	new_debris->SetBounds(_horizontalBounds,_verticalBounds);
	new_debris->SetTravelSpeed(&_scrollSpeed);
	_debrisCount++;
}

void Level::Reset() {
	// Restore Player state
	_player->Reset();
	// Reset prop travel speeds.
	_scrollSpeed = _startingScrollSpeed;
	_currentScrollTime = 0;
	
	// Delete all debris
	// Because all debris objects are positioned after the player in the linked list, all nodes after where the player's node is can be erased to clear all Debris specifically.
	_sceneObjects->DeleteAllAfter(_sceneObjects->Find(_sceneObjectsHead, _player));
	_debrisCount = 0;

	// Reset Score
	_currentScore = 0.0f;
}

void Level::DrawString(const char* text, Vector3* position, Color* color) {
	glPushMatrix();

	// Briefly Disable lighting and textures so that text color isn't affected by lighting or the current texture, and to ensure text isn't blocked by meshes or objects in front of it.
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glColor3f(color->r, color->g, color->b);

	glTranslatef(position->x, position->y, position->z);
	glRasterPos2f(0.0f, 0.0f);

	
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text);

	// Re-enable lighting and textures now that drawing text is all done.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glPopMatrix();
}