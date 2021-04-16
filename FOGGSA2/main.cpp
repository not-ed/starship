#include <iostream>
#include "HelloGL.h"

int main(int argc, char* argv[]) {

	HelloGL* game_instance = new HelloGL(argc, argv);

	// A graceful/successful exit is assumed if 0 is returned.
	return 0;
}