#include <QCoreApplication>
#include <QThread>
#include <QObject>
#include "server.hpp"


int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    GameServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server failed to start!";
        return 1;
    }

    qDebug() << "Server started on port 12345";
    return a.exec();
}