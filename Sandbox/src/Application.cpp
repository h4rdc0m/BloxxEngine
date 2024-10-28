#include <BloxxEngine.h>

int main()
{
    BloxxEngine::Engine engine;

    if (!engine.Init("Sandbox", 1024, 768))
    {
        return EXIT_FAILURE;
    }

    engine.Run();

    return EXIT_SUCCESS;
}