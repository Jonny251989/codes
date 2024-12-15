#include <QCoreApplication>
#include <QThread>
#include <QObject>
#include "client.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    GameClient client("127.0.0.1", 12345);

    return a.exec();
}