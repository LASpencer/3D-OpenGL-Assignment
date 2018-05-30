#include "Application3D.h"

int main() {
	
	// allocation
	auto app = new las::Application3D();

	// initialise and loop
	if (!app->run("OpenGL Graphics Assignment", 1280, 720, false)) {
		system("pause");
	}

	// deallocation
	delete app;

	return 0;
}