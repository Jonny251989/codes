#include "server.hpp"

GameServer::GameServer(QObject *parent)
    : QTcpServer(parent), currentPlayerIndex(0) {}

GameServer::~GameServer() {
    // Очистка сокетов при уничтожении сервера
    for (QTcpSocket* socket : clients) {
        socket->close();
    }
}

void GameServer::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);
    connect(clientSocket, &QTcpSocket::readyRead, this, &GameServer::onDataReceived);
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    clients.append(clientSocket);

    if (clients.size() == 2) {
        qDebug() << "Two clients connected, starting the game!";
    }
}

void GameServer::onDataReceived() {
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    // Чтение координат от текущего клиента
    QDataStream in(clientSocket);
    Coordinate coord;
    in >> coord.x >> coord.y;

    qDebug() << "Received from client: (" << coord.x << ", " << coord.y << ")";

    // Переключаем игрока (чтобы отправить координаты следующему)
    currentPlayerIndex = (currentPlayerIndex + 1) % clients.size();
    
    // Выводим индекс следующего клиента для отладки
    qDebug() << "Sending to client with index: " << currentPlayerIndex;

    QTcpSocket* nextClient = clients.at(currentPlayerIndex);  // Находим сокет следующего клиента

    // Проверим, открыт ли сокет следующего клиента
    if (!nextClient->isOpen()) {
        qDebug() << "Error: The next client socket is not open.";
        return;
    }

    // Создаем поток для записи данных в сокет следующего клиента
    QDataStream out(nextClient);
    out << coord.x << coord.y;  // Отправляем координаты следующему клиенту

    qDebug() << "Forwarding to next client: (" << coord.x << ", " << coord.y << ")";
}