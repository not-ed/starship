#include "GLUTCallbacks.h"
#include "HelloGL.h"

namespace GLUTCallbacks {
	// Anonymous namespace - only accessable to code within this namespace.
	namespace {
		HelloGL* helloGL = nullptr;
	}

	void Init(HelloGL* gl) {
		helloGL = gl;
	}

	void Display() {
		if (helloGL != nullptr) {
			helloGL->Display();
		}
	}

	void Timer(int preferredRefresh) {
		int updateTime = glutGet(GLUT_ELAPSED_TIME);
		helloGL->Update();
		updateTime = glutGet(GLUT_ELAPSED_TIME) - updateTime;

		// Call itself again every 16ms minus how long the previous Update took, if it took longer than 16ms, then it will execute again straight away
		glutTimerFunc(preferredRefresh - updateTime, GLUTCallbacks::Timer, preferredRefresh);
	}

	void Keyboard(unsigned char key, int x, int y) {
		helloGL->Keyboard(key, x, y);
	}

	void KeyboardUp(unsigned char key, int x, int y) {
		helloGL->KeyboardUp(key, x, y);
	}
}