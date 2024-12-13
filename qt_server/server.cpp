#include "server.hpp"
#include <QTcpSocket>
#include <iostream>

Server::Server(quint16 port)  {
    server.listen(QHostAddress::Any, port);
    connect(&server, &QTcpServer::newConnection, this, &Server::onNewConnection);
    std::cout << "Server started, waiting for clients..." << std::endl;
}

void Server::onNewConnection() {
    QTcpSocket *clientSocket = server.nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onClientMessage);
    clients.append(clientSocket);
    std::cout << "New client connected." << std::endl;
}

void Server::onClientMessage() {
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (!senderSocket) return;

    QByteArray data = senderSocket->readAll();
    Item item = Item::deserialize(data);

    // Пересылаем Item всем другим клиентам
    for (QTcpSocket *clientSocket : clients) {
        if (clientSocket != senderSocket) {
            clientSocket->write(data);
        }
    }
}

Server::~Server(){
    qDeleteAll(clients);
};