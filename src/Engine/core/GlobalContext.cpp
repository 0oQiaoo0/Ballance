#include "GlobalContext.h"

#include "Logger.h"
#include "../GameEngine.h"


GlobalContext::GlobalContext()
{
    mLogger = std::make_shared<Logger>();
    mGameEngine = std::make_unique<GameEngine>();
}
GlobalContext globalContext;