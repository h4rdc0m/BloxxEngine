#include <Engine.h>

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
  BE::Engine engine{"BloxxEngine", 800, 600};

  try {
    engine.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
