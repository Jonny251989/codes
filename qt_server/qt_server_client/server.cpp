#include "server.hpp"

GameServer::GameServer() {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &GameServer::onNewConnection);
}

void GameServer::start() {
    if (server->listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server started on port 12345";
    } else {
        qDebug() << "Server failed to start!";
    }
}

void GameServer::onNewConnection() {
    QTcpSocket* clientSocket = server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &GameServer::onDataReceived);
    qDebug() << "New client connected!";
    clients.push_back(clientSocket);
}

void GameServer::onDataReceived() {
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(clientSocket);
    Coordinate coord;
    in >> coord.x >> coord.y;

    qDebug() << "Received from client: (" << coord.x << ", " << coord.y << ")";

    // Forward the coordinates to the next client (round-robin)
    static int currentPlayerIndex = 0;
    currentPlayerIndex = (currentPlayerIndex + 1) % clients.size();
    QTcpSocket* nextClient = clients.at(currentPlayerIndex);
    QDataStream out(nextClient);
    out << coord.x << coord.y;

    qDebug() << "Forwarding to next client: (" << coord.x << ", " << coord.y << ")";
}