#include "client.hpp"

GameClient::GameClient() : socket(new QTcpSocket(this)), waitingForInput(false) {
    connect(socket, &QTcpSocket::connected, this, &GameClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &GameClient::onDataReceived);
}

void GameClient::connectToServer() {
    socket->connectToHost("127.0.0.1", 12345);
}

void GameClient::sendCoordinates(int x, int y) {
    Coordinate coord = { x, y };
    QDataStream out(socket);
    out << coord.x << coord.y;

    qDebug() << "Sent to server: (" << coord.x << ", " << coord.y << ")";
}

void GameClient::onConnected() {
    qDebug() << "Connected to server!";
    requestInput();
}

void GameClient::onDataReceived() {
    if (waitingForInput) {  // Если мы не ожидаем ввода, не продолжаем обработку данных
        QDataStream in(socket);
        Coordinate coord;
        in >> coord.x >> coord.y;

        if (in.status() != QDataStream::Ok) {
            qDebug() << "Error: Failed to read data from server.";
            return;
        }

        qDebug() << "Received from server: (" << coord.x << ", " << coord.y << ")";
        QTextStream(stdout) << "Received coordinates: (" << coord.x << ", " << coord.y << ")\n";

        waitingForInput = false;  // Мы получили данные, теперь можно снова запрашивать ввод
        requestInput();  // Запрашиваем новые координаты
    }
}

void GameClient::requestInput() {

    if (waitingForInput) {
        return;  // Если мы уже ожидаем ввода, не делаем ничего
    }

    waitingForInput = true;  // Устанавливаем флаг, что ожидаем ввод

    QTextStream(stdout) << "Enter coordinates (x y): ";
    int x, y;
    QTextStream(stdin) >> x >> y;
    sendCoordinates(x, y);  // Отправляем координаты

    // Чтобы клиент мог выждать время для ввода и не отправлять сразу
    QThread::msleep(100);  // Добавляем задержку
}

