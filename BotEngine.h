#ifndef BOTENGINE_H
#define BOTENGINE_H
#include <mutex>

#define engine BotEngine::getInstance()

class BotEngine {
private:
    static BotEngine* instance;
    static std::mutex mutex;
protected:
    BotEngine();
    ~BotEngine(){}
public:
    BotEngine(BotEngine const&) = delete;
    void operator=(const BotEngine&) = delete;
    static BotEngine* getInstance();

    // Engine variables
    bool hasTarget = false;
    bool isLooting = false;
    bool isWalking = false;
};



#endif //BOTENGINE_H
