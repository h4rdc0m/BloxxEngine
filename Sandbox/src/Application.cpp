#include <BloxxEngine.h>

int main()
{
    BloxxEngine::Renderer renderer;

    if (!renderer.Initialize("Sandbox", 1024, 768))
    {
        return EXIT_FAILURE;
    }

    renderer.Run();

    return EXIT_SUCCESS;
}