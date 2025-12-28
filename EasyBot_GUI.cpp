#include "MainBot/BotController.h"
#include <QStyleFactory>



int main(int argc, char* argv[]) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    auto localPlayer = proto->getLocalPlayer();
    auto playerPos = proto->getPosition(localPlayer);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << duration.count() << std::endl;
    QApplication a(argc, argv);

    std::filesystem::create_directory("Save/");
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    BotView w;
    BotController controller(&w);
    w.show();
    return QApplication::exec();
}
