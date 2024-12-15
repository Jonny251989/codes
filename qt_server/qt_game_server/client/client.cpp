#include "client.hpp"

GameClient::GameClient(const QString &serverAddress, quint16 serverPort, QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this)), inputStream(new QTextStream(stdin)) {

    socket->connectToHost(serverAddress, serverPort);
    connect(socket, &QTcpSocket::connected, this, &GameClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &GameClient::onDataReceived);
}

void GameClient::sendCoordinates(int x, int y) {
    Coordinate coord = { x, y };
    QDataStream out(socket);
    out << coord.x << coord.y;

    qDebug() << "Sent to server: (" << coord.x << ", " << coord.y << ")";
}

void GameClient::onConnected() {
    qDebug() << "Connected to server!";
    requestInput();  // Запрашиваем координаты сразу после подключения
}

void GameClient::onDataReceived() {
    QDataStream in(socket);
    Coordinate coord;
    in >> coord.x >> coord.y;

    // Если данные не были получены корректно
    if (in.status() != QDataStream::Ok) {
        qDebug() << "Error: Failed to read data from server.";
        return;
    }

    qDebug() << "Received from server: (" << coord.x << ", " << coord.y << ")";
    QTextStream(stdout) << "Получены координаты: (" << coord.x << ", " << coord.y << ")\n";

    requestInput();  // После получения данных снова запрашиваем ввод
}


void GameClient::requestInput() {
    QTextStream(stdout) << "Введите координаты для отправки (x y): ";
    int x, y;
    (*inputStream) >> x >> y;
    sendCoordinates(x, y);  // Отправляем координаты на сервер
}