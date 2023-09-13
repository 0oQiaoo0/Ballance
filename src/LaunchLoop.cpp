#include "Engine/GameEngine.h"
#include "time.h"
#include "Engine/core/GlobalContext.h"
#include "Engine/core/GlobalContext.h"
#include "Engine/core/Logger.h"
#include "Engine/core/macro.h"
#include "Game/Timer.h"
#include "Network/Client.h"
#include "Network/Server.h"
#include "Network/test.helloworld.h"

extern int demoMain(int, const char* const*);
void keydonw()
{
    LOG_ERROR("sfa");

    globalContext.mGameEngine.reset();
    //demoMain(1,nullptr);
}
int main(int argc, char* argv[])
{

    globalTimer.Reset();
    globalContext.mGameEngine->Init();
    InputSystem::BindKeyDownEvent('x', keydonw);
    bool Quit = false;
    MSG msg = { 0 };
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    while (msg.message != WM_QUIT)
    {
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                Quit = true;
        }
        if (Quit) {
            break;
        }

        globalTimer.Tick();
        globalContext.mGameEngine->Tick(globalTimer.DeltaTime());

        // Sleep(100);
    }
    return 0;
}
