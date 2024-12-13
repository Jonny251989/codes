#include <QCoreApplication>
#include <QThread>
#include "client.hpp"
#include "server.hpp"
#include "item.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Создаем и запускаем сервер в отдельном потоке
    Server *server = new Server(1234);  // Родительский объект не передан

    // Запуск клиентов
    Client *client1 = new Client("localhost", 1234);  // Родительский объект не передан
    Client *client2 = new Client("localhost", 1234);

    return a.exec();
}