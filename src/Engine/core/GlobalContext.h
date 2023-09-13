#pragma once
#include <memory>
class Logger;
class GameEngine;
class GlobalContext
{
public:
    GlobalContext();
    std::shared_ptr<Logger>  mLogger;
    std::unique_ptr<GameEngine> mGameEngine;

};
extern GlobalContext globalContext;
