#include <Engine.h>
#include <cstdlib>
#include <exception>
#include <iostream>

int main(int argc, char** argv)
{
	BE::Engine engine = {"BloxxEngine Sandbox"};
	
	try {
		engine.Run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
