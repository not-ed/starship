#include "HelloGL.h"


Level* l;

HelloGL::HelloGL(int argc, char* argv[]) {
	// Set random number seed based on time the program is run.
	srand(time(NULL));

	InitGL(argc, argv);

	// Flush keyboard states before anything starts being read from callbacks.
	for (int i = 0; i < ASCII_COUNT; i++)
	{
		_keyStates[i] = false;
	}

	InitObjects();

	glutMainLoop();
}

HelloGL::~HelloGL() {
	delete l;
}

void HelloGL::InitObjects() {
	l = new Level();
}

void HelloGL::InitGL(int argc, char* argv[]) {
	GLUTCallbacks::Init(this);

	glutInit(&argc, argv); // Open

	// Enable double buffering and depth testing
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Starship"); // Close

	// Switch to the projection matrix in order to set up the camera/viewport.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set viewport to the dimensions of the initial window size.
	glViewport(0, 0, 1280, 720);

	// Set the viewport's perspective.
	// ZNear must be a positive non-zero value for depth testing to work correctly.
	gluPerspective(90, 1280.0f/720.0f, 1.0f, 1000.0f);

	// Switch back to model view matrix in order to work with models again.
	glMatrixMode(GL_MODELVIEW);

	// Enable Backface Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable the use of 2D textures
	glEnable(GL_TEXTURE_2D);

	// Depth Testing
	glEnable(GL_DEPTH_TEST);
	
	// Callbacks
	glutDisplayFunc(GLUTCallbacks::Display); // Assign Display() as a callback function for whenever the scene needs to be re-drawn
	glutTimerFunc(REFRESH_RATE, GLUTCallbacks::Timer, REFRESH_RATE); // Trigger Timer() as a callback every 16ms, whilst passing 16ms into the function's parameter
	glutKeyboardFunc(GLUTCallbacks::Keyboard); // Implement Keyboard input to Keyboard()
	glutKeyboardUpFunc(GLUTCallbacks::KeyboardUp);
}

void HelloGL::Update() {
	l->Update(_keyStates);

	// Make the scene re-draw itself.
	glutPostRedisplay();
}

void HelloGL::Display() {
	// Clear the scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Begin matrix calculation isolation for rotation
	glPushMatrix();

	l->Draw();

	// End matrix calculation isolation
	glPopMatrix();

	// "Flush" what has been drawn to the Graphics Card
	glFlush(); 
	// Swap display buffers
	glutSwapBuffers();
}

void HelloGL::Keyboard(unsigned char key, int x, int y) {
	_keyStates[key] = true;
}

void HelloGL::KeyboardUp(unsigned char key, int x, int y) {
	_keyStates[key] = false;
}

//void HelloGL::DrawString(const char* text, Vector3* position, Color* color) {
//	glPushMatrix();
//	glTranslatef(position->x, position->y, position->z);
//	
//	glRasterPos2f(0.0f, 0.0f);
//	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text);
//	glPopMatrix();
//}