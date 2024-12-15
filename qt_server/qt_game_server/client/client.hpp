#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QString>
#include <iostream>
#include "item.hpp"  
#include <thread>
#include <chrono>
#include <QTimer>
#include <QThread>

class GameClient : public QObject {
    Q_OBJECT

public:
    explicit GameClient(const QString &serverAddress, quint16 serverPort, QObject *parent = nullptr);
    void sendCoordinates(int x, int y);

private slots:
    void onConnected();
    void onDataReceived();
    void requestInput();  // Новый слот для повторного запроса ввода координат

private:
    QTcpSocket *socket;
    QTextStream *inputStream;  // Поток ввода для повторного запроса
};