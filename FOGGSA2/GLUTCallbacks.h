#pragma once
#ifndef GLUTCALLBACKS_H
#define GLUTCALLBACKS_H

class HelloGL;

namespace GLUTCallbacks {
	void Init(HelloGL* gl);
	void Display();
	void Timer(int preferred_refresh);
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
}

#endif