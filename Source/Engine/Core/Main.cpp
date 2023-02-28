#include <precomp.h>
#include "Application.h"


#if defined(WIN32) && !defined(_DEBUG)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
int main()
#endif
{
    Log::Init();
    auto* app = Engine::NewApp();
    if(!app->Init())
        return 1;

    app->Run();

    app->Destroy();
    delete app;

    return 0;
}
