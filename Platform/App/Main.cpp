#include "Host.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

#ifdef __EMSCRIPTEN__
    static Host host;
#else
    Host host;
#endif
    return host.Run();
}
