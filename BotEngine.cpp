#include "BotEngine.h"
BotEngine* BotEngine::instance{nullptr};
std::mutex BotEngine::mutex;


BotEngine::BotEngine() = default;

BotEngine* BotEngine::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr)
    {
        instance = new BotEngine();
    }
    return instance;
}

