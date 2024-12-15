#include <QCoreApplication>
#include <QThread>
#include <QObject>
#include "client.hpp"
#include "server.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Запуск сервера в потоке
    QThread* serverThread = new QThread();
    GameServer* server = new GameServer();
    server->moveToThread(serverThread);

    QObject::connect(serverThread, &QThread::started, server, &GameServer::start);
    serverThread->start();

    // Запуск клиентов
    GameClient* client1 = new GameClient();
    client1->connectToServer();

    GameClient* client2 = new GameClient();
    client2->connectToServer();

    // Мы можем немного подождать, чтобы сервер успел запуститься
    QThread::sleep(1);

    return a.exec();
}